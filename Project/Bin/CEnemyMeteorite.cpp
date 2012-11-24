//=============================================================================
#include "CEnemyBoss.h"
#include "CAppStateGame.h"
#include "CFactory.h"
//=============================================================================

CEnemyMeteorite::CEnemyMeteorite() {
}

bool CEnemyMeteorite::OnLoad(char* File, int Width, int Height, int MaxFrames) {
	if(CEnemy::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

	return true;
}

void CEnemyMeteorite::OnLoop() {
	X += static_cast<float>(-2*CFPS::FPSControl.GetSpeedFactor());
	Y += static_cast<float>(5*CFPS::FPSControl.GetSpeedFactor());

	OnAnimate();
}

//-----------------------------------------------------------------------------
void CEnemyMeteorite::OnRender(SDL_Surface* Surf_Display) {
	CEnemy::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CEnemyMeteorite::OnCleanup() {
	CEnemy::OnCleanup();
}

void CEnemyMeteorite::OnCollision(CEntity* Entity) {
	if( !IsActive() || !Entity->IsActive() ) return;

	switch( Entity->Type ) {
		case ENTITY_TYPE_PLAYER:
			Entity->Damage(1);
		case ENTITY_TYPE_ENEMY:
			Entity->Damage(10000);
		default:
			return;
	}

	return;
}