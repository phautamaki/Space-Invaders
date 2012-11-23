//==============================================================================
// Font Class
//==============================================================================
#ifndef _CFONT_H_
	#define _CFONT_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "CSurface.h"

//==============================================================================
class CFont {
	public:
		static CFont FontControl;

		struct FontData {
			char*		FontPath;
			TTF_Font*	Font;
			SDL_Color	Color;
		};

	private:
		std::vector<FontData> Fonts;

//------------------------------------------------------------------------------
	public:
		CFont();

		bool OnLoad(char* File, int size, long index = 0);

		void OnCleanup();

		// FontIndex = nth font from vector
		void Write(SDL_Surface* Surf_Display, const char* Text, unsigned int x, unsigned int y, unsigned int FontIndex = 0) const;

		void SetFontStyle(int style, unsigned int FontIndex = 0);

		void SetFontOutline(int outline, unsigned int FontIndex = 0);

		void SetFontColor(int r, int g, int b, unsigned int FontIndex = 0);

		void GetTextSize(const char *text, int *w, int *h, unsigned int FontIndex = 0);

	private:
		bool ValidIndex(unsigned int& Index) const;
};

//==============================================================================

#endif