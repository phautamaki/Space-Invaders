//=============================================================================
#include "CGun.h"

#include "CEntity.h"
#include "CCamera.h"
#include "CArea.h"

#include "CFactory.h"
#include "CSoundBank.h"

#include "functions.h"
#include "define.h"
#include "Paths.h"

//=============================================================================
CGun::CGun() {
	Type		= 0;
	Level		= 0;
	ChargeLevel = 0;
	ChargeStart = 0;
	LastShot	= 0;
	LastMissileShot = 0;
	MissileDelay = PLAYER_SHOOT_DELAY_MISSILE;
	BeamOn		= false;
	BeamWidth   = 0;
}

//=============================================================================
bool CGun::OnLoad() {
	// Set to default gun and level
	Reset();

	// Load laser beam sprite
	if((LaserBody = CSurface::OnLoad(PATH_IMAGES PATH_ITEMS "bullet_beam.png")) == NULL) {
		return false;
	}
	if((LaserEnd = CSurface::OnLoad(PATH_IMAGES PATH_ITEMS "bullet_beam_end.png")) == NULL) {
		return false;
	}

    return true;
}

//-----------------------------------------------------------------------------
void CGun::OnLoop() {
	if( ChargeStart > 0 ) {
		Charge();
	}
}

void CGun::OnRender(SDL_Surface* Surf_Display) {
	// ChargeBar
	int BarStart = WWIDTH/2 - 50;
	SDL_Rect RectRed;
	RectRed.x = BarStart;
	RectRed.y = 30;
	RectRed.w = ChargeLevel * 10;
	RectRed.h = 10;
	SDL_Rect RectGray;
	RectGray.x = BarStart;
	RectGray.y = 30;
	RectGray.w = 100;
	RectGray.h = 10;

	SDL_FillRect(Surf_Display, &RectGray, SDL_MapRGB(Surf_Display->format, 211, 211, 211));
	SDL_FillRect(Surf_Display, &RectRed, SDL_MapRGB(Surf_Display->format, 255, 0, 0));

	if( BeamOn ) {
		// Beam start
		int X = (int)CFactory::Factory.GetPlayer()->X + PLAYER_SPRITE_WIDTH;
		int Y = (int)CFactory::Factory.GetPlayer()->Y + (PLAYER_SPRITE_HEIGHT / 2);

		UpdateBeamWidth(X, Y);
		if( BeamWidth > 0 ) {
			int BodyW = 20;
			int BodyH = 9;
			int EndW = 13;
			int EndH = 11;

			// Draw laser body
			int LastX = 0;
			for( int i = 0; i < BeamWidth / BodyW; i++ ) {
				LastX = X-CCamera::CameraControl.GetX() + i*BodyW;
				if( LastX >= X-CCamera::CameraControl.GetX() ) {
					CSurface::OnDraw(Surf_Display, LaserBody, LastX, Y, BodyW*Level, 0, BodyW, BodyH);
				}
			}
			// Draw laser end
			int Extra = BeamWidth % BodyW;
			if( Extra != 0 ) {
				if( LastX+BodyW > X-CCamera::CameraControl.GetX() ) {
					CSurface::OnDraw(Surf_Display, LaserBody, LastX+BodyW, Y, BodyW*Level, 0, Extra, BodyH);
				}
			}
			if( X-CCamera::CameraControl.GetX() + BeamWidth - EndW > X-CCamera::CameraControl.GetX() ) {
				CSurface::OnDraw(Surf_Display, LaserEnd, X-CCamera::CameraControl.GetX() + BeamWidth - EndW, Y-1, BodyW*Level, 0, Extra, BodyH);
			}
			 
		}
	}
}

//-----------------------------------------------------------------------------
void CGun::OnCleanup() {
	SDL_FreeSurface(LaserBody);
	SDL_FreeSurface(LaserEnd);
}

//=============================================================================
void CGun::ChangeType(int nType) {
	if( nType == Type ) {
		Level++;
		/*
		// Missiles will get shorter delay
		MissileDelay = MissileDelay-200;
		if( MissileDelay < 100 ) {
			MissileDelay = 100;
		}
		*/
		// Laser will upgrade once
		if( Type == GUN_BEAM ) {
			if( Level > 1 ) {
				Level--;
			}
		}
	}
	else {
		Reset();
		Type = nType;
	}
}

//-----------------------------------------------------------------------------
void CGun::Reset() {
	Type   = GUN_NORMAL;
	Level  = 0;
	ChargeStart = 0;
	ChargeLevel = 0;
	BeamOn = false;
	LastShot = 0;
	LastMissileShot = 0;
	MissileDelay = PLAYER_SHOOT_DELAY_MISSILE;
}

//-----------------------------------------------------------------------------
void CGun::Activate() {
	if( Type == GUN_BEAM ) {
		Shoot();
	}
	else if( ChargeStart == 0 ) {
		ChargeStart = SDL_GetTicks();
	}
}

//------------------------------------------------------ -----------------------
void CGun::Deactivate() {
	Shoot();
	ChargeStart = 0;
	ChargeLevel = 0;
	BeamOn = false;
	BeamWidth = 0;
	if(BeamChannel != -1) {
		CSoundBank::SoundControl.StopChannel(BeamChannel);
		BeamChannel = -1;
	}
}

void CGun::Shoot() {
	int X = (int)CFactory::Factory.GetPlayer()->X;
	int Y = (int)CFactory::Factory.GetPlayer()->Y;

	switch( Type ) {
		case GUN_NORMAL:
			// Can't shoot too fast
			if( LastShot + PLAYER_SHOOT_DELAY < SDL_GetTicks() ) {
				int CurrentBulletType = 0;
				if( ChargeLevel < 10 ) {
					CurrentBulletType = ENTITY_SUBTYPE_BULLET_NORMAL;
					if( Level != 0 ) {
						CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_SMALL_45U, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2 -5);
						CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_SMALL_45D, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2 +5);
					}
					CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "ShootingSoundBasic");
				}
				else if( ChargeLevel >= 10 ) {
					CurrentBulletType = ENTITY_SUBTYPE_BULLET_CHARGE1;
					CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "ShootingSoundBig");
				}
				CFactory::Factory.CreateBullet(CurrentBulletType, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2);
				LastShot = SDL_GetTicks();
			}
			break;
		case GUN_BEAM:
			BeamOn = true;
			//BeamChannel = CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "ShootingSoundBeam", true);
			//CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_BEAM, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2);
			break;
		case GUN_MISSILES:
			// Always shoot basic bullet
			if( LastShot + PLAYER_SHOOT_DELAY < SDL_GetTicks() ) {
				CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "ShootingSoundBasic");
				CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_NORMAL, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2);
				LastShot = SDL_GetTicks();
			}
			// Can't shoot too fast
			if( LastMissileShot + MissileDelay < SDL_GetTicks() ) {
				if( Level != 0 ) {
					CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_MISSILE, (X + PLAYER_SPRITE_WIDTH-5), (Y + PLAYER_SPRITE_HEIGHT / 2-6));
					CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_MISSILE, (X + PLAYER_SPRITE_WIDTH/2-5), Y-7);
					CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_MISSILE, (X + PLAYER_SPRITE_WIDTH/2-5), (Y + PLAYER_SPRITE_HEIGHT)-7);
				}
				else {
					CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_MISSILE, (X + PLAYER_SPRITE_WIDTH-5), (Y + PLAYER_SPRITE_HEIGHT / 2-6));
				}
				LastMissileShot = SDL_GetTicks();
			}
		default:
			break;
	}
}

//-----------------------------------------------------------------------------
void CGun::Charge() {
	unsigned int ChargeDelay = 100;
	if(static_cast<unsigned int>(ChargeStart + ChargeDelay) < SDL_GetTicks()) {
		if( ChargeLevel != 10 ) {
			ChargeLevel++;
		}
		ChargeStart = SDL_GetTicks();
	}
}

//-----------------------------------------------------------------------------
int CGun::UpdateBeamWidth(int StartX, int StartY) {
	int  MaxWidth   = WWIDTH*2;
	bool FoundTile  = false;
	bool FoundEnemy = false;

	if( StartX < 0 || StartY < 0 ) {
		return MaxWidth;
	}

	// Find closest Entity or Tile
	CEntity* ClosestEntity = CFactory::Factory.GetClosest(StartX, StartY, ENTITY_TYPE_ENEMY, true, true, 13);
	CTile*	 ClosestTile   = CArea::AreaControl.GetNextHorizontalTile(StartX,StartY);
	if( ClosestTile != NULL || ClosestEntity != NULL ){
		// Need a big enough value in case we don't have tile
		int ClosestX = X+WWIDTH*2;
		if( ClosestTile != NULL ) {
			ClosestX = ClosestTile->X;
		}
		if( ClosestEntity != NULL && ClosestEntity->X < ClosestX ) {
			ClosestX = (int)ClosestEntity->X;
			FoundEnemy = true;
		}
		else if( ClosestTile != NULL ){
			FoundTile = true;
		}

		// Final width
		if( ClosestX > StartX ) {
			MaxWidth = ClosestX - StartX;
		}
	}
	
	// BWidth now contains maximum width for beam
	// Let's calculate correct width based on our current beam length
	if( MaxWidth <= BeamWidth ) {
		BeamWidth = MaxWidth;

		// Damage
		if( FoundTile ){
			ClosestTile->Damage(BULLET_BEAM_STR+(BULLET_BEAM_STR*Level));
		}
		else if( FoundEnemy ) {
			ClosestEntity->Damage(BULLET_BEAM_STR+(BULLET_BEAM_STR*Level));
		}
	}
	else {
		BeamWidth = BeamWidth + PLAYER_BULLET_BEAM_SPEED;
	}

	return MaxWidth;
}

//=============================================================================
