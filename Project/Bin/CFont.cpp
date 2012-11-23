//==============================================================================
#include "CFont.h"
#include <string>
#include "functions.h"

//==============================================================================
CFont CFont::FontControl;

//==============================================================================
CFont::CFont() {
}

//------------------------------------------------------------------------------
bool CFont::OnLoad(char* File, int size, long index) {
	if( File == NULL || size < 1 || index < 0 ) return false;

	FontData TempFont;

	if( (TempFont.Font = TTF_OpenFontIndex(File, size, index)) == NULL ){
		std::string filename = File;
		error("Couldn't load font " + filename + ".");
		return false;
	}
	TempFont.FontPath	= File;
	TempFont.Color.r = 255;
	TempFont.Color.g = 255;
	TempFont.Color.b = 0;

	Fonts.push_back(TempFont);

	return true;
}

//------------------------------------------------------------------------------
void CFont::OnCleanup() {
	for( unsigned int i = 0; i < Fonts.size(); i++ ) {
		TTF_CloseFont(Fonts.at(i).Font);
	}
}

//==============================================================================
void CFont::Write(SDL_Surface* Surf_Display, const char* Text, unsigned int x, unsigned int y, unsigned int FontIndex) const {
    if( Text == NULL || !ValidIndex(FontIndex) ) return;

	SDL_Surface* Surf_Text = TTF_RenderText_Blended(Fonts.at(FontIndex).Font, Text, Fonts.at(FontIndex).Color);
	CSurface::OnDraw(Surf_Display, Surf_Text, x, y);
	SDL_FreeSurface(Surf_Text);
}

//------------------------------------------------------------------------------
void CFont::SetFontStyle(int style, unsigned int FontIndex) {
	if( style < 0 || !ValidIndex(FontIndex) ) return;
	TTF_SetFontStyle(Fonts.at(FontIndex).Font, style);
}

//------------------------------------------------------------------------------
void CFont::SetFontOutline(int outline, unsigned int FontIndex) {
	if( outline < 0 || !ValidIndex(FontIndex) ) return;
	TTF_SetFontOutline(Fonts.at(FontIndex).Font, outline);
}

//------------------------------------------------------------------------------
void CFont::SetFontColor(int r, int g, int b, unsigned int FontIndex) {
	if( !ValidIndex(FontIndex) ) return;

	Fonts.at(FontIndex).Color.r = r;
	Fonts.at(FontIndex).Color.g = g;
	Fonts.at(FontIndex).Color.b = b;
}

//------------------------------------------------------------------------------
void CFont::GetTextSize(const char *text, int *w, int *h, unsigned int FontIndex) {
	if( !ValidIndex(FontIndex) ) return;

	TTF_SizeText(Fonts.at(FontIndex).Font, text, w, h);
}

//==============================================================================
bool CFont::ValidIndex(unsigned int &Index) const {
	if( Index >= Fonts.size() ) {
		error("Invalid Font index " + IntToString(Index) + ".");
		return false;
	}
	return true;
}
//==============================================================================

