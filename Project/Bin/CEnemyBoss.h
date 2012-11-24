//=============================================================================
#ifndef _CENEMYBOSS_H_
    #define _CENEMYBOSS_H_

#include "CEnemy.h"

//=============================================================================
class CEnemyBoss : public CEnemy {
    public:
        CEnemyBoss(int level);

		virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        virtual void OnLoop();

		virtual void OnRender(SDL_Surface* Surf_Display);

		virtual void OnCleanup();

		virtual void OnCollision(CEntity* Entity);

		virtual void Die();

		virtual bool IsDead();

private:
	int lastBubble;
	bool MakeBossDeathScene;
	int DeathMoment;

	bool bossDead;
};

//=============================================================================

#endif