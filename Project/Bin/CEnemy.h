//=============================================================================
#ifndef _CENEMY_H_
    #define _CENEMY_H_

#include <vector>

#include "CEntity.h"
#include "CManouver.h"

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

	protected:
		std::vector<CManouver*>::iterator CurrentManouver;
		std::vector<CManouver*> Manouvers;
};

//=============================================================================

#endif