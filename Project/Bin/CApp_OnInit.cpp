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

	//Loads all the sounds
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "EnemyExplodingSound", PATH_EFFECTS FILENAME_ENEMY_EXPLODING) ? debug("Shit hit the fan when loading EnemyExplodingSound.") : debug("Loading EnemyExplodingSound was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "BossExplodingSound", PATH_EFFECTS FILENAME_BOSS_EXPLODING) ? debug("Shit hit the fan when loading BossExplodingSound.") : debug("Loading BossExplodingSound was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ShootingSoundBasic", PATH_EFFECTS FILENAME_SHOOTING_BASIC) ? debug("Shit hit the fan when loading ShootingSoundBasic.") : debug("Loading ShootingSoundBasic was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ShootingSoundBig", PATH_EFFECTS FILENAME_SHOOTING_BIG) ? debug("Shit hit the fan when loading ShootingSoundBig.") : debug("Loading ShootingSoundBig was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ShootingSoundBeam", PATH_EFFECTS FILENAME_SHOOTING_BEAM) ? debug("Shit hit the fan when loading ShootingSoundBeam.") : debug("Loading ShootingSoundBeam was a great success!");	
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "PlayerCrashingSound", PATH_EFFECTS FILENAME_PLAYER_CRASHING) ? debug("Shit hit the fan when loading PlayerCrashingSound.") : debug("Loading PlayerCrashingSound was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ShootingSoundMissile", PATH_EFFECTS FILENAME_SHOOTING_MISSILE) ? debug("Shit hit the fan when loading ShootingSoundMissile.") : debug("Loading ShootingSoundMissile was a great success!");
	!CSoundBank::SoundControl.OnLoad(CSoundBank::EFFECT, "ButtonClickSound", PATH_EFFECTS FILENAME_BUTTON_CLICK) ? debug("Shit hit the fan when loading ButtonClickSound.") : debug("Loading ButtonClickSound was a great success!");
	
	debug("All sounds initialized", 1);

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
