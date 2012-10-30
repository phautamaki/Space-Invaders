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

//=============================================================================
CAppStateGame CAppStateGame::Instance;

//=============================================================================
CAppStateGame::CAppStateGame() {
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
	Player = CFactory::Factory.CreatePlayer(400, 290);

	// TODO: Player2 not needed yet. Make the game support co-op later

	//Loading of level information. Level changing feature should be implemented.
	Level = GetCurrentLevelInfo ("level1info.txt");

	//We start from the first item in the vector
	LevelInfoIndex = 0;

	debug("All entities loaded successfully", 1);

	debug("Additional image loading stat", 1);
	if((IconLife = CSurface::OnLoad(PATH_IMAGES PATH_UI FILENAME_UI_ICON_LIFE)) == NULL) {
		return;
	}
	if((SpaceBG = CSurface::OnLoad(PATH_IMAGES FILENAME_SPACE_BG_1)) == NULL) {
		return;
	}
	BG_offset = BG_WIDTH;
	debug("Additional images loaded successfully", 1);

	// Camera initialization, make it start from 0,0
	CCamera::CameraControl.TargetMode = TARGET_MODE_NORMAL;
	debug("Camera set", 1);

	//Loads the level music TODO: Music!
	//CSoundBank::SoundControl.OnLoad(CSoundBank::MUSIC, "FirstLevelMusic", PATH_MUSIC "darkness.ogg");

	//Plays the music
	//CSoundBank::SoundControl.Play(CSoundBank::MUSIC, "FirstLevelMusic");

	debug("Game initialization successful");
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnDeactivate() {
	CArea::AreaControl.OnCleanup();

	// Empty entities
	CFactory::Factory.OnCleanup();
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnLoop() {
	// Spawn new entities
	if( LevelInfoIndex < Level.size() && CCamera::CameraControl.GetX() >= Level.at(LevelInfoIndex).ActiveXPosition) {

		CAppStateGame::LevelInfo TmpInfo = Level.at(LevelInfoIndex);
		
		switch(TmpInfo.Type) {
			case ENTITY_TYPE_ENEMY:
				// Enemy Ship
				CFactory::Factory.CreateEnemyShip(ENITTY_SUBTYPE_ENEMY_1, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
				break;
			case ENTITY_TYPE_ITEM:
				// Item
				CFactory::Factory.CreateItem(ITEM_1, CCamera::CameraControl.GetX()+1000, TmpInfo.YPosition);
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
	float moveX = CAMERA_SPEED * CFPS::FPSControl.GetSpeedFactor();
	CCamera::CameraControl.OnMove(moveX, static_cast<float>(CCamera::CameraControl.GetY()));
	// Update BG offset
	BG_offset = static_cast<int>(BG_offset - BG_SPEED * CFPS::FPSControl.GetSpeedFactor());
	if( BG_offset <= 0 ) {
		BG_offset = BG_WIDTH;
	}

	// Player died -> Reset level
	if( Player->TookHit ) {
		ResetLevel();
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

	CSurface::OnDraw(Surf_Display, SpaceBG, BG_offset, GUI_HEIGHT);
	CSurface::OnDraw(Surf_Display, SpaceBG, BG_offset-BG_WIDTH, GUI_HEIGHT);
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
	// Player ChargeBar
	int BarStart = WWIDTH/2 - 50;
	SDL_Rect RectRed;
	RectRed.x = BarStart;
	RectRed.y = 30;
	RectRed.w = Player->ChargeLevel * 10;
	RectRed.h = 10;
	SDL_Rect RectGray;
	RectGray.x = BarStart;
	RectGray.y = 30;
	RectGray.w = 100;
	RectGray.h = 10;

	SDL_FillRect(Surf_Display, &RectGray, SDL_MapRGB(Surf_Display->format, 211, 211, 211));
	SDL_FillRect(Surf_Display, &RectRed, SDL_MapRGB(Surf_Display->format, 255, 0, 0));

	// Life icons
	for(unsigned int i = 0; i < Player->Lives; i++ ){
		CSurface::OnDraw(Surf_Display, IconLife, 50 + (i*30), 30);
	}

	//CFont::FontControl.Write(Surf_Display, "test", 50, 50);
}

//=============================================================================
CAppStateGame* CAppStateGame::GetInstance() {
	return &Instance;
}

//=============================================================================
void CAppStateGame::ResetLevel(){
	if( Player->Lives == 0 ) {
		NextState = APPSTATE_MAINMENU;
		CAppStateManager::SetActiveAppState(NextState);
	}
	else {
		Player->TookHit = false;

		Player->X = 400;
		Player->Y = 290+GUI_HEIGHT;
		Player->SpeedX = Player->SpeedY = Player->AccelX = Player->AccelY = 0;

		CCamera::CameraControl.SetPos(0,0);
		
		//Let's set spawner index back to the start
		LevelInfoIndex = 0;

		//And kill everything except player 
		CFactory::Factory.FlagNonPlayerEntities();
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

	file.close();
	return tmp;
}

//=============================================================================