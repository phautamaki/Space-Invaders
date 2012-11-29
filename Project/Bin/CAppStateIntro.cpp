//=============================================================================
#include "CAppStateIntro.h"
#include "CAppStateManager.h"
#include "CSoundBank.h"

#include "Paths.h"

//=============================================================================
CAppStateIntro CAppStateIntro::Instance;

//=============================================================================
CAppStateIntro::CAppStateIntro() {
	Surf_Logo = NULL;
}

//=============================================================================
void CAppStateIntro::OnActivate() {
	NextState = APPSTATE_MAINMENU;
	IntroLength = 3000;

	CSoundBank::SoundControl.Play(CSoundBank::MUSIC, "MenuMusic");

	// Load Simple Logo
	Surf_Logo = CSurface::OnLoad(PATH_IMAGES "spaceship_splash.png");

	StartTime = SDL_GetTicks();
}

//-----------------------------------------------------------------------------
void CAppStateIntro::OnDeactivate() {
    if(Surf_Logo) {
        SDL_FreeSurface(Surf_Logo);
	    Surf_Logo = NULL;
    }
}

//-----------------------------------------------------------------------------
void CAppStateIntro::OnLoop() {
	if(static_cast<unsigned int>(StartTime + IntroLength) < SDL_GetTicks()) {
		CAppStateManager::SetActiveAppState(NextState);
	}
}

//-----------------------------------------------------------------------------
void CAppStateIntro::OnRender(SDL_Surface* Surf_Display) {
	if(Surf_Logo) {
		CSurface::OnDraw(Surf_Display, Surf_Logo, 0, 0);
	}
}

//-----------------------------------------------------------------------------
void CAppStateIntro::OnLButtonDown(int mX, int mY) {
	CAppStateManager::SetActiveAppState(NextState);
}

//=============================================================================
CAppStateIntro* CAppStateIntro::GetInstance() {
	return &Instance;
}

//=============================================================================
