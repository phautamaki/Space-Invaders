//=============================================================================
#include "CAppStateGame.h"
#include "CFont.h"
#include "CFactory.h"
#include "functions.h"
#include "Paths.h"

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
	Player = CFactory::Factory.CreatePlayer(400, 240);

	// TODO: Player2 not needed yet. Make the game support co-op later

	// Enemy Ship
	CFactory::Factory.CreateEnemyShip(SHIP_1, 1000, 100);

	// Item
	CFactory::Factory.CreateItem(ITEM_1, 1000, 50);
	debug("All entities loaded successfully", 1);

	debug("Additional image loading stat", 1);
	if((SpaceBG = CSurface::OnLoad(PATH_IMAGES FILENAME_SPACE_BG_1)) == NULL) {
		return;
	}
	BG_offset = BG_WIDTH;
	debug("Additional images loaded successfully", 1);

	// Camera initialization, make it start from 0,0
	CCamera::CameraControl.TargetMode = TARGET_MODE_NORMAL;
	debug("Camera set", 1);

	debug("Game initialization successful");
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnDeactivate() {
	CArea::AreaControl.OnCleanup();

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    /*for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnCleanup();
    }

    CEntity::EntityList.clear();
	*/

	// Empty entities
	CFactory::Factory.OnCleanup();
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnLoop() {
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
				//debug("Collision");
				
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
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnRender(SDL_Surface* Surf_Display) {
	SDL_Rect Rect;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = WWIDTH;
	Rect.h = WHEIGHT;

	SDL_FillRect(Surf_Display, &Rect, 0);

	CSurface::OnDraw(Surf_Display, SpaceBG, BG_offset, 0);
	CSurface::OnDraw(Surf_Display, SpaceBG, BG_offset-BG_WIDTH, 0);
	CArea::AreaControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnRender(Surf_Display);
    }

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

	//CFont::FontControl.Write(Surf_Display, "test", 50, 50);
}

//=============================================================================
CAppStateGame* CAppStateGame::GetInstance() {
	return &Instance;
}

//=============================================================================
