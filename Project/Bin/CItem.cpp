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
	// Assume all items initally move left
	Angle = 180;

	CManouver* tmpMan = 0;
	
	// Start moving left
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_LEFT);
	Manouvers.push_back(tmpMan);
	// Wait 1,6s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 1600);
	Manouvers.push_back(tmpMan);
	// Turn down
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, 90);
	Manouvers.push_back(tmpMan);
	// Wait 0,5s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 500);
	Manouvers.push_back(tmpMan);
	// Turn left
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, 180);
	Manouvers.push_back(tmpMan);
	// Wait 0,2s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 200);
	Manouvers.push_back(tmpMan);
	// Turn up
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, -90);
	Manouvers.push_back(tmpMan);
	// Wait 0,2s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 150);
	Manouvers.push_back(tmpMan);
	// Turn right
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_TURN, 0);
	Manouvers.push_back(tmpMan);
	// Wait 2s
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_WAIT, 2000);
	Manouvers.push_back(tmpMan);

	CurrentManouver = Manouvers.begin();
	
    return true;
}

//-----------------------------------------------------------------------------
void CItem::OnLoop() {
	if( Dead ) return;

	CEntity::OnLoop();

	if( CurrentManouver != Manouvers.end() ) {
		if( (*CurrentManouver)->OnLoop() ) {
			CurrentManouver++;
		}
	}
	else {
		CurrentManouver = Manouvers.begin();
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