//=============================================================================
#ifndef _CITEM_H_
    #define _CITEM_H_

#include <vector>

#include "CEntity.h"
#include "CManouver.h"

//==============================================================================
// Could do some kind of sub typing with these (Item -> HP+) or sth like that
enum {
	ITEM_1 = 0
};

//=============================================================================
class CItem : public CEntity {
    public:
        CItem();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        bool OnCollision(CEntity* Entity);

		void OnMove(float MoveX, float MoveY);

	private:
		std::vector<CManouver*>::iterator CurrentManouver;
		std::vector<CManouver*> Manouvers;
};

//=============================================================================

#endif