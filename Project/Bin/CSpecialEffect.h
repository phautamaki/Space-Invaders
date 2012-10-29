//=============================================================================
#ifndef _CSPECIALEFFECT_H_
    #define _CSPECIALEFFECT_H_

#include "CEntity.h"

//=============================================================================
class CSpecialEffect : public CEntity {
    public:
        CSpecialEffect();

        void OnLoop();
};

//=============================================================================

#endif