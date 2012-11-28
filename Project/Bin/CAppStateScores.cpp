//=============================================================================
#include "CAppStateScores.h"
#include "CAppStateManager.h"

#include "Paths.h"
#include "Define.h"
#include "functions.h"

//=============================================================================
CAppStateScores CAppStateScores::Instance;

//=============================================================================
CAppStateScores::CAppStateScores() {
	MainBG = NULL;
}

//=============================================================================
void CAppStateScores::OnActivate() {
	NextState = APPSTATE_MAINMENU;

	// Load Simple Logo
	MainBG = CSurface::OnLoad(PATH_IMAGES "title_bg.png");

	Back.OnLoad("Back", WWIDTH/2+50, WHEIGHT - 150);
	Back.Hoverable = true;
	CUIElement::UIElementList.push_back(&Back);

	// Load scores
	if( !HighScore.OnInit() ) {
		error("Failed loading high scores.");
	}
}

//-----------------------------------------------------------------------------
void CAppStateScores::OnDeactivate() {
    if(MainBG) {
        SDL_FreeSurface(MainBG);
	    MainBG = NULL;
    }

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
void CAppStateScores::OnLoop() {
	for(unsigned int i = 0;i < CUIElement::UIElementList.size();i++) {
        if(!CUIElement::UIElementList[i]) continue;
        CUIElement::UIElementList[i]->OnLoop();
    }
}

//-----------------------------------------------------------------------------
void CAppStateScores::OnRender(SDL_Surface* Surf_Display) {
	if(MainBG) {
		CSurface::OnDraw(Surf_Display, MainBG, 0, 0);
	}

	HighScore.OnRender(Surf_Display);

	//--------------------------------------------------------------------------
    // UI
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CUIElement::UIElementList.size();i++) {
        if(!CUIElement::UIElementList[i]) continue;

        CUIElement::UIElementList[i]->OnRender(Surf_Display);
    }
}

//-----------------------------------------------------------------------------
void CAppStateScores::OnLButtonDown(int mX, int mY) {
	if( Back.IsClicked(mX,mY) ) {
		Back.OnClick();
		CAppStateManager::SetActiveAppState(NextState);
	}
}

//=============================================================================
CAppStateScores* CAppStateScores::GetInstance() {
	return &Instance;
}

//=============================================================================
