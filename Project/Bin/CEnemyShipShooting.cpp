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
	if( !Broken ) {
		if(static_cast<unsigned int>(TimeStart + 500) < SDL_GetTicks()) {

			CBullet*	tmp = CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_ENEMY_MISSILE,(int)X - 50,(int)Y);

			if( tmp != NULL ) {
				tmp->MaxSpeedX = tmp->MaxSpeedY = 3;
				tmp->SpeedX = -3;
				tmp->SpeedY = -3;
				if( OnRoof ) {
					tmp->SpeedY = 3;
				}
				TimeStart = SDL_GetTicks();
				Anim_Control.MaxFrames = 5;
			}
		}
	}
	OnAnimate();
	if( Anim_Control.GetCurrentFrame() == 3 ) {
		Anim_Control.SetCurrentFrame(0);
		Anim_Control.MaxFrames = 1;
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
