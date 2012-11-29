//=============================================================================
#include "CInput.h"
#include "CFont.h"

//=============================================================================
CInput::CInput() {
	Text = "";
	MaxCharacters = 10;
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
	CFont::FontControl.Write(Surf_Display,"New high score!",X,Y);
	CFont::FontControl.SetFontColor(0,0,0,1);
	CFont::FontControl.Write(Surf_Display,"Name",X+10,Y+70,1);
	CFont::FontControl.SetFontColor(255,255,0,1);
	// Write text
	CFont::FontControl.Write(Surf_Display,Text.c_str(),X+110,Y+70,1);
}

//-----------------------------------------------------------------------------
void CInput::OnCleanup() {
	CUIElement::OnCleanup();
	//Disable Unicode 
	SDL_EnableUNICODE(SDL_DISABLE); 
}

//-----------------------------------------------------------------------------
void CInput::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if( Text.length() < MaxCharacters ) {
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
			//Text += (char)sym; 
			Text += '_';
		}
	}

	// Remove a character from the end 
	if( ( sym == SDLK_BACKSPACE ) && ( Text.length() != 0 ) ) { 
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