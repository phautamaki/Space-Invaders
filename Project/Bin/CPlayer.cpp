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

	Points = 0;
	
	MaxSpeedX = PLAYER_MAX_SPEED_X;
	MaxSpeedY = PLAYER_MAX_SPEED_Y;

	Gun = CGun();
	
}

//=============================================================================
// Stop player's moving horizontally
void CPlayer::StopMoveX() {

	// If left or right isn't pressed and speed differs from CCamera::CameraControl.speed,
	// let's start to brake player's speed until...
	if(SpeedX > CCamera::CameraControl.speed) {
		AccelX = -1;
	}
	else if(SpeedX < CCamera::CameraControl.speed) {
		AccelX =  1;
	}

	// ... it is slow enough to set to same as CCamera::CameraControl.speed
	if(SpeedX < (CCamera::CameraControl.speed+1) && SpeedX > (-CCamera::CameraControl.speed-1)) {
		AccelX = 0;
		SpeedX = CCamera::CameraControl.speed;
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

//=============================================================================
bool CPlayer::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }
	Afterburner.OnLoad(PATH_IMAGES PATH_SPECIALEFFECTS "afterburner.png",23,17,3);

	Gun.OnLoad();

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

	// Do player's animations
	OnAnimate();

	/* Move player */
	
	// If player is not touching horizontal borders, move normally
	if (!leftScreenCrossed && !rightScreenCrossed) {
		OnMove(SpeedX, SpeedY);
	} 
	// If player is touching one but his speed is not too slow (left border)
	// or too fast (right border) to go off the screen, move normally
	else if (leftScreenCrossed && SpeedX >= CCamera::CameraControl.speed || 
			 rightScreenCrossed && SpeedX <= CCamera::CameraControl.speed) {
		OnMove(SpeedX, SpeedY);
	} 
	// Otherwise force player to move same speed as the camera (or the
	// rolling play area)
	else {
		// Might also want to check player X won't get off screen (level objects can "push" player)
		OnMove(CCamera::CameraControl.speed, SpeedY);
	}

	/* After has been moved */
	Gun.OnLoop();
	Afterburner.OnLoop();
	Afterburner.X = X-20;
	Afterburner.Y = Y+PLAYER_SPRITE_HEIGHT/2-7;

	// Reset level after death scene is complete
	if ( MakeDeathScene && (SDL_GetTicks() > (unsigned int)(DeathMoment + 3000)) ) {
		MakeDeathScene = false;
		CAppStateGame::Instance.ResetLevelNow();
	}

}

//-----------------------------------------------------------------------------
void CPlayer::OnRender(SDL_Surface* Surf_Display) {
	if (!TookHit) {
		if( SpeedX > 0 ) {
			Afterburner.OnRender(Surf_Display);
		}
		CEntity::OnRender(Surf_Display);
		Gun.OnRender(Surf_Display);
	}
}

//------------------------------------------------------------------------------
void CPlayer::OnCleanup() {
	Afterburner.OnCleanup();
	Gun.OnCleanup();
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
	if( IsDead() || Entity->IsDead()) return;

	switch(Entity->Type) {
		case ENTITY_TYPE_ENEMY: 
			//Die();
			break;
		case ENTITY_TYPE_ITEM: 
			if (!TookHit) {
				Points = Points + 100;
				if( Entity->SubType == ENTITY_SUBTYPE_ITEM_WPN_NORMAL ) {
					Gun.ChangeType(GUN_NORMAL);
				}
				else if( Entity->SubType == ENTITY_SUBTYPE_ITEM_WPN_BEAM ) {
					Gun.ChangeType(GUN_BEAM);
				}
				else if( Entity->SubType == ENTITY_SUBTYPE_ITEM_WPN_MISSILE ) {
					Gun.ChangeType(GUN_MISSILES);
				}
				else if( Entity->SubType == ENTITY_SUBTYPE_ITEM_POINTS ) {
					Points = Points+900;
				}
			}
			Entity->Die();
			break;
		default: 
			// Unknown collision
			break;
	}
	
    return; 
}

//------------------------------------------------------------------------------
void CPlayer::OnCollision(CTile* Tile){
	Die();
}

//------------------------------------------------------------------------------
void CPlayer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (!IsActive()) {
		MoveLeft = false;
		MoveRight = false;
		MoveUp = false;
		MoveDown = false;
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

		case SDLK_SPACE: {
			Gun.Activate();
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

		case SDLK_SPACE: {
			Gun.Deactivate();
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
		Gun.Reset();

		CFactory::Factory.CreateExplosion((int)X, (int)Y-200, EXPLOSION_ENEMY);
		CFactory::Factory.CreateSlowMotion(LEVEL_SLOWMO_8X, 3000);
		
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