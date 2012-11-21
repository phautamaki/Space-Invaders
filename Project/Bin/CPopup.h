//=============================================================================
#ifndef _CPOPUP_H_
    #define _CPOPUP_H_

#include <string>

#include "CUIElement.h"
#include "CSurface.h"

//=============================================================================
class CPopup : public CUIElement {
	public:
        CPopup();

		bool OnLoad(std::string nText, int oX, int oY, int TimeInMS = 0);

		void OnLoop();

		void OnRender(SDL_Surface* Surf_Display);
		void OnRender(SDL_Surface* Surf_Display, int dX, int dY);

		void OnCleanup();

		void ChangeText(std::string& nText);
		void SetVisibilityTime(int TimeInMS);

		void Show();

	private:
		std::string Text;

		int VisibilityTime;
		int CreationTime;
};

//=============================================================================

#endif