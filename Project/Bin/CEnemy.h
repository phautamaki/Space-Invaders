//=============================================================================
#ifndef _CENEMY_H_
    #define _CENEMY_H_

#include "CManouvarableEntity.h"

//=============================================================================
class CEnemy : public CManouvarableEntity {
    public:
        CEnemy();

		virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        virtual void OnLoop();

        virtual void OnCollision(CEntity* Entity);

		virtual void Die();

		virtual bool IsDead();
};

//=============================================================================

#endif