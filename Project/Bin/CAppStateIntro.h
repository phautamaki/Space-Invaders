//=============================================================================
#ifndef __CAPPSTATEINTRO_H__
	#define __CAPPSTATEINTRO_H__

#include "CAppState.h"
#include "CSurface.h"

//=============================================================================
class CAppStateIntro : public CAppState {
	private:
		static CAppStateIntro Instance;

		SDL_Surface* Surf_Logo;

		int StartTime;
		int	IntroLength;

	private:
		CAppStateIntro();

	public:
		void OnActivate();

		void OnDeactivate();

		void OnLoop();

		void OnRender(SDL_Surface* Surf_Display);

		void OnLButtonDown(int mX, int mY);

	public:
		static CAppStateIntro* GetInstance();
};

//=============================================================================

#endif
