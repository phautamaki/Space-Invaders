#include "CManouver.h"
#include "sdl.h"

//=============================================================================
CManouver::CManouver() {
	StartTime = SDL_GetTicks();
	Wait = 0;
}

//=============================================================================
bool CManouver::OnLoad(int type) {
	Type = type;

	return true;
}

//------------------------------------------------------------------------------
bool CManouver::OnLoad(int type, int time) {
	Type = type;
	Wait = time;

	return true;
}

//------------------------------------------------------------------------------
bool CManouver::OnLoop(bool& moveleft, bool& moveright) {
	if( Type == M_WAIT ) {
		if(static_cast<unsigned int>(StartTime + Wait) < SDL_GetTicks()) {
			return true;
		}
	}
	if( Type == M_START_MOVE ) {
		moveleft = true;
	}
	return false;
}

//------------------------------------------------------------------------------
void CManouver::OnCleanup() {
}

//=============================================================================