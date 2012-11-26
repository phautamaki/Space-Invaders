//=============================================================================
#ifndef _CENEMYSHIPSHOOTING_H_
    #define _CENEMYSHIPSHOOTING_H_

#include "CEnemy.h"
#include "CGun.h"


//=============================================================================
class CEnemyShipShooting : public CEnemy {
    public:
        CEnemyShipShooting();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

	private:
		int TimeStart;
		int Interval;

		bool Broken;

		CGun	Gun;
};

//=============================================================================

#endif