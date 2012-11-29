//=============================================================================
#include "CItem.h"
//=============================================================================
CItem::CItem() {
	MaxSpeedX = 3;
}

//=============================================================================
bool CItem::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CManouvarableEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }
	Type = 	ENTITY_TYPE_ITEM;
	// Assume all items initally move left
	
	CManouver* tmpMan = 0;
	
	
	// Start moving left
	Angle = 180;

	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_LEFT);
	Manouvers.push_back(tmpMan);
	
	CurrentManouver = Manouvers.begin();
    return true;
}

//-----------------------------------------------------------------------------
void CItem::OnLoop() {
	if( Dead ) return;

	CManouvarableEntity::OnLoop();
}

//-----------------------------------------------------------------------------
void CItem::OnRender(SDL_Surface* Surf_Display) {

	CManouvarableEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CItem::OnCleanup() {
	CManouvarableEntity::OnCleanup();
}

//------------------------------------------------------------------------------
void CItem::OnAnimate() {
	CManouvarableEntity::OnAnimate();
}

//------------------------------------------------------------------------------
void CItem::OnCollision(CEntity* Entity) {
	// Prevent multiple handlings for same collissions
	if( IsDead() || Entity->IsDead() ) return;

	switch(Entity->Type) {
		case ENTITY_TYPE_PLAYER: 
			break;
		case ENTITY_TYPE_BULLET: 
			break;
		default: 
			// Unknown collision
			break;
	}
	
    return; 
}

//=============================================================================