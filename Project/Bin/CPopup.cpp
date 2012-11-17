//=============================================================================
#include "CPopup.h"
#include "CFont.h"

//=============================================================================
CPopup::CPopup() {
	Text = "";
	VisibilityTime = 0;
	CreationTime = 0;
}

//=============================================================================
bool CPopup::OnLoad(std::string nText, int oX, int oY, int TimeInMS) {
	if( oX < 0 || oY < 0 || nText == "" ) return false;

	X = oX;
	Y = oY;

	Text = nText;

	VisibilityTime = TimeInMS;

	CFont::FontControl.GetTextSize(Text.c_str(), &Width, &Height );

	State = UI_NORMAL;

	return true;
}
//-----------------------------------------------------------------------------
void CPopup::OnLoop() {
	if( VisibilityTime > 0 ) {
		if(static_cast<unsigned int>(CreationTime + VisibilityTime) < SDL_GetTicks()) {
			Hide();
		}
	}
}

//-----------------------------------------------------------------------------
void CPopup::OnRender(SDL_Surface* Surf_Display) {
	if( State == UI_HIDDEN ) return;

	// Write text
	CFont::FontControl.Write(Surf_Display,Text.c_str(),X,Y);
}
//-----------------------------------------------------------------------------
void CPopup::OnRender(SDL_Surface* Surf_Display, int dX, int dY) {
	if( dX < 0 || dY < 0 ) return;

	int oldX = X; 
	int oldY = Y;

	X = dX; 
	Y = dY;

	this->OnRender(Surf_Display);

	X = oldX;
	Y = oldY;
}
//-----------------------------------------------------------------------------
void CPopup::OnCleanup() {
	CUIElement::OnCleanup();
}

//=============================================================================
void CPopup::ChangeText(std::string nText) {
	if( nText != "" ) {
		Text = nText;
	}
}

//-----------------------------------------------------------------------------
void CPopup::SetVisibilityTime(int TimeInMS) {
	VisibilityTime = TimeInMS;
}

//=============================================================================
void CPopup::Show() {
	Enable();
	CreationTime = SDL_GetTicks();
}

//=============================================================================