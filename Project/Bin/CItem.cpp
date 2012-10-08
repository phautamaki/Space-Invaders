//=============================================================================
#include "CItem.h"
#include <cmath>

#include <cstdlib>
#include <iostream>

//=============================================================================
CItem::CItem() {
}

//=============================================================================
bool CItem::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }
	Type = 	ENTITY_TYPE_ITEM;

	CManouver* tmpMan = 0;

	Angle = 180;
	
	// Start moving left
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_LEFT);
	Manouvers.push_back(tmpMan);
	// Wait 1,6s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 1600);
	Manouvers.push_back(tmpMan);
	// Turn down
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, 90);
	Manouvers.push_back(tmpMan);
	// Wait 0,5s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 500);
	Manouvers.push_back(tmpMan);
	// Turn left
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, 180);
	Manouvers.push_back(tmpMan);
	// Wait 0,2s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 200);
	Manouvers.push_back(tmpMan);
	// Turn up
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, -90);
	Manouvers.push_back(tmpMan);
	// Wait 0,2s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 150);
	Manouvers.push_back(tmpMan);
	// Turn right
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, 0);
	Manouvers.push_back(tmpMan);
	// Wait 2s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 2000);
	Manouvers.push_back(tmpMan);

	CurrentManouver = Manouvers.begin();
	
    return true;
}

//-----------------------------------------------------------------------------
void CItem::OnLoop() {
	//CEntity::OnLoop();
	if( Dead ) return;

	if( X < CCamera::CameraControl.GetX() - ENTITY_KILLDISTANCE ) {
		Dead = true;
	}

	if( TargetAngle == 999 ) {
		//We're not Moving
		if(MoveLeft == false && MoveRight == false && MoveUp == false && MoveDown == false) {
			StopMove();
		}

		if (MoveLeft) {
			AccelX = -0.5;
		}
		else if (MoveRight) {
			AccelX = 0.5;
		}

		if (MoveUp) {
			AccelY = -0.5;
		}
		else if (MoveDown) {
			AccelY = 0.5;
		}

		if(Flags & ENTITY_FLAG_GRAVITY) {
			AccelY = 0.75f;
		}
	}
	else{
		// Need to turn the moving direction of object to TargetAngle
		if( Angle >= 0 && Angle < 90 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
		}
		else if( Angle >= 90 && Angle <= 180 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
		}
		else if( Angle < 0 && Angle >= -90 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
		}
		else if( Angle < -90 && Angle >= -180 ) {
			if( Angle > TargetAngle ) {
				AccelX = static_cast<float>(AccelX - 0.05);
				AccelY = static_cast<float>(AccelY + 0.05);
			}
			else {
				AccelX = static_cast<float>(AccelX + 0.05);
				AccelY = static_cast<float>(AccelY - 0.05);
			}
		}
	}

	SpeedX += AccelX * CFPS::FPSControl.GetSpeedFactor();
	SpeedY += AccelY * CFPS::FPSControl.GetSpeedFactor();

	if(SpeedX > MaxSpeedX)  SpeedX =  MaxSpeedX;
	if(SpeedX < -MaxSpeedX) SpeedX = -MaxSpeedX;
	if(SpeedY > MaxSpeedY)  SpeedY =  MaxSpeedY;
	if(SpeedY < -MaxSpeedY) SpeedY = -MaxSpeedY;

	if( CurrentManouver != Manouvers.end() ) {
		if( (*CurrentManouver)->OnLoop() ) {
			CurrentManouver++;
		}
	}
	else {
		CurrentManouver = Manouvers.begin();
	}



	CEntity::OnAnimate();
	this->OnMove(SpeedX, SpeedY);
}

//-----------------------------------------------------------------------------
void CItem::OnRender(SDL_Surface* Surf_Display) {
	CEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CItem::OnCleanup() {
	CEntity::OnCleanup();

	for( unsigned int i = 0; i < Manouvers.size(); i++ ) {
		Manouvers.at(i)->OnCleanup();
		delete Manouvers.at(i);
		Manouvers.at(i) = 0;
	}
	Manouvers.clear();
}

//------------------------------------------------------------------------------
void CItem::OnAnimate() {
	if(SpeedX != 0) {
		//Anim_Control.MaxFrames = 8;
	}else{
		//Anim_Control.MaxFrames = 0;
	}

	CEntity::OnAnimate();
}

//------------------------------------------------------------------------------
bool CItem::OnCollision(CEntity* Entity) {
    return true;
}

//=============================================================================
void CItem::OnMove(float MoveX, float MoveY) {
	if(MoveX == 0 && MoveY == 0) return;

	float orX = MoveX;
	float orY = MoveY;

	int oldAngle = Angle;	
	Angle = static_cast<int>(atan2(MoveY, MoveX) * 180.0 / 3.141592);

	if( oldAngle == -180 && Angle == 180 ){
		Angle = -180;
	}

	CEntity::OnMove(orX,orY);
}