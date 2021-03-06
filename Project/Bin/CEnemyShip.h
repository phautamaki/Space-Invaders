//=============================================================================
#ifndef _CENEMYSHIP_H_
    #define _CENEMYSHIP_H_

#include "CEnemy.h"

//=============================================================================
class CEnemyShip : public CEnemy {
    public:
        CEnemyShip();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();
};

//=============================================================================

#endif