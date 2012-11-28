//=============================================================================
#include "CInput.h"
#include "CFont.h"

//=============================================================================
CInput::CInput() {
	Text = "";
}

//=============================================================================
bool CInput::OnLoad(char* BackgroundFile, std::string nText, int oX, int oY, bool autoFocus ) {
	if( oX < 0 || oY < 0 ) return false;

	if( BackgroundFile != "" ) {
		if( !CUIElement::OnLoad(BackgroundFile,0,0)) {
			return false;
		}
	}

	X = oX;
	Y = oY;

	Text = nText;

	Focus = autoFocus;

	CFont::FontControl.GetTextSize(Text.c_str(), &Width, &Height );

	State = UI_NORMAL;

	//Enable Unicode 
	SDL_EnableUNICODE(SDL_ENABLE); 

	return true;
}
//-----------------------------------------------------------------------------
void CInput::OnLoop() {
}

//-----------------------------------------------------------------------------
void CInput::OnRender(SDL_Surface* Surf_Display) {
	if( State == UI_HIDDEN ) return;

	// Background
	CUIElement::OnRender(Surf_Display);
	// Write text
	CFont::FontControl.Write(Surf_Display,Text.c_str(),X,Y);
}

//-----------------------------------------------------------------------------
void CInput::OnCleanup() {
	CUIElement::OnCleanup();
	//Disable Unicode 
	SDL_EnableUNICODE(SDL_DISABLE); 
}

//-----------------------------------------------------------------------------
void CInput::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	// Numbers 0-9
	if( (char)sym >= '0' || (char)sym >= '9' ){
		Text += (char)sym; 
	}
	// Letters a-z
	else if( (char)sym >= 'a' || (char)sym >= 'z' ){
		Text += (char)sym; 
	}
	// Space
	else if( sym == SDLK_SPACE ){
		Text += (char)sym; 
	}
	// Remove a character from the end 
	else if( ( sym == SDLK_BACKSPACE ) && ( Text.length() != 0 ) ) { 
		Text.erase( Text.length() - 1 ); 
	}
	// Return
	else if( sym == SDLK_RETURN ){
		Focus = false;
	}
}

//=============================================================================
std::string CInput::GetValue() const {
	return Text;
}

//=============================================================================