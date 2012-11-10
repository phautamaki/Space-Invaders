//=============================================================================
#include "CPlayer.h"
#include "CSoundBank.h"
#include "CFactory.h"
#include "functions.h"
#include "Paths.h"

#include "CAppStateGame.h"

//=============================================================================
CPlayer::CPlayer() {
	Type = ENTITY_TYPE_PLAYER;

	Lives	= PLAYER_STARTING_LIVES;
	Points	= 0;

	MoveUp		= false;
	MoveDown	= false;
	TookHit		= false;

	MakeDeathScene = false;
	DeathMoment = 0;

	MaxSpeedX = PLAYER_MAX_SPEED_X;
	MaxSpeedY = PLAYER_MAX_SPEED_Y;

	GunType = GUN_NORMAL;
	GunLevel = 1;
	ChargeLevel = 0;
	ChargeStart = 0;
	LastShot	= 0;

	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ShootingSoundBasic", PATH_EFFECTS FILENAME_SHOOTING_BASIC) ? debug("Shit hit the fan when loading ShootingSoundBasic.") : debug("Loading ShootingSoundBasic was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ShootingSoundBig", PATH_EFFECTS FILENAME_SHOOTING_BIG) ? debug("Shit hit the fan when loading ShootingSoundBig.") : debug("Loading ShootingSoundBig was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "PlayerCrashingSound", PATH_EFFECTS FILENAME_PLAYER_CRASHING) ? debug("Shit hit the fan when loading PlayerCrashingSound.") : debug("Loading PlayerCrashingSound was a great success!");
	
}

//=============================================================================
// Stop player's moving horizontally
void CPlayer::StopMoveX() {

	// If left or right isn't pressed and speed differs from CAMERA_SPEED,
	// let's start to brake player's speed until...
	if(SpeedX > CAMERA_SPEED) {
		AccelX = -1;
	}
	else if(SpeedX < CAMERA_SPEED) {
		AccelX =  1;
	}

	// ... it is slow enough to set to same as CAMERA_SPEED
	if(SpeedX < (CAMERA_SPEED+1) && SpeedX > (-CAMERA_SPEED-1)) {
		AccelX = 0;
		SpeedX = CAMERA_SPEED;
	}
}

//------------------------------------------------------------------------------
// Stop player's move vertically
void CPlayer::StopMoveY() {

	if(SpeedY > 0) {
		AccelY = -1;
	}
	else if(SpeedY < 0) {
		AccelY =  1;
	}

	if(SpeedY < 2.0f && SpeedY > -2.0f) {
		AccelY = 0;
		SpeedY = 0;
	}
}

//------------------------------------------------------------------------------
void CPlayer::ChargeGun() {
	if(static_cast<unsigned int>(ChargeStart + 100) < SDL_GetTicks()) {
		if( ChargeLevel != 10 ) {
			ChargeLevel++;
		}
		ChargeStart = SDL_GetTicks();
	}
}

//------------------------------------------------------------------------------
void CPlayer::Shoot() {
	switch( GunType ) {
		case GUN_NORMAL:
			// Can't shoot too fast
			if( LastShot + PLAYER_SHOOT_DELAY < SDL_GetTicks() ) {
				int CurrentBulletType = 0;
				if( ChargeLevel < 10 ) {
					CurrentBulletType = ENTITY_SUBTYPE_BULLET_NORMAL;
					if( GunLevel != 0 ) {
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
			CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_BEAM, static_cast<int>(X) + PLAYER_SPRITE_WIDTH + 5, static_cast<int>(Y) + PLAYER_SPRITE_HEIGHT / 2);
			break;
		default:
			break;
	}
}

//=============================================================================
bool CPlayer::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void CPlayer::OnLoop() {

	// If left or right button is not pressed anymore, stop player movement
	if(MoveLeft == false && MoveRight == false) {
		StopMoveX();
	}

	// If up or down button is not pressed anymore, stop player movement
	if(MoveUp == false && MoveDown == false) {
		StopMoveY();
	}

	// Variables that determine if player has reached left or right end
	// of the screen and thus isn't allowed to move farther to same direction
	bool leftScreenCrossed = false;
	bool rightScreenCrossed = false;
	
	// If player is at the left end of the screen. Camera is on the screen 
	// locally at point 0,0 so comparing to player's location is easy
	if (X < CCamera::CameraControl.GetX()) {
		leftScreenCrossed = true;
	} 
	// Same thing here with the right end of the screen with little adjustment
	else if ((X+PLAYER_SPRITE_WIDTH) > (CCamera::CameraControl.GetX()+WWIDTH)) {
		rightScreenCrossed = true;
	}
		
	// Player is not at the left end and can move to left freely
	if(MoveLeft && !leftScreenCrossed) {
		AccelX = PLAYER_ACCEL_LEFT;
	} 
	// Player is not at the right end and can move to right freely
	else if(MoveRight && !rightScreenCrossed) {
		AccelX = PLAYER_ACCEL_RIGHT;
	}

	// Bottom and top sides of the screen are blocked with blocking tiles
	// so there is no need to check if player has vertically crossed the line
	if(MoveUp) {
		AccelY = PLAYER_ACCEL_UP;
	} else if(MoveDown) {
		AccelY = PLAYER_ACCEL_DOWN;
	}

	// Automatic speed adjusting calculations
	SpeedX += AccelX * CFPS::FPSControl.GetSpeedFactor();
	SpeedY += AccelY * CFPS::FPSControl.GetSpeedFactor();

	// Don't let player's speed go over maximum definitions
	if(SpeedX > MaxSpeedX)  SpeedX =  MaxSpeedX;
	if(SpeedX < -MaxSpeedX) SpeedX = -MaxSpeedX;
	if(SpeedY > MaxSpeedY)  SpeedY =  MaxSpeedY;
	if(SpeedY < -MaxSpeedY) SpeedY = -MaxSpeedY;

	if( ChargeStart > 0 ) {
		ChargeGun();
	}

	// Do player's animations
	OnAnimate();

	/* Move player */
	
	// If player is not touching horizontal borders, move normally
	if (!leftScreenCrossed && !rightScreenCrossed) {
		OnMove(SpeedX, SpeedY);
	} 
	// If player is touching one but his speed is not too slow (left border)
	// or too fast (right border) to go off the screen, move normally
	else if (leftScreenCrossed && SpeedX >= CAMERA_SPEED || 
			 rightScreenCrossed && SpeedX <= CAMERA_SPEED) {
		OnMove(SpeedX, SpeedY);
	} 
	// Otherwise force player to move same speed as the camera (or the
	// rolling play area)
	else {
		// Might also want to check player X won't get off screen (level objects can "push" player)
		OnMove(CAMERA_SPEED, SpeedY);
	}


	// Reset level after death scene is complete
	if ( MakeDeathScene && (SDL_GetTicks() > (DeathMoment + 3000)) ) {
		MakeDeathScene = false;
		CAppStateGame::Instance.ResetLevelNow();
	}
}

//-----------------------------------------------------------------------------
void CPlayer::OnRender(SDL_Surface* Surf_Display) {
	if (!TookHit)
	CEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CPlayer::OnCleanup() {
	CEntity::OnCleanup();
}

//------------------------------------------------------------------------------
void CPlayer::OnAnimate() {
	if(SpeedX != 0) {
		Anim_Control.MaxFrames = PLAYER_MAX_FRAMES;
	}else{
		Anim_Control.MaxFrames = 0;
	}

	CEntity::OnAnimate();
}

//------------------------------------------------------------------------------

// NOTE: Let enemy entities call player's Die(), that's probably
// easiest way to keep everything in sync
void CPlayer::OnCollision(CEntity* Entity) {
	
	// Prevent multiple handlings for same collissions
	if( Dead || Entity->Dead ) return;

	switch(Entity->Type) {
		case ENTITY_TYPE_ENEMY: 
			//Die();
			break;
		case ENTITY_TYPE_ITEM: 
			// TODO: collect powers from item
			break;
		default: 
			// Unknown collision
			break;
	}
	
    return; 
}

//------------------------------------------------------------------------------
void CPlayer::OnCollision(CTile* Tile){

	if (!TookHit) {

		switch( Tile->TypeID ){
		case TILE_TYPE_BLOCK:
			//HP = -1;
			Die();
			break;
		case TILE_TYPE_BLOCK_BREAKABLE:
			//HP = -1;
			CFactory::Factory.CreateExplosion(Tile->X-8,Tile->Y-8, EXPLOSION_TILE);
			Tile->TypeID = TILE_TYPE_NONE;
			CArea::AreaControl.BrokenTiles.push_back(Tile);
			Die();
			break;
		default:
			return;
			break;
		}
	}
	return;
}

//------------------------------------------------------------------------------
void CPlayer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (!IsActive()) {
		MoveLeft = false;
		MoveRight = false;
		MoveUp = false;
		MoveDown = false;
		ChargeStart = 0;
		ChargeLevel = 0;
		return;
	}

	switch(sym) {
		// Movement
		case SDLK_LEFT: {
			MoveLeft = true;
			break;
		}
		case SDLK_RIGHT: {
			MoveRight = true;
			break;
		}
		case SDLK_UP: {
            MoveUp = true;
            break;
        }
		case SDLK_DOWN: {
            MoveDown = true;
            break;
        }

		// Start charging gun
		case SDLK_SPACE: {
			if( ChargeStart == 0 ) {
				ChargeStart = SDL_GetTicks();
			}
			if( GunType == GUN_BEAM ) {
				Shoot();
			}
		    break;
		}

		default: {
		}
	}
}

//------------------------------------------------------------------------------
void CPlayer::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (!IsActive()) {
		MoveLeft = false;
		MoveRight = false;
		MoveUp = false;
		MoveDown = false;
		ChargeStart = 0;
		ChargeLevel = 0;
		return;
	}


	switch(sym) {
		// Movement
		case SDLK_LEFT: {
			MoveLeft = false;
			break;
		}
		case SDLK_RIGHT: {
			MoveRight = false;
			break;
		}
		case SDLK_UP: {
            MoveUp = false;
            break;
        }
		case SDLK_DOWN: {
            MoveDown = false;
            break;
        }

		// Start charging gun
		case SDLK_SPACE: {
			Shoot();
			ChargeStart = 0;
			ChargeLevel = 0;
		    break;
		}

		default: {
		}
	}
}

//=============================================================================
void CPlayer::Die() {
	// There might be multiple collission events from a single hit
	if(!TookHit){
		CFactory::Factory.CreateExplosion(X, Y-200, ExplType::EXPLOSION_ENEMY);
		CFactory::Factory.CreateSlowMotion(SlowMotionLevel::LEVEL_SLOWMO_8X, 3000);
		
		DeathMoment = SDL_GetTicks();
		MakeDeathScene = true;
		HP = -1;

		// Play explosion sound of player's death
		CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "PlayerCrashingSound");

		Lives--;
		TookHit = true;
	}
}

//=============================================================================