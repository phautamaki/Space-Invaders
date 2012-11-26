//=============================================================================
#ifndef _CENEMYCANNON_H_
    #define _CENEMYCANNON_H_

#include "CEnemy.h"

//=============================================================================
class CEnemyCannon : public CEnemy {
    public:
		CEnemyCannon();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

		void OnRender(SDL_Surface* Surf_Display);
		
		void OnCleanup();

		void OnCollision(CEntity* Entity);

		void Die();

//------------------------------------------------------------------------------
	private:
		int TimeStart;
		int Interval;

		bool Broken;
};

//=============================================================================

#endif