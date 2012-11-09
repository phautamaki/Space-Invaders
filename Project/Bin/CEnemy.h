//=============================================================================
#ifndef _CENEMY_H_
    #define _CENEMY_H_

#include "CManouvarableEntity.h"

//=============================================================================
class CEnemy : public CManouvarableEntity {
    public:
        CEnemy();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnCollision(CEntity* Entity);
};

//=============================================================================

#endif