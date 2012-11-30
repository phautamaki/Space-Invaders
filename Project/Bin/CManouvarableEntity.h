//=============================================================================
#ifndef _CMANOUVARABLEENTITY_H_
    #define _CMANOUVARABLEENTITY_H_

#include <vector>

#include "CEntity.h"
#include "CManouver.h"

//=============================================================================
class CManouvarableEntity : public CEntity {
    public:
        CManouvarableEntity();

		virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        virtual void OnLoop();

        virtual void OnRender(SDL_Surface* Surf_Display);

        virtual void OnCleanup();

		virtual bool IsDead();

		virtual void Kill();

	protected:
		std::vector<CManouver*>::iterator CurrentManouver;
		std::vector<CManouver*> Manouvers;
};

//=============================================================================

#endif