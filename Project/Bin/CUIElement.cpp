#include "CUIElement.h"

//==============================================================================
std::vector<CUIElement*> 	CUIElement::UIElementList;

//==============================================================================
CUIElement::CUIElement() {
	Hover = false;
	Hoverable = false;
	Surf_Entity = NULL;
}

//-----------------------------------------------------------------------------
CUIElement::~CUIElement() {
}

//==============================================================================
bool CUIElement::OnLoad(char* File, int Width, int Height) {
	return true;
}

//-----------------------------------------------------------------------------
void CUIElement::OnLoop() {
	if( Hoverable ) {
		int mouse_x;
		int mouse_y;

		SDL_PumpEvents();
		SDL_GetMouseState(&mouse_x, &mouse_y);

		if( this->IsClicked(mouse_x, mouse_y) ) {
			Hover = true;
		}
		else {
			Hover = false;
		}
	}
}

//-----------------------------------------------------------------------------
void CUIElement::OnRender(SDL_Surface* Surf_Display) {
	if(Surf_Entity == NULL || Surf_Display == NULL) return;
}

//-----------------------------------------------------------------------------
void CUIElement::OnCleanup() {
	if(Surf_Entity != NULL) {
        SDL_FreeSurface(Surf_Entity);
    }
    Surf_Entity = NULL;
}

//==============================================================================
void CUIElement::Disable() {
	State = UI_DISABLED;
}

//-----------------------------------------------------------------------------
void CUIElement::Enable() {
	State = UI_NORMAL;
}

//-----------------------------------------------------------------------------
void CUIElement::Hide() {
	State = UI_HIDDEN;
}

//-----------------------------------------------------------------------------
void CUIElement::Show() {
	this->Enable();
}

//==============================================================================
void CUIElement::OnClick() {
	if( State == UI_ACTIVE ) {
		State = UI_NORMAL;
	}
	else {
		State = UI_ACTIVE;
	}
}

//-----------------------------------------------------------------------------
bool CUIElement::IsClicked(int oX, int oY) {
	if( State == UI_DISABLED || State == UI_HIDDEN ) return false;

	if( oX > X && oX < X + Width ) {
		if( oY > Y && oY < Y + Height ) {
			return true;
		}
	}
	return false;
}

//==============================================================================