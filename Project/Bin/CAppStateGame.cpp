//=============================================================================
#include "CAppStateGame.h"
#include "CFont.h"
#include "CFactory.h"
#include "functions.h"
#include "Paths.h"

//=============================================================================
CAppStateGame CAppStateGame::Instance;

//=============================================================================
CAppStateGame::CAppStateGame() {
}

//=============================================================================
void CAppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) {
		case SDLK_LEFT: {
			Player.MoveLeft = true;
			break;
		}

		case SDLK_RIGHT: {
			Player.MoveRight = true;
			break;
		}

		case SDLK_SPACE: {
		    Player.Jump();
		    break;
		}

		default: {
		}
	}
}

//------------------------------------------------------------------------------
void CAppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) {
		case SDLK_LEFT: {
			Player.MoveLeft = false;
			break;
		}

		case SDLK_RIGHT: {
			Player.MoveRight = false;
			break;
		}

		default: {
		}
	}
}

//=============================================================================
void CAppStateGame::OnActivate() {
	debug("Game initialization start");

	debug("Area loading start", 1);
	if(CArea::AreaControl.OnLoad(PATH_MAPS "1.area") == false) {
    	return;
    }
	debug("All areas loaded successfully", 1);

	debug("Entity loading start", 1);
	if(Player.OnLoad("yoshi.png", 64, 64, 8) == false) {
    	return;
    }

    if(Player2.OnLoad("yoshi.png", 64, 64, 8) == false) {
    	return;
    }
	Player2.X = 100;
	 
	CEntity::EntityList.push_back(&Player);
    CEntity::EntityList.push_back(&Player2);

	CEntity::EntityList.push_back(CFactory::Factory.CreateEnemyShip(SHIP_1, 1000, 100));
	debug("All entities loaded successfully", 1);

	CCamera::CameraControl.TargetMode = TARGET_MODE_CENTER;
    CCamera::CameraControl.SetTarget(&Player.X, &Player.Y);
	debug("Camera set", 1);

	debug("Game initialization successful");
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnDeactivate() {
	CArea::AreaControl.OnCleanup();

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnCleanup();
    }

    CEntity::EntityList.clear();
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

        if(EntityA->OnCollision(EntityB)) {
            EntityB->OnCollision(EntityA);
        }
    }

    CEntityCol::EntityColList.clear();
}

//-----------------------------------------------------------------------------
void CAppStateGame::OnRender(SDL_Surface* Surf_Display) {
	SDL_Rect Rect;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = WWIDTH;
	Rect.h = WHEIGHT;

	SDL_FillRect(Surf_Display, &Rect, 0);

	CArea::AreaControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnRender(Surf_Display);
    }

	CFont::FontControl.Write(Surf_Display, "test", 50, 50);
}

//=============================================================================
CAppStateGame* CAppStateGame::GetInstance() {
	return &Instance;
}

//=============================================================================
