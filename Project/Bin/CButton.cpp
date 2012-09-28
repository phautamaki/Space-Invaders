//=============================================================================
#include "CButton.h"
#include "CFont.h"

//=============================================================================
CButton::CButton() {
	Surf_Entity = NULL;
}

//=============================================================================
bool CButton::OnLoad(std::string Text, int oX, int oY) {
	if( oX < 0 || oY < 0 || Text == "" ) return false;

	bool isText = false;
	// Text might be a path to an image
	if((Surf_Entity = CSurface::OnLoad(const_cast<char*>(Text.c_str()))) == NULL) {
		Label = Text;
		isText = true;
	}

	X = oX;
	Y = oY;

	if( isText ) {
		int tW = 0;
		int tH = 0;
		CFont::FontControl.GetTextSize(Text.c_str(), &tW, &tH );

		Width = tW+20;
		Height = tH+20;
	}
	else {
		Width = 200;
		Height = 200;
	}

	State = UI_NORMAL;

	return true;
}
//-----------------------------------------------------------------------------
void CButton::OnRender(SDL_Surface* Surf_Display) {
	if( State == UI_HIDDEN ) return;

	SDL_Rect Rect;
	Rect.x = X;
	Rect.y = Y;
	Rect.w = Width;
	Rect.h = Height;
	
	if( Surf_Entity != NULL ) {
		CSurface::OnDraw(Surf_Display, Surf_Entity, X, Y);
	}
	else if( State == UI_DISABLED ) {
		SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,190,190,190) );
	}
	else if( Hover ) {
		SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,0,255,0) );
	}
	else if( State == UI_ACTIVE ) {
		SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,0,0,255) );
	}
	else {
		SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,255,0,0) );
	}

	CFont::FontControl.Write(Surf_Display,Label.c_str(),X+10,Y+10);
}
//-----------------------------------------------------------------------------
void CButton::OnRender(SDL_Surface* Surf_Display, int dX, int dY) {
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
void CButton::OnCleanup() {
	CUIElement::OnCleanup();
}

//-----------------------------------------------------------------------------
void CButton::OnClick() {
	CUIElement::OnClick();
}

//=============================================================================