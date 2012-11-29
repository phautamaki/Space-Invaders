//=============================================================================
#include <fstream>
#include <sstream>
#include <string>
#include "CAppStateGame.h"
#include "CFont.h"
#include "CFactory.h"
#include "functions.h"
#include "Paths.h"
#include "CFileReader.h"
#include "CSoundBank.h"
#include "CHighScores.h"

#include "CApp.h"
#include "CPopup.h"

//=============================================================================
CAppStateGame CAppStateGame::Instance;

//=============================================================================
CAppStateGame::CAppStateGame() {
	ResetCurrentLevel = false;
	BossFightOn = false;
	BossDead = false;
	GameOver = false;
}

//=============================================================================
void CAppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if( Input.HasFocus() ){
		Input.OnKeyDown(sym,mod,unicode);
	}
	else if ( sym == SDLK_F1) {
		CFactory::Factory.FlagNonPlayerEntities();
	}
	else if ( sym == SDLK_F2) {
		Player->Gun.ChangeType(GUN_NORMAL);
	}
	else if ( sym == SDLK_F3) {
		Player->Gun.ChangeType(GUN_BEAM);
	}
	else if ( sym == SDLK_F4) {
		Player->Gun.ChangeType(GUN_MISSILES);
	}
	else {
		Player->OnKeyDown(sym, mod, unicode);
	}
}

//------------------------------------------------------------------------------
void CAppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if( Input.HasFocus() ){
		Input.OnKeyUp(sym,mod,unicode);
	}
	else {
		Player->OnKeyUp(sym, mod, unicode);
	}
}

//=============================================================================
void CAppStateGame::OnActivate() {
	debug("Game initialization start");

	// Loading area
	// Note that area is the whole level which might consist of many maps
	debug("Area loading start", 1);
	if(CArea::AreaControl.OnLoad(PATH_MAPS PATH_AREA1) == false) {
    	return;
    }
	debug("All areas loaded successfully", 1);

	debug("Entity loading start", 1);
	Player = CFactory::Factory.CreatePlayer(PLAYER_STARTING_X, PLAYER_STARTING_Y);

	// TODO: Player2 not needed yet. Make the game support co-op later

	//We would like to start from the first level.
	CurrentLevelNumber = 1;

	//This is mainly for testing purpose in here (of course we usually start from the first level).
	std::string LevelInfoFile = "level"+IntToString(CurrentLevelNumber)+"info.txt";

	//Loading of level information. Level changing feature should be implemented.
	Level = GetCurrentLevelInfo (LevelInfoFile);

	//We start from the first item in the vector
	LevelInfoIndex = 0;

	//Sets the point where the level should be changed (Information is in the last line of the level info file).
	LevelEndingPoint = Level.back().ActiveXPosition;

	debug("Levelendingpoint: "+IntToString(LevelEndingPoint));

	debug("All entities loaded successfully", 1);

	debug("Additional image loading stat", 1);
	if((UIIcons = CSurface::OnLoad(PATH_IMAGES PATH_UI FILENAME_UI_ICONS)) == NULL) {
		error("Couldn't load UI life icon.");
		return;
	}
	if((UItiles = CSurface::OnLoad(PATH_IMAGES PATH_UI FILENAME_UI_TILES)) == NULL) {
		error("Couldn't load UI tilesheet.");
		return;
	}
	if((SpaceBG = CSurface::OnLoad(PATH_IMAGES FILENAME_SPACE_BG_1)) == NULL) {
		error("Couldn't load background image.");
		return;
	}
	BG_offset = BG_WIDTH;
	debug("Additional images loaded successfully", 1);

	// Camera initialization, make it start from 0,0
	CCamera::CameraControl.TargetMode = TARGET_MODE_NORMAL;
	debug("Camera set", 1);

	std::string LevelMusicID = IntToString(CurrentLevelNumber);
	std::string LevelMusicFile = "level" + IntToString(CurrentLevelNumber) + "music.ogg";

	//Loads the level music TODO: Music!
	//CSoundBank::SoundControl.OnLoad(CSoundBank::MUSIC, LevelMusicID, PATH_MUSIC "level1music.ogg");

	//Plays the music
	//CSoundBank::SoundControl.Play(CSoundBank::MUSIC, LevelMusicID);
	if(!Input.OnLoad(PATH_IMAGES PATH_UI "input_field.png","",WWIDTH/2-165,WHEIGHT/2-55, true)){
		error("Couldn't load input field");
		return;
	}
	Input.Hide();

	debug("Game initialization successful");
	ResetLevel();
}

//-----------------------------------------------------------------------------

//TODO: This function still lacks the functionality to set the camera to the correct place when level changes (or however the levels are implemented)
void CAppStateGame::OnLevelChange() {

	CurrentLevelNumber++;

	//If we have survived the last level (TODO:next state should be end credits or something?)
	
	if (CurrentLevelNumber == 1) {
		debug("Area 1 loading start", 1);
		if(CArea::AreaControl.OnLoad(PATH_MAPS PATH_AREA1) == false) {
			return;
		}
	}
	else if (CurrentLevelNumber == 2) {
		debug("Area 2 loading start", 1);
		if(CArea::AreaControl.OnLoad(PATH_MAPS PATH_AREA2) == false) {
			return;
		}
	}
	else if(CurrentLevelNumber > 2) {
		NextState = APPSTATE_MAINMENU;
		CAppStateManager::SetActiveAppState(NextState);
		return;
	}

	std::string LevelInfoFile = "level"+IntToString(CurrentLevelNumber)+"info.txt";
	
	//Loading of new level information
	Level = GetCurrentLevelInfo (LevelInfoFile);

	debug("Changed to level" + IntToString(CurrentLevelNumber));

	//Let's set the new level ending point
	LevelEndingPoint = Level.back().ActiveXPosition;

	std::string LevelMusicID = IntToString(CurrentLevelNumber);
	std::string LevelMusicFile = "level"+IntToString(CurrentLevelNumber) + "music.ogg";

	//Loads the level music TODO: Music!
	//CSoundBank::SoundControl.OnLoad(CSoundBank::MUSIC, LevelMusicID, PATH_MUSIC "level2music.ogg");

	//Plays the music
	//CSoundBank::SoundControl.Play(CSoundBank::MUSIC, LevelMusicID);

	ResetLevel();

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void CAppStateGame::OnDeactivate() {
	Input.OnCleanup();

	CArea::AreaControl.OnCleanup();

	// Empty entities
	CFactory::Factory.OnCleanup();

	//CSoundBank::SoundControl.OnCleanup();
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnLoop() {

	if( GameOver ) {
		if( !Input.HasFocus() ) {
			if( Input.State == UI_NORMAL ) {
				CHighScores HighScore;
				HighScore.OnInit();
				HighScore.Add(Input.GetValue(),Player->Points);
			}
			GameOver = false;
			CAppStateManager::SetActiveAppState(NextState);
		}
		return;
	}

	//If the level should change
	//if( CCamera::CameraControl.GetX() >= LevelEndingPoint ) {
	//	OnLevelChange();
	//}

	// Spawn new entities
	//else if( LevelInfoIndex < Level.size() && 
	if( LevelInfoIndex < Level.size() &&
		(unsigned int)CCamera::CameraControl.GetX() >= Level.at(LevelInfoIndex).ActiveXPosition) {

		CAppStateGame::LevelInfo TmpInfo = Level.at(LevelInfoIndex);

		switch(TmpInfo.Type) { 
			case ENTITY_TYPE_ENEMY:
				debug("creating enemy of type: " + IntToString(TmpInfo.SubType));
				CFactory::Factory.CreateEnemy(TmpInfo.SubType, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
				break;
			case ENTITY_TYPE_ITEM:
				CFactory::Factory.CreateItem(TmpInfo.SubType, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
				break;
			case ENTITY_TYPE_SPECIAL_EFFECT:
				// NOTE: y-position is now indicating the ms duration for the slowmo effect

				if (TmpInfo.SubType == ENTITY_SUBTYPE_SPECIAL_EFFECT_SLOWMO_2X) {
					CFactory::Factory.CreateSlowMotion(LEVEL_SLOWMO_2X, TmpInfo.YPosition);
				}
				else if (TmpInfo.SubType == ENTITY_SUBTYPE_SPECIAL_EFFECT_SLOWMO_4X) {
					CFactory::Factory.CreateSlowMotion(LEVEL_SLOWMO_4X, TmpInfo.YPosition);
				}
				else if (TmpInfo.SubType == ENTITY_SUBTYPE_SPECIAL_EFFECT_SLOWMO_8X) {
					CFactory::Factory.CreateSlowMotion(LEVEL_SLOWMO_8X, TmpInfo.YPosition);
				}
				break;
			default:
				break;
		}

		LevelInfoIndex++;
	}

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnLoop();
    }

    //Collision Events
    for(unsigned int i = 0;i < CEntityCol::EntityColList.size();i++) {
        CEntity* EntityA = CEntityCol::EntityColList[i].EntityA;
        CEntity* EntityB = CEntityCol::EntityColList[i].EntityB;

        if(EntityA == NULL || EntityB == NULL) continue;

		/* Pixel precise collision detection */

		// Create rectangle of the possible collision area
		SDL_Rect collisionRect = CEntity::Intersection(EntityA->GetBounds(), EntityB->GetBounds());
		
		// Check if there is even a possible collision
		if(collisionRect.w != 0 && collisionRect.h != 0 ) {
			// Check if there is/are actual pixel overlapping between
			// both entities. If there is...
			if(CEntity::CheckCollision(EntityA, EntityB)) {
				// Let both entities know of the collision
				EntityA->OnCollision(EntityB);
				EntityB->OnCollision(EntityA);
			}
		}
    }

    CEntityCol::EntityColList.clear();

	// Camera movement:
	// Make camera move wanted amount of pixels per second to right
	float moveX = CCamera::CameraControl.speed * CFPS::FPSControl.GetSpeedFactor();

	// If we are not in the boss area
	if ( (CCamera::CameraControl.GetX() + (int)moveX) < LEVEL_LENGTH) {
		CCamera::CameraControl.OnMove(moveX, static_cast<float>(CCamera::CameraControl.GetY()));
	
		// Update BG offset
		BG_offset = BG_offset - BG_SPEED * CFPS::FPSControl.GetSpeedFactor();
		if( BG_offset <= 0 ) {
			BG_offset = BG_WIDTH;
		}
	}
	// When we come to the boss map of the level area
	else {
		CCamera::CameraControl.SetPos(LEVEL_LENGTH, CCamera::CameraControl.GetY());
		CCamera::CameraControl.speed = 0;
		BossFightOn = true;
	}
	

	if (ResetCurrentLevel) {
		ResetLevel();
	}
	else if (BossFightOn && BossDead) {
		OnLevelChange();
	}
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnRender(SDL_Surface* Surf_Display) {
	SDL_Rect Rect;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = WWIDTH;
	Rect.h = WHEIGHT;

	SDL_FillRect(Surf_Display, &Rect, 0);

	CSurface::OnDraw(Surf_Display, SpaceBG, (int)BG_offset, GUI_HEIGHT);
	CSurface::OnDraw(Surf_Display, SpaceBG, (int)(BG_offset-BG_WIDTH), GUI_HEIGHT);
	CArea::AreaControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnRender(Surf_Display);
    }

	//--------------------------------------------------------------------------
    // UI
    //--------------------------------------------------------------------------
// Todo: Make a UI class for handling this functionaly?
	RenderUIBase(Surf_Display);
	RenderUIIcons(Surf_Display);

	// Popup
	CFactory::Factory.Popup.OnRender(Surf_Display);
	// Input
	Input.OnRender(Surf_Display);
}

//-----------------------------------------------------------------------------
void CAppStateGame::RenderUIBase(SDL_Surface* Surf_Display) const {
	// Tiles
	// Corners
	CSurface::OnDraw(Surf_Display, UItiles, 0, 0, 0, 0, TILE_SIZE, TILE_SIZE);
	CSurface::OnDraw(Surf_Display, UItiles, WWIDTH-TILE_SIZE, 0, TILE_SIZE*2, 0, TILE_SIZE, TILE_SIZE);
	CSurface::OnDraw(Surf_Display, UItiles, 0, GUI_HEIGHT-TILE_SIZE, 0,  TILE_SIZE*2, TILE_SIZE, TILE_SIZE);
	CSurface::OnDraw(Surf_Display, UItiles, WWIDTH-TILE_SIZE, GUI_HEIGHT-TILE_SIZE, TILE_SIZE*2,  TILE_SIZE*2, TILE_SIZE, TILE_SIZE);
	// Middles
	for(unsigned int i = 1; i < MAP_WIDTH-1; i++ ){
		CSurface::OnDraw(Surf_Display, UItiles, TILE_SIZE*i, 0, TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
		CSurface::OnDraw(Surf_Display, UItiles, TILE_SIZE*i, GUI_HEIGHT-TILE_SIZE, TILE_SIZE, TILE_SIZE*2, TILE_SIZE, TILE_SIZE);
	}
	for(unsigned int i = 1; i < (GUI_HEIGHT/TILE_SIZE)-1; i++ ){
		CSurface::OnDraw(Surf_Display, UItiles, 0, TILE_SIZE*i, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE);
		CSurface::OnDraw(Surf_Display, UItiles, WWIDTH-TILE_SIZE, TILE_SIZE*i, TILE_SIZE*2, TILE_SIZE, TILE_SIZE, TILE_SIZE);
	}
	// Fill
	for(unsigned int i = 1; i < MAP_WIDTH-1; i++ ){
		for(unsigned int j = 1; j < (GUI_HEIGHT/TILE_SIZE)-1; j++ ){
			CSurface::OnDraw(Surf_Display, UItiles, TILE_SIZE*i, TILE_SIZE*j, TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}
}

//-----------------------------------------------------------------------------
void CAppStateGame::RenderUIIcons(SDL_Surface* Surf_Display) const {
	CFont::FontControl.Write(Surf_Display,"Lives:", TILE_SIZE, TILE_SIZE,1);
	// Life icons
	for(unsigned int i = 0; i < PLAYER_MAX_LIVES; i++ ){
		CSurface::OnDraw(Surf_Display, UIIcons, TILE_SIZE + (i*36), TILE_SIZE*2,140,0,35,40);
	}
	for(unsigned int i = 0; i < Player->Lives; i++ ){
		CSurface::OnDraw(Surf_Display, UIIcons, TILE_SIZE + (i*36), TILE_SIZE*2,0,0,35,40);
	}

	// Weapon info
	CFont::FontControl.Write(Surf_Display,"Weapon:", TILE_SIZE*10, TILE_SIZE,1);
	CSurface::OnDraw(Surf_Display, UIIcons, TILE_SIZE*10, TILE_SIZE*2,35+35*Player->Gun.GetType(),0,35,40);

	// Points
	CFont::FontControl.Write(Surf_Display,"Points:", TILE_SIZE*20, TILE_SIZE,1);
	CFont::FontControl.Write(Surf_Display,IntToString(Player->Points).c_str(), TILE_SIZE*20, TILE_SIZE*2,1);

	// Draw ChargeBar only for basic gun
	if( Player->Gun.GetType() == GUN_NORMAL ) {
		int BarStart = TILE_SIZE*11;
		SDL_Rect RectRed;
		RectRed.x = BarStart;
		RectRed.y = TILE_SIZE*2+5;
		RectRed.w = Player->Gun.ChargeLevel * 10;
		RectRed.h = TILE_SIZE/2;
		SDL_Rect RectGray;
		RectGray.x = BarStart;
		RectGray.y = TILE_SIZE*2+5;
		RectGray.w = 100;
		RectGray.h = TILE_SIZE/2;

		SDL_FillRect(Surf_Display, &RectGray, SDL_MapRGB(Surf_Display->format, 211, 211, 211));
		SDL_FillRect(Surf_Display, &RectRed, SDL_MapRGB(Surf_Display->format, 255, 0, 0));
	}
}

//=============================================================================
CAppStateGame* CAppStateGame::GetInstance() {
	return &Instance;
}

//=============================================================================
void CAppStateGame::ResetLevel(){

	ResetCurrentLevel = false;
	BossFightOn = false;
	BossDead = false;

	// Reset camera starting position
	CCamera::CameraControl.SetPos(0,0);
	CCamera::CameraControl.speed = CAMERA_SPEED;
	
	if( Player->Lives == 0 ) {
		CheckHighScores();

		NextState = APPSTATE_MAINMENU;

		GameOver = true;
	}
	else {
		Player->TookHit = false;
		Player->SetHP(1);
		Player->X = PLAYER_STARTING_X;
		Player->Y = PLAYER_STARTING_Y+GUI_HEIGHT;
		Player->SpeedX = Player->SpeedY = Player->AccelX = Player->AccelY = 0;
		Player->Gun.Reset();

		//Let's set spawner index back to the start
		LevelInfoIndex = 0;

		//And kill everything except player 
		CFactory::Factory.FlagNonPlayerEntities();

		if (CurrentLevelNumber == 1) {
			debug("Area 1 loading start on resetlevel", 1);
			if(CArea::AreaControl.OnLoad(PATH_MAPS PATH_AREA1) == false) {
				return;
			}
		}
		else if (CurrentLevelNumber == 2) {
			debug("Area 2 loading start on resetlevel", 1);
			if(CArea::AreaControl.OnLoad(PATH_MAPS PATH_AREA2) == false) {
				return;
			}
		}


		std::stringstream ss;
		ss << CurrentLevelNumber;
		std::string text = "LEVEL " + ss.str();
		CFactory::Factory.CreateText(text, 2000, (int)Player->X, (int)Player->Y-100);
	}

	debug("ResetLevel() exit");
}

//-----------------------------------------------------------------------------
void CAppStateGame::CheckHighScores() {
	CHighScores HighScore;
	if( !HighScore.OnInit() ) {
		error("Failed loading high scores.");
	}
	else {
		// Check whether player made it to the list
		if( HighScore.CheckPoints(Player->Points) ){
			Input.Show();
		}
	}
}

//=============================================================================

std::vector<CAppStateGame::LevelInfo> CAppStateGame::GetCurrentLevelInfo(const std::string& filename) {

	//Temporary vector to be returned
	std::vector<CAppStateGame::LevelInfo> tmp;

	std::ifstream file;
	CFileReader::FileReader.GetFile(filename, file);
	std::string line;

	//In the file the contents should be in (one item on one line) 
	//Format: ActiveXPosition Type SubType YPosition.
	while(!file.eof()) {

		//First we get the item
		std::getline(file, line);
		std::istringstream istream(line);
		std::vector<int>values;
		std::string value = "";
		
		// Ignore comment lines and empty lines
		if (istream.peek() != '#' && istream.peek() != 15 && line != "") {

			//Then we get the separate values
			while(istream >> value) {
				values.push_back(StringToInt(value));
			}

			//Lousy check to see if contets in the file are in correct format
			if(values.size() == 4) {
				CAppStateGame::LevelInfo info;
				info.ActiveXPosition = values.at(0);
				info.Type = values.at(1);
				info.SubType = values.at(2);
				info.YPosition = values.at(3);

				tmp.push_back(info);
			}
			else {
				error("There was some problems with the file. Could not read contents properly");
				break;
			}
		}
	}

	file.close();
	return tmp;
}

//=============================================================================

void CAppStateGame::ResetLevelNow() {
	ResetCurrentLevel = true;
}
