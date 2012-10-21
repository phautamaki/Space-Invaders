//=============================================================================
#include "CManouvarableEntity.h"

//=============================================================================
CManouvarableEntity::CManouvarableEntity() {
}

//=============================================================================
bool CManouvarableEntity::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void CManouvarableEntity::OnLoop() {
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
void CManouvarableEntity::OnRender(SDL_Surface* Surf_Display) {
	CEntity::OnRender(Surf_Display);
}

//------------------------------------------------------------------------------
void CManouvarableEntity::OnCleanup() {
	CEntity::OnCleanup();

	for( unsigned int i = 0; i < Manouvers.size(); i++ ) {
		Manouvers.at(i)->OnCleanup();
		delete Manouvers.at(i);
		Manouvers.at(i) = 0;
	}
	Manouvers.clear();
}

//=============================================================================