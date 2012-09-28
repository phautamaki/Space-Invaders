//=============================================================================
#include "CAppStateMainMenu.h"
#include "CAppStateManager.h"
#include "CFont.h"

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
		//CAppStateManager::SetActiveAppState(NextState);
	}
}

//=============================================================================
void CAppStateMainMenu::OnActivate() {
	//NextState = APPSTATE_GAME;

	Start.OnLoad("Start", 200, 100);
	CUIElement::UIElementList.push_back(&Start);
	Exit.OnLoad("Quit", 200, 300);
	CUIElement::UIElementList.push_back(&Exit);
}

//-----------------------------------------------------------------------------
void CAppStateMainMenu::OnDeactivate() {
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
}

//-----------------------------------------------------------------------------
void CAppStateMainMenu::OnRender(SDL_Surface* Surf_Display) {
	SDL_Rect Rect;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = WWIDTH;
	Rect.h = WHEIGHT;

	SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,100,215,0) );

	//--------------------------------------------------------------------------
    // UI
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CUIElement::UIElementList.size();i++) {
        if(!CUIElement::UIElementList[i]) continue;

        CUIElement::UIElementList[i]->OnRender(Surf_Display);
    }

//	CFont::FontControl.Write(Surf_Display, "Space Invaders", 50, 50);
}

//=============================================================================
CAppStateMainMenu* CAppStateMainMenu::GetInstance() {
	return &Instance;
}

//=============================================================================
