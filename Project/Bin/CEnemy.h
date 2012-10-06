//=============================================================================
#ifndef _CENEMY_H_
    #define _CENEMY_H_

#include "CEntity.h"

//=============================================================================
class CEnemy : public CEntity {
    public:
        CEnemy();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        bool OnCollision(CEntity* Entity);
};

//=============================================================================

#endif