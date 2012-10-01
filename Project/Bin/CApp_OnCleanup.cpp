//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnCleanup() {
	debug("Clean up start");

	CAppStateManager::SetActiveAppState(APPSTATE_NONE);

    SDL_FreeSurface(Surf_Display);
	debug("Screen cleared",1);
	
	CFactory::Factory.OnCleanup();
	debug("All entities cleared",1);

	CFont::FontControl.OnCleanup();
	debug("All fonts cleared",1);

	CSoundBank::SoundControl.OnCleanup();
	debug("All sounds cleared",1);
 
	TTF_Quit();
	debug("SDL TTF closed",1);
	Mix_CloseAudio();
	debug("SDL mixer closed",1);
    SDL_Quit();
}

//==============================================================================
