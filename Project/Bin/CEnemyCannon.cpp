//=============================================================================
#include "CEnemyCannon.h"
#include "CFactory.h"
#include "CSoundBank.h"
//=============================================================================

CEnemyCannon::CEnemyCannon() {
	TimeStart = 0;
	Interval  = ENEMY_CANNON_INTERVAL;
	Broken    = false;
}

//-----------------------------------------------------------------------------
bool CEnemyCannon::OnLoad(char* File, int Width, int Height, int MaxFrames) {
	if(CEnemy::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

	TimeStart = SDL_GetTicks();

	return true;
}

//-----------------------------------------------------------------------------
void CEnemyCannon::OnLoop() {
	if( !Broken ) {
		if(static_cast<unsigned int>(TimeStart + Interval) < SDL_GetTicks()) {
			CBullet* tmp = 0;
			if( OnRoof ) {
				tmp = CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_GENERIC,(int)X-6,(int)Y+45);
			}
			else {
				tmp = CFactory::Factory.CreateBullet(ENTITY_SUBTYPE_BULLET_GENERIC,(int)X-6,(int)Y);
			}
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
}

//-----------------------------------------------------------------------------
void CEnemyCannon::OnRender(SDL_Surface* Surf_Display) {
	CEnemy::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CEnemyCannon::OnCleanup() {
	CEnemy::OnCleanup();
}

//-----------------------------------------------------------------------------
void CEnemyCannon::OnCollision(CEntity* Entity) {
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

//-----------------------------------------------------------------------------
void CEnemyCannon::Die() {
	CFactory::Factory.CreateExplosion((int)X-80,(int)Y-200, EXPLOSION_ENEMY);
	CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "EnemyExplodingSound");
	CFactory::Factory.GetPlayer()->Points = CFactory::Factory.GetPlayer()->Points + 600;

	Broken = true;
	CurrentFrameCol = 1;
}

//=============================================================================