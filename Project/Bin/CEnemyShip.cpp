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

	CManouver* tmpMan = 0;
	
	// Start moving left
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_UP);
	Manouvers.push_back(tmpMan);
	// Wait 0,1s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 200);
	Manouvers.push_back(tmpMan);
	// Wait 0,1s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, -180);
	Manouvers.push_back(tmpMan);
	// Wait 10s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 10000);
	Manouvers.push_back(tmpMan);

	CurrentManouver = Manouvers.begin();

    return true;
}

//-----------------------------------------------------------------------------
void CEnemyShip::OnLoop() {
	CEnemy::OnLoop();
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
