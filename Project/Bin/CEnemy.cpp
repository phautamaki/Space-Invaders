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
			if( Entity->SubType == ENTITY_SUBTYPE_BULLET_NORMAL ) {
				Damage(BULLET_NORMAL_STR);
			}
			else if( Entity->SubType == ENTITY_SUBTYPE_BULLET_SMALL_45U || Entity->SubType == ENTITY_SUBTYPE_BULLET_SMALL_45D) {
				Damage(BULLET_SMALL_STR);
			}
			else if( Entity->SubType == ENTITY_SUBTYPE_BULLET_CHARGE1 ) {
				Damage(BULLET_CHARGE1_STR);
			}

			if( IsDead() ){
				// Need to substract bullet life, since it won't check collission when enemy dies from hit
				Entity->Damage(1);
			}
			break;
		case ENTITY_TYPE_PLAYER:
			// Always die when colliding with player
			CFactory::Factory.CreateExplosion((int)X-130,(int)Y-200, EXPLOSION_ENEMY);
			Entity->Damage(1);
			Die();
		default:
			return;
	}

	return;
}

//=============================================================================
void CEnemy::Die() {
	CManouvarableEntity::Die();
	CFactory::Factory.CreateExplosion((int)X-130,(int)Y-200, EXPLOSION_ENEMY);
	CFactory::Factory.CreateRandomItem((int)X,(int)Y+(Height/2));
	CFactory::Factory.GetPlayer()->Points = CFactory::Factory.GetPlayer()->Points + 500;
}

//=============================================================================
