//=============================================================================
#include "CEnemy.h"

//=============================================================================
CEnemy::CEnemy() {
}

//=============================================================================
bool CEnemy::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CManouvarableEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
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

	CManouvarableEntity::OnLoop();
}

//------------------------------------------------------------------------------
bool CEnemy::OnCollision(CEntity* Entity) {
    return true;
}

//=============================================================================
