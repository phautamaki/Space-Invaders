//=============================================================================
#include "CGun.h"
#include "define.h"
#include "CEntity.h"
#include "CFactory.h"
#include "CSoundBank.h"
#include "CCamera.h"
#include "CArea.h"

#include "functions.h"

//=============================================================================
CGun::CGun() {
	Type		= 0;
	Level		= 0;
	ChargeLevel = 0;
	ChargeStart = 0;
	LastShot	= 0;
	BeamOn		= false;
}

//=============================================================================
bool CGun::OnLoad() {
	// Set to default gun and level
	Reset();

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
		int X = CFactory::Factory.GetPlayer()->X;
		int Y = CFactory::Factory.GetPlayer()->Y + (PLAYER_SPRITE_HEIGHT / 2);

		// Find closest Entity or Tile
		CEntity* ClosestEntity = CFactory::Factory.GetClosest(X, Y, ENTITY_TYPE_ENEMY, true, true, 40);
		CTile*	 ClosestTile   = CArea::AreaControl.GetNextHorizontalTile(X,Y);
		int ClosestX = ClosestTile->X;
		if( ClosestEntity != NULL && ClosestEntity->X < ClosestX ) {
			ClosestX = ClosestEntity->X;
			ClosestEntity->HP = ClosestEntity->HP - BULLET_BEAM_STR;
		}

		// Beam width
		int BeamWidth = WWIDTH;
		if( ClosestX > X ) {
			BeamWidth = ClosestX - X;
		}

		// Graphic
		SDL_Rect LaserBeam;
		LaserBeam.x = X-CCamera::CameraControl.GetX() + PLAYER_SPRITE_WIDTH;
		LaserBeam.y = Y;
		LaserBeam.w = BeamWidth - PLAYER_SPRITE_WIDTH;
		LaserBeam.h = 20;

		SDL_FillRect(Surf_Display, &LaserBeam, SDL_MapRGB(Surf_Display->format, 211, 211, 211));
	}
}

//-----------------------------------------------------------------------------
void CGun::OnCleanup() {
}

//=============================================================================
void CGun::ChangeType(int nType) {
	if( nType == Type ) {
		Level++;
	}
	else {
		Type = nType;
		Level = 0;
		ChargeStart = 0;
		LastShot = 0;
	}
}

//-----------------------------------------------------------------------------
void CGun::Reset() {
	Type   = GUN_NORMAL;
	//Type   = GUN_BEAM;
	Level  = 1;
	BeamOn = false;
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

//-----------------------------------------------------------------------------
void CGun::Deactivate() {
	Shoot();
	ChargeStart = 0;
	ChargeLevel = 0;
	BeamOn = false;
}

void CGun::Shoot() {
	int X = CFactory::Factory.GetPlayer()->X;
	int Y = CFactory::Factory.GetPlayer()->Y;

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
			//CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_BEAM, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2);
			break;
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

//=============================================================================
