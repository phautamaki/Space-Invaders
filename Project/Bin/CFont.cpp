//==============================================================================
#include "CFont.h"
#include <string>
#include "functions.h"

//==============================================================================
CFont CFont::FontControl;

//==============================================================================
CFont::CFont() {
	Color.r = 255;
	Color.g = 215;
	Color.b = 0;
}

//------------------------------------------------------------------------------
bool CFont::OnLoad(char* File, int size, long index) {
	if( File == NULL || size < 1 || index < 0 ) return false;

	TTF_Font* TempFont = NULL;

	if( (TempFont = TTF_OpenFontIndex(File, size, index)) == NULL ){
		return false;
	}

	Default = TempFont;
	Size = size;
	FontPath = File;

	std::string filename = File;
	debug("Loaded font " + filename, 2);

	return true;
}

//------------------------------------------------------------------------------
void CFont::OnCleanup() {
	TTF_CloseFont(Default);
}

//==============================================================================
void CFont::Write(SDL_Surface* Surf_Display, const char* Text, unsigned int x, unsigned int y) {
    if( Default == NULL || Text == NULL ) return;

	SDL_Surface* Surf_Text = TTF_RenderText_Blended(Default, Text, Color);
	CSurface::OnDraw(Surf_Display, Surf_Text, x, y);
	SDL_FreeSurface(Surf_Text);
}

//------------------------------------------------------------------------------
void CFont::Write(SDL_Surface* Surf_Display, const char* Text, unsigned int x, unsigned int y, int size) {
	int original_size = Size;
	OnCleanup();
	OnLoad(FontPath, size);
	Write(Surf_Display, Text, x, y);
	OnCleanup();
	OnLoad(FontPath, original_size);
}

//------------------------------------------------------------------------------
void CFont::SetFontStyle(int style) {
	if( style < 0 ) return;
	TTF_SetFontStyle(Default, style);
}

//------------------------------------------------------------------------------
void CFont::SetFontOutline(int outline) {
	if( outline < 0 ) return;
	TTF_SetFontOutline(Default, outline);
}

//------------------------------------------------------------------------------
void CFont::GetTextSize(const char *text, int *w, int *h) {
	TTF_SizeText(Default, text, w, h);
}
//==============================================================================
