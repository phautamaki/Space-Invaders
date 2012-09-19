//==============================================================================
// Font Class
//==============================================================================
#ifndef _CFONT_H_
	#define _CFONT_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include "CSurface.h"

//==============================================================================
class CFont {
	public:
		static CFont	FontControl;

	public:
		CFont();

		bool OnLoad(char* File, int size, long index = 0);

		void OnCleanup();

	public:
		void Write(SDL_Surface* Surf_Display, char* Text);

		void SetFontStyle(int style);

		void SetFontOutline(int outline);

	private:
		TTF_Font*	Default;
		int			Size;
		SDL_Color	Color;
};

//==============================================================================

#endif