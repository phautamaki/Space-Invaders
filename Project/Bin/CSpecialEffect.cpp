//=============================================================================
#include "CSpecialEffect.h"

//=============================================================================
CSpecialEffect::CSpecialEffect() {
	Type = ENTITY_TYPE_SPECIAL_EFFECT;
	Flags = ENTITY_FLAG_GHOST;
}

//=============================================================================
void CSpecialEffect::OnLoop() {
	if(!Anim_Control.KeepAnimating) {
		Die();
	}

	CEntity::OnLoop();
}

//=============================================================================
