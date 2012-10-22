//=============================================================================
#ifndef _CBULLET_H_
    #define _CBULLET_H_

#include "CManouvarableEntity.h"

//=============================================================================
class CBullet : public CManouvarableEntity {
    public:
        CBullet();

		bool OnLoad(int nType);

		bool OnCollision(CEntity* Entity);

		bool OnCollision(CTile* Tile);
};

//=============================================================================

#endif