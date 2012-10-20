//=============================================================================
#include "CButton.h"
#include "CFont.h"
#include "Paths.h"
#include "functions.h"

//=============================================================================
CButton::CButton() {
	Surf_Entity = NULL;
	TextStart = 0;

	ButtonClickSound = NULL;
}

//=============================================================================
bool CButton::OnLoad(std::string Text, int oX, int oY, bool Image) {
	if( oX < 0 || oY < 0 || Text == "" ) return false;

	if( Image ) {
		if((Surf_Entity = CSurface::OnLoad(PATH_IMAGES PATH_UI FILENAME_UI_BUTTON)) == NULL) {
			return false;
		}
	}

	X = oX;
	Y = oY;

	Label = Text;

	int tW = 0;
	int tH = 0;
	CFont::FontControl.GetTextSize(Text.c_str(), &tW, &tH );
	if( !Image ) {
		Width = tW+20;
		Height = tH+20;
	}
	else {
		Width = 136;
		Height = 84;
		TextStart = Width/2 - tW/2;
	}

	State = UI_NORMAL;

	ButtonClickSound = Mix_LoadWAV(PATH_EFFECTS FILENAME_BUTTON_CLICK);
	ButtonClickSound == NULL ? debug("Shit hit the fan when loading ButtonClickSound.") : debug("Loading ButtonClickSound was a great success!");

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
	
	if( State == UI_DISABLED ) {
		SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,190,190,190) );
	}
	else if( Hover ) {
		if( Surf_Entity != NULL ) {
			CSurface::OnDraw(Surf_Display, Surf_Entity, X, Y, Width, 0, Width, Height);
		}
		else {
			SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,0,255,0) );
		}
	}
	else if( State == UI_ACTIVE ) {
		SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,0,0,255) );
	}
	else {
		if( Surf_Entity != NULL ) {
			CSurface::OnDraw(Surf_Display, Surf_Entity, X, Y, 0, 0, Width, Height);
		}
		else {
			SDL_FillRect( Surf_Display, &Rect, SDL_MapRGB(Surf_Display->format,255,0,0) );
		}
	}

	// Write button text
	if( Surf_Entity != NULL ) {
		CFont::FontControl.Write(Surf_Display,Label.c_str(),X+TextStart,Y+10);
	}
	else {
		CFont::FontControl.Write(Surf_Display,Label.c_str(),X+10,Y+10);
	}
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
	Mix_PlayChannel( -1, ButtonClickSound, 0 );
}

//=============================================================================