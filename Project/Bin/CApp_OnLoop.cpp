//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnLoop() {
	CAppStateManager::OnLoop();

    CFPS::FPSControl.OnLoop();

	// Call this last
	CFactory::Factory.OnLoop();

    char Buffer[255];
    sprintf(Buffer, "FPS: %d", CFPS::FPSControl.GetFPS());
    SDL_WM_SetCaption(Buffer, Buffer);
}

//==============================================================================
