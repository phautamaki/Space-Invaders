//=============================================================================
#ifndef _CBULLET_H_
    #define _CBULLET_H_

#include "CManouvarableEntity.h"

//=============================================================================
class CBullet : public CManouvarableEntity {
    public:
        CBullet();

		bool OnLoad(int nType);

		void OnCollision(CEntity* Entity);

		void OnCollision(CTile* Tile);
};

//=============================================================================

#endif