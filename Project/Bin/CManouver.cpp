#include "CManouver.h"
#include "sdl.h"
#include "functions.h"
#include <cmath>

#include <iostream>
#include <cstdlib>

//=============================================================================
CManouver::CManouver(CEntity* parent) {
	Parent = parent;

	StartTime = 0;
	Wait = 0;
}

//=============================================================================
bool CManouver::OnLoad(int type) {
	Type = type;

	return true;
}

//------------------------------------------------------------------------------
bool CManouver::OnLoad(int type, int time) {
	Type = type;
	Wait = time; // Also used as an angle

	return true;
}

//------------------------------------------------------------------------------
bool CManouver::OnLoop() {
	if( Type == M_WAIT ) {
		if( StartTime == 0 ) {
			StartTime = SDL_GetTicks();
		}
		if(static_cast<unsigned int>(StartTime + Wait) < SDL_GetTicks()) {
			StartTime = 0;
			return true;
		}
	}
	else if( Type == M_MOVE_LEFT ) {
		Parent->MoveLeft  = true;
		Parent->MoveRight = false;
		return true;
	}
	else if( Type == M_MOVE_RIGHT ) {
		Parent->MoveLeft  = false;
		Parent->MoveRight = true;
		return true;
	}
	else if( Type == M_MOVE_UP ) {
		Parent->MoveUp	 = true;
		Parent->MoveDown = false;
		return true;
	}
	else if( Type == M_MOVE_DOWN ) {
		Parent->MoveUp	 = false;
		Parent->MoveDown = true;
		return true;
	}
	else if( Type == M_STOP_MOVE ) {
		Parent->MoveLeft	= false;
		Parent->MoveRight	= false;
		Parent->MoveUp		= false;
		Parent->MoveDown	= false;
		return true;
	}
	else if( Type == M_TURN ) {
		if( Parent->TargetAngle == 999 ){
			// Take shorter path around the circle
			if( Parent->Angle == 180 && Wait < 0 ) {
				Parent->Angle = -180;
			}
			else if( Parent->Angle == -180 && Wait >= 0 ) {
				Parent->Angle = 180;
			}
			// Useful to know from which direction we are coming to target
			OriginalAngle = Parent->Angle;

			// This is where we aim at
			Parent->TargetAngle = Wait;
		}

		// It's hard to hit the target angle precicely, so in case we go over a bit, count it a hit
		if( (OriginalAngle <= Parent->TargetAngle && Parent->TargetAngle <= Parent->Angle ) || 
			(OriginalAngle >= Parent->TargetAngle && Parent->TargetAngle >= Parent->Angle ) ||
			(Parent->TargetAngle == 180 && Parent->Angle < -175 ) || 
			(Parent->TargetAngle == -180 && Parent->Angle > 175 ) ) {

			if( Parent->TargetAngle == 0 ){
				Parent->MoveLeft	= false;
				Parent->MoveRight	= true;
				Parent->MoveUp		= false;
				Parent->MoveDown	= false;

				Parent->SpeedY = Parent->AccelY = 0;
			}
			else if( Parent->TargetAngle == 90 ) {
				Parent->MoveLeft	= false;
				Parent->MoveRight	= false;
				Parent->MoveUp		= false;
				Parent->MoveDown	= true;

				Parent->SpeedX = Parent->AccelX = 0;
			}
			else if( Parent->TargetAngle == -90 ) {
				Parent->MoveLeft	= false;
				Parent->MoveRight	= false;
				Parent->MoveUp		= true;
				Parent->MoveDown	= false;

				Parent->SpeedX = Parent->AccelX = 0;
			}
			else if( Parent->TargetAngle == 180 || Parent->TargetAngle == -180 ) {
				Parent->MoveLeft	= true;
				Parent->MoveRight	= false;
				Parent->MoveUp		= false;
				Parent->MoveDown	= false;

				Parent->SpeedY = Parent->AccelY = 0;
			}
			
			Parent->Angle = Parent->TargetAngle;
			Parent->TargetAngle = 999;
			return true;
		}
	}
	else if( Type == M_AIM ) {
		if( Parent->X == TargetX && Parent->Y == TargetY ) {
			return true;
		}

		float Speed = Parent->MaxSpeedX;
		int AngleToTarget = (int)atan( static_cast<float>((TargetX - Parent->X)) / static_cast<float>((TargetY - Parent->Y)) ) * 100;

		Parent->SpeedX = cos(DegreesToRadian(AngleToTarget)) * Speed;
		Parent->SpeedY = sin(DegreesToRadian(AngleToTarget)) * Speed;
	}
	else if( Type == M_FOLLOW ) {
		if( TargetEntity == NULL || (Parent->X == TargetEntity->X && Parent->Y == TargetEntity->Y) ) {
			return true;
		}

		float Speed = Parent->MaxSpeedX;
		int AngleToTarget = (int)atan( (TargetEntity->X - Parent->X) / (TargetEntity->Y - Parent->Y) );

		Parent->SpeedX = cos(DegreesToRadian(AngleToTarget)) * Speed;
		Parent->SpeedY = sin(DegreesToRadian(AngleToTarget)) * Speed;
	}
	return false;
}

//------------------------------------------------------------------------------
void CManouver::OnCleanup() {
	// Do NOT delete Parent
	Parent = 0;
}

//=============================================================================