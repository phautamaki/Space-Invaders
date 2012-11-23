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

#include "CApp.h"
#include "CPopup.h"

//=============================================================================
CAppStateGame CAppStateGame::Instance;

//=============================================================================
CAppStateGame::CAppStateGame() {
	ResetCurrentLevel = false;
	BossFightOn = false;
	BossDead = false;
}

//=============================================================================
void CAppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	Player->OnKeyDown(sym, mod, unicode);
}

//------------------------------------------------------------------------------
void CAppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	Player->OnKeyUp(sym, mod, unicode);
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
	Player = CFactory::Factory.CreatePlayer(100, 240);

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
	if((IconLife = CSurface::OnLoad(PATH_IMAGES PATH_UI FILENAME_UI_ICON_LIFE)) == NULL) {
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

	debug("Game initialization successful");
	ResetLevel();
}

//-----------------------------------------------------------------------------

//TODO: This function still lacks the functionality to set the camera to the correct place when level changes (or however the levels are implemented)
void CAppStateGame::OnLevelChange() {

	CurrentLevelNumber++;

	//If we have survived the last level (TODO:next state should be end credits or something?)
	if(CurrentLevelNumber > 2) {
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
	CArea::AreaControl.OnCleanup();

	// Empty entities
	CFactory::Factory.OnCleanup();

	//CSoundBank::SoundControl.OnCleanup();
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnLoop() {


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
				// Enemy Ship
				if (TmpInfo.SubType == ENTITY_SUBTYPE_ENEMY_1) {
					CFactory::Factory.CreateEnemyShip(TmpInfo.SubType, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
				}
				else if (TmpInfo.SubType == ENTITY_SUBTYPE_ENEMY_BOSS_1) {
					CFactory::Factory.CreateEnemyShip(ENTITY_SUBTYPE_ENEMY_BOSS_1, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
					BossFightOn = true;
				}

				break;
			case ENTITY_TYPE_ITEM:
				// Item
				CFactory::Factory.CreateItem(TmpInfo.SubType, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
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

	if ( (CCamera::CameraControl.GetX() + (int)moveX) < LEVEL_LENGTH) {
		CCamera::CameraControl.OnMove(moveX, static_cast<float>(CCamera::CameraControl.GetY()));
	
		// Update BG offset
		BG_offset = BG_offset - BG_SPEED * CFPS::FPSControl.GetSpeedFactor();
		if( BG_offset <= 0 ) {
			BG_offset = BG_WIDTH;
		}
	}
	else {
		CCamera::CameraControl.SetPos(LEVEL_LENGTH, CCamera::CameraControl.GetY());
		CCamera::CameraControl.speed = 0;
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
	CFont::FontControl.Write(Surf_Display,"Lives:", TILE_SIZE, TILE_SIZE,1);
	// Life icons
	for(unsigned int i = 0; i < Player->Lives; i++ ){
		CSurface::OnDraw(Surf_Display, IconLife, TILE_SIZE + (i*30), TILE_SIZE*2);
	}
	CFont::FontControl.Write(Surf_Display,"Points:", TILE_SIZE*20, TILE_SIZE,1);
	CFont::FontControl.Write(Surf_Display,IntToString(Player->Points).c_str(), TILE_SIZE*21, TILE_SIZE*2,1);
	CFactory::Factory.Popup.OnRender(Surf_Display);
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
		NextState = APPSTATE_MAINMENU;
		CAppStateManager::SetActiveAppState(NextState);
	}
	else {
		Player->TookHit = false;
		Player->SetHP(1);
		Player->X = 100;
		Player->Y = 240+GUI_HEIGHT;
		Player->SpeedX = Player->SpeedY = Player->AccelX = Player->AccelY = 0;

		//Let's set spawner index back to the start
		LevelInfoIndex = 0;

		//And kill everything except player 
		CFactory::Factory.FlagNonPlayerEntities();

		// Restore already broken breakable tiles
		CArea::AreaControl.RestoreBrokenTiles();

		std::stringstream ss;
		ss << CurrentLevelNumber;
		std::string text = "LEVEL " + ss.str();
		CFactory::Factory.CreateText(text, 2000, (int)Player->X, (int)Player->Y-100);
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
				debug("There was some problems with the file. Could not read contents properly");
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
