//=============================================================================
#ifndef __CAPPSTATESCORES_H__
	#define __CAPPSTATESCORES_H__

#include "CAppState.h"
#include "CSurface.h"
#include "CButton.h"
#include "CHighScores.h"

//=============================================================================
class CAppStateScores : public CAppState {
	private:
		static CAppStateScores Instance;

		SDL_Surface* MainBG;
		CButton		 Back;

		CHighScores HighScore;

	private:
		CAppStateScores();

	public:
		void OnActivate();

		void OnDeactivate();

		void OnLoop();

		void OnRender(SDL_Surface* Surf_Display);

		void OnLButtonDown(int mX, int mY);

	public:
		static CAppStateScores* GetInstance();
};

//=============================================================================

#endif
