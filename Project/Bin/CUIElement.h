//==============================================================================
#ifndef _CUIELEMENT_H_
    #define _CUIELEMENT_H_

#include <vector>

#include "CSurface.h"

//=============================================================================
enum {
	UI_NORMAL,
	UI_ACTIVE,
	UI_DISABLED,
	UI_HIDDEN
};

//==============================================================================
class CUIElement{
	public:
		static std::vector<CUIElement*>    UIElementList;

	protected:
		SDL_Surface*    Surf_Entity;

	public:
		int		X;
		int		Y;

		int		Width;
		int		Height;

		int		State;
		bool	Hover;

	public:
		CUIElement();

		virtual ~CUIElement();

	public:
		virtual bool OnLoad(char* File, int Width, int Height);

		virtual void OnLoop();

		virtual void OnRender(SDL_Surface* Surf_Display);

		virtual void OnCleanup();

	public:
		virtual	void Disable();
		virtual	void Enable();
		virtual	void Hide();
		virtual	void Show();

		virtual void OnClick();

		bool	IsClicked(int oX, int oY);
};

//==============================================================================

#endif