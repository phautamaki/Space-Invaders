//=============================================================================
#include "CItem.h"

//=============================================================================
CItem::CItem() {
}

//=============================================================================
bool CItem::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }
	Type = 	ENTITY_TYPE_ITEM;

	CManouver* tmpMan = 0;
	// Wait 3s
	tmpMan = new CManouver();
	tmpMan->OnLoad(M_WAIT, 3000);
	Manouvers.push_back(tmpMan);
	// Start moving left
	tmpMan = new CManouver();
	tmpMan->OnLoad(M_START_MOVE);
	Manouvers.push_back(tmpMan);
	CurrentManouver = Manouvers.begin();
	
    return true;
}

//-----------------------------------------------------------------------------
void CItem::OnLoop() {
	CEntity::OnLoop();

	if( CurrentManouver != Manouvers.end() ) {
		if( (*CurrentManouver)->OnLoop(MoveLeft, MoveRight) ) {
			CurrentManouver++;
		}
	}
}

//-----------------------------------------------------------------------------
void CItem::OnRender(SDL_Surface* Surf_Display) {
	CEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CItem::OnCleanup() {
	CEntity::OnCleanup();

	for( unsigned int i = 0; i < Manouvers.size(); i++ ) {
		Manouvers.at(i)->OnCleanup();
		delete Manouvers.at(i);
		Manouvers.at(i) = 0;
	}
	Manouvers.clear();
}

//------------------------------------------------------------------------------
void CItem::OnAnimate() {
	if(SpeedX != 0) {
		//Anim_Control.MaxFrames = 8;
	}else{
		//Anim_Control.MaxFrames = 0;
	}

	CEntity::OnAnimate();
}

//------------------------------------------------------------------------------
bool CItem::OnCollision(CEntity* Entity) {
    return true;
}

//=============================================================================
