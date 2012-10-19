//=============================================================================
#ifndef _CBUTTON_H_
    #define _CBUTTON_H_

#include <string>

#include "CUIElement.h"
#include "CSurface.h"

//=============================================================================
class CButton : public CUIElement {
	public:
        CButton();

		bool OnLoad(std::string Text, int oX, int oY, bool Image = true);

		void OnRender(SDL_Surface* Surf_Display);
		void OnRender(SDL_Surface* Surf_Display, int dX, int dY);

		void OnCleanup();

		void OnClick();

	public:
		std::string Label;

	private:
		int TextStart;

		SDL_Surface*	Surf_Entity;

};

//=============================================================================

#endif