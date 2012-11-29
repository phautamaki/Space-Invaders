//=============================================================================
#ifndef _CINPUT_H_
    #define _CINPUT_H_

#include <string>

#include "CUIElement.h"
#include "CSurface.h"

//=============================================================================
class CInput : public CUIElement {
	public:
        CInput();

		bool OnLoad(char* BackgroundFile, std::string nText, int oX, int oY, bool autoFocus = false);
		void OnLoop();
		void OnRender(SDL_Surface* Surf_Display);
		void OnCleanup();

		void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

		std::string GetValue() const;

	private:
		std::string Text;
		int MaxCharacters;
};

//=============================================================================

#endif