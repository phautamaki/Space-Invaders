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
	
    return true;
}

//-----------------------------------------------------------------------------
void CEnemy::OnLoop() {
	CEntity::OnLoop();
}

//-----------------------------------------------------------------------------
void CEnemy::OnRender(SDL_Surface* Surf_Display) {
	CEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CEnemy::OnCleanup() {
	CEntity::OnCleanup();
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
