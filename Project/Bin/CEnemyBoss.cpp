//=============================================================================
#include "CEnemyBoss.h"
#include "CAppStateGame.h"
#include "CFactory.h"
#include "CSoundBank.h"
//=============================================================================

CEnemyBoss::CEnemyBoss(int level) {
	SpeedY = 2;

	lastBubble = 0;
	MakeBossDeathScene = false;
	DeathMoment = 0;
	bossDead = false;
}

bool CEnemyBoss::OnLoad(char* File, int Width, int Height, int MaxFrames) {
	if(CEnemy::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

	return true;
}

void CEnemyBoss::OnLoop() {

	if (Y > 250+GUI_HEIGHT) SpeedY = -2;
	else if (Y < 50+GUI_HEIGHT) SpeedY = 2;

	// Make a new little bubble enemy every second
	if ( CAppStateGame::Instance.Player->IsActive() &&
		CAppStateGame::Instance.BossFightOn &&
		!MakeBossDeathScene &&
		(SDL_GetTicks()-lastBubble > 1000) ) {

		CFactory::Factory.CreateEnemy(ENTITY_SUBTYPE_ENEMY_BOSS_1_LITTLE_BUBBLES, (int)X-64, (int)Y+80);
		lastBubble = SDL_GetTicks();
	}

	CEnemy::OnLoop();

	// Change level after death scene is complete
	if ( MakeBossDeathScene && (SDL_GetTicks() > (unsigned int)(DeathMoment + 3000)) ) {
		MakeBossDeathScene = false;
		CAppStateGame::Instance.BossDead = true;
		bossDead = true;
	}
}

//-----------------------------------------------------------------------------
void CEnemyBoss::OnRender(SDL_Surface* Surf_Display) {
	if (!MakeBossDeathScene) {
		CEnemy::OnRender(Surf_Display);
	}
}

//------------------------------------------------------------------------------
void CEnemyBoss::OnCleanup() {
	CEnemy::OnCleanup();
}

void CEnemyBoss::OnCollision(CEntity* Entity) {

	// Prevent premature damage taking
	if( !CAppStateGame::Instance.BossFightOn ) return;

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
			Entity->Damage(1);
		default:
			return;
	}

	return;
}

void CEnemyBoss::Die() {
	CFactory::Factory.CreateSlowMotion(LEVEL_SLOWMO_4X, 3000);
	MakeBossDeathScene = true;
	DeathMoment = SDL_GetTicks();

	CFactory::Factory.CreateExplosion((int)X,(int)Y-60, EXPLOSION_ENEMY_BOSS_1);
	CFactory::Factory.CreateExplosion((int)X-40,(int)Y-120, EXPLOSION_ENEMY_BOSS_1);
	CFactory::Factory.CreateExplosion((int)X+30,(int)Y-170, EXPLOSION_ENEMY_BOSS_1);
	CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "BossExplodingSound");
	CFactory::Factory.GetPlayer()->Points = CFactory::Factory.GetPlayer()->Points + 5000;
}

bool CEnemyBoss::IsDead() {
	return bossDead;
}