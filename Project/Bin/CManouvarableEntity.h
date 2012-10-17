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

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

	protected:
		std::vector<CManouver*>::iterator CurrentManouver;
		std::vector<CManouver*> Manouvers;
};

//=============================================================================

#endif