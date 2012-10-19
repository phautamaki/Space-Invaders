//=============================================================================
#ifndef __CAPPSTATEMAINMENU_H__
	#define __CAPPSTATEMAINMENU_H__

#include "CAppState.h"
#include "CSurface.h"
#include "CEntity.h"

#include "CButton.h"

//=============================================================================
class CAppStateMainMenu : public CAppState {
	private:
		static CAppStateMainMenu Instance;

		SDL_Surface* MainBG;
		SDL_Surface* Title1;
		SDL_Surface* Title2;

		CButton Start;
		CButton Exit;

	private:
		CAppStateMainMenu();

	public:
		void OnLButtonDown(int mX, int mY);

	public:
		void OnActivate();

		void OnDeactivate();

		void OnLoop();

		void OnRender(SDL_Surface* Surf_Display);

	public:
		static CAppStateMainMenu* GetInstance();
};

//=============================================================================

#endif