//=============================================================================
#include "CEnemy.h"

//=============================================================================
CEnemy::CEnemy() {
}

//=============================================================================
bool CEnemy::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }
	Type = 	ENTITY_TYPE_ENEMY;
	// Assume all enemies initally move left
	Angle = 180;
	
    return true;
}

//-----------------------------------------------------------------------------
void CEnemy::OnLoop() {
	if( Dead ) return;

	CEntity::OnLoop();

	if( CurrentManouver != Manouvers.end() ) {
		if( (*CurrentManouver)->OnLoop() ) {
			CurrentManouver++;
		}
	}
	else {
		CurrentManouver = Manouvers.begin();
	}
}

//-----------------------------------------------------------------------------
void CEnemy::OnRender(SDL_Surface* Surf_Display) {
	CEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CEnemy::OnCleanup() {
	CEntity::OnCleanup();

	for( unsigned int i = 0; i < Manouvers.size(); i++ ) {
		Manouvers.at(i)->OnCleanup();
		delete Manouvers.at(i);
		Manouvers.at(i) = 0;
	}
	Manouvers.clear();
}

//------------------------------------------------------------------------------
void CEnemy::OnAnimate() {
	if(SpeedX != 0) {
		//Anim_Control.MaxFrames = 8;
	}else{
		//Anim_Control.MaxFrames = 0;
	}

	CEntity::OnAnimate();
}

//------------------------------------------------------------------------------
bool CEnemy::OnCollision(CEntity* Entity) {
    return true;
}

//=============================================================================
