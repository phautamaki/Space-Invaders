//==============================================================================
#include "CApp.h"

//==============================================================================
bool CApp::OnInit() {
    debug("Initialization start");
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
	debug("SDL core initialized", 1);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		return false;
	}

	debug("SDL mixer initialized", 1);
	if (TTF_Init() < 0) {
		return false;
	}
	debug("SDL TTF initialized", 1);

	if( FULLSCREEN_MODE ) {
		if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
			return false;
		}
	}
	else {
		if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
			return false;
		}
    }
	
	debug("Screen created", 1);

	SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

	if(!CFont::FontControl.OnLoad(PATH_FONTS "FairyDustB.ttf", 50 )){
		error("Coudln't initialize FontControl.");
	}
	if(!CFont::FontControl.OnLoad(PATH_FONTS "DigitalDream.ttf", 25 )){
		error("Coudln't initialize FontControl.");
	}
	if(!CFactory::Factory.OnInit()){
		error("Coudln't initialize Factor.");
	}

	CAppStateManager::SetActiveAppState(APPSTATE_INTRO);
	
	debug("Initialization successful");

    return true;
}

//==============================================================================
