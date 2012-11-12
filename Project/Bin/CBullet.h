//=============================================================================
#ifndef _CBULLET_H_
    #define _CBULLET_H_

#include "CManouvarableEntity.h"

//=============================================================================
class CBullet : public CManouvarableEntity {
    public:
        CBullet();

		bool OnLoad(int nType);
		void OnRender(SDL_Surface* Surf_Display);
		void OnCollision(CEntity* Entity);
		void OnCollision(CTile* Tile);

	private:
		unsigned int DamageOutput;
};

//=============================================================================

#endif