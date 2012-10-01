//=============================================================================
#include "CEnemyShip.h"

//=============================================================================
CEnemyShip::CEnemyShip() {
}

//=============================================================================
bool CEnemyShip::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEnemy::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void CEnemyShip::OnLoop() {
	CEnemy::OnLoop();
	
	MoveLeft = true;
}

//-----------------------------------------------------------------------------
void CEnemyShip::OnRender(SDL_Surface* Surf_Display) {
	CEnemy::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CEnemyShip::OnCleanup() {
	CEnemy::OnCleanup();
}

//=============================================================================
