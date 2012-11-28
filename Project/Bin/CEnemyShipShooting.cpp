//=============================================================================
#include "CEnemyShipShooting.h"
#include "CFactory.h"

//=============================================================================
CEnemyShipShooting::CEnemyShipShooting() {
	TimeStart = 0;
	Interval  = ENEMY_CANNON_INTERVAL;
	Broken    = false;

	Gun = CGun();
}

//=============================================================================
bool CEnemyShipShooting::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEnemy::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

	CManouver* tmpMan = 0;
	
	// Start moving left
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_LEFT);
	Manouvers.push_back(tmpMan);
	// Wait 10s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 10000);
	Manouvers.push_back(tmpMan);

	CurrentManouver = Manouvers.begin();

	Gun.OnLoad(true);

    return true;
}

//-----------------------------------------------------------------------------
void CEnemyShipShooting::OnLoop() {
	CEnemy::OnLoop();
	
	if(static_cast<unsigned int>(TimeStart + 2000) < SDL_GetTicks()) {
		Gun.Deactivate((int)X, (int)Y);
		TimeStart = SDL_GetTicks();
	}

	Gun.OnLoop();
}

//-----------------------------------------------------------------------------
void CEnemyShipShooting::OnRender(SDL_Surface* Surf_Display) {
	CEnemy::OnRender(Surf_Display);
	Gun.OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CEnemyShipShooting::OnCleanup() {
	CEnemy::OnCleanup();
	Gun.OnCleanup();
}

//=============================================================================
