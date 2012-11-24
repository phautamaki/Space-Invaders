//=============================================================================
#ifndef _CENEMYMETEORITE_H_
    #define _CENEMYMETEORITE_H_

#include "CEnemy.h"

//=============================================================================
class CEnemyMeteorite : public CEnemy {
    public:
		CEnemyMeteorite();

		virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        virtual void OnLoop();

		virtual void OnRender(SDL_Surface* Surf_Display);
		
		virtual void OnCleanup();

		virtual void OnCollision(CEntity* Entity);
};

//=============================================================================

#endif