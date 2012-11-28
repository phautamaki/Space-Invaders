//=============================================================================
#include "CAppStateMainMenu.h"
#include "CAppStateManager.h"
#include "CFont.h"
#include "Paths.h"
#include "Define.h"

//=============================================================================
CAppStateMainMenu CAppStateMainMenu::Instance;

//=============================================================================
CAppStateMainMenu::CAppStateMainMenu() {
}
//=============================================================================
void CAppStateMainMenu::OnLButtonDown(int mX, int mY) {
	if( Exit.IsClicked(mX,mY) ) {
		Exit.OnClick();
		CAppStateManager::SetActiveAppState(APPSTATE_NONE);
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
	else if( Start.IsClicked(mX,mY) ) {
		Start.OnClick();
		CAppStateManager::SetActiveAppState(NextState);
	}
	else if( Scores.IsClicked(mX,mY) ) {
		Scores.OnClick();
		NextState = APPSTATE_SCORES;
		CAppStateManager::SetActiveAppState(NextState);
	}
}

//=============================================================================
void CAppStateMainMenu::OnActivate() {
	NextState = APPSTATE_GAME;

	if((MainBG = CSurface::OnLoad(PATH_IMAGES PATH_TITLE FILENAME_TITLE_BG)) == NULL) {
		return;
	}
	if((Title1 = CSurface::OnLoad(PATH_IMAGES PATH_TITLE FILENAME_TITLE_TEXT_1)) == NULL) {
		return;
	}
	if((Title2 = CSurface::OnLoad(PATH_IMAGES PATH_TITLE FILENAME_TITLE_TEXT_2)) == NULL) {
		return;
	}

	Start.OnLoad("Start", WWIDTH/2-250, WHEIGHT - 150);
	Start.Hoverable = true;
	CUIElement::UIElementList.push_back(&Start);
	Exit.OnLoad("Quit", WWIDTH/2-50, WHEIGHT - 150);
	Exit.Hoverable = true;
	CUIElement::UIElementList.push_back(&Exit);
	Scores.OnLoad("Score", WWIDTH/2+150, WHEIGHT - 150);
	Scores.Hoverable = true;
	CUIElement::UIElementList.push_back(&Scores);
}

//-----------------------------------------------------------------------------
void CAppStateMainMenu::OnDeactivate() {
	SDL_FreeSurface(MainBG);
	SDL_FreeSurface(Title1);
	SDL_FreeSurface(Title2);

	//--------------------------------------------------------------------------
    // UI
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CUIElement::UIElementList.size();i++) {
        if(!CUIElement::UIElementList[i]) continue;

        CUIElement::UIElementList[i]->OnCleanup();
    }

    CUIElement::UIElementList.clear();
}

//-----------------------------------------------------------------------------
void CAppStateMainMenu::OnLoop() {
    for(unsigned int i = 0;i < CUIElement::UIElementList.size();i++) {
        if(!CUIElement::UIElementList[i]) continue;
        CUIElement::UIElementList[i]->OnLoop();
    }
}

//-----------------------------------------------------------------------------
void CAppStateMainMenu::OnRender(SDL_Surface* Surf_Display) {
	CSurface::OnDraw(Surf_Display, MainBG, 0, 0);
	CSurface::OnDraw(Surf_Display, Title1, WWIDTH/2-377, 50);
	CSurface::OnDraw(Surf_Display, Title2, WWIDTH/2-157, 130);

	//--------------------------------------------------------------------------
    // UI
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CUIElement::UIElementList.size();i++) {
        if(!CUIElement::UIElementList[i]) continue;

        CUIElement::UIElementList[i]->OnRender(Surf_Display);
    }
}

//=============================================================================
CAppStateMainMenu* CAppStateMainMenu::GetInstance() {
	return &Instance;
}

//=============================================================================
