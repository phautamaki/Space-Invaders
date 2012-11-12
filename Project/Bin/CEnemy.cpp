//=============================================================================
#include "CEnemy.h"
#include "CFactory.h"

//=============================================================================
CEnemy::CEnemy() {
	HP = 1;
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

//-----------------------------------------------------------------------------
void CEnemy::OnCollision(CEntity* Entity) {
	if( !IsActive() || !Entity->IsActive() ) return;

	switch( Entity->Type ) {
		case ENTITY_TYPE_BULLET:
			HP--;
			if( HP <= 0 ){
				// Need to substract bullet life, since it won't check collission when enemy dies from hit
				Entity->HP--;
				CFactory::Factory.CreateExplosion(X-130,Y-200, EXPLOSION_ENEMY);
			}
			break;
		case ENTITY_TYPE_PLAYER:
			// Always die when colliding with player
			CFactory::Factory.CreateExplosion(X-130,Y-200, EXPLOSION_ENEMY);
			Entity->Die();
			HP = -1;
			Die();
		default:
			return;
	}

	return;
}

//=============================================================================
