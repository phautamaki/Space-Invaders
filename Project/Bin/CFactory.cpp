//==============================================================================
#include "CFactory.h"
#include "Paths.h"
#include "functions.h"
#include <algorithm>
#include <cmath>
#include "CAppStateGame.h"

//==============================================================================
CFactory CFactory::Factory;

//==============================================================================
CFactory::CFactory() {
	SMStartMoment = 0;
	SMDuration = 0;
	SMLevel = LEVEL_NORMAL;
	LastEnemyKillMoment = 0;
}

//------------------------------------------------------------------------------
bool CFactory::OnInit() {
	// TODO: Popup should be somewhere else. Make a CUI or sth for it and other UI stuff
	Popup = CPopup();
	if( !Popup.OnLoad("Empty", 0, 0) ) {
		error("Couldn't create Popup-object.");
		return false;
	}
	Popup.Hide();

	return true;
}

//------------------------------------------------------------------------------
void CFactory::OnLoop() {
	if (SDL_GetTicks() > SMStartMoment + SMDuration) {
		CFPS::FPSControl.SetSlowMo(LEVEL_NORMAL);
	}
	else {
		CFPS::FPSControl.SetSlowMo(SMLevel);
	}
	
	Popup.OnLoop();

	std::vector<CEntity*>::iterator it = CEntity::EntityList.begin();
	while( it != CEntity::EntityList.end() ) {
		if( (*it)->IsDead() ) {

			// Create slow motion effect if 2 enemies die within
			// one second. TODO: make better way to create these,
			// because now even if player doesn't kill these enemies
			// the effect might go on if they die other ways (collision etc)
			if ((*it)->Type == ENTITY_TYPE_ENEMY) {
				/*
				if (SDL_GetTicks() - LastEnemyKillMoment < 1000) {
					CFPS::FPSControl.SetSlowMo(LEVEL_SLOWMO_8X);
					SlowMotionDuration = 2000; // <- effect lasts 2 seconds
					SlowMotionStartMoment = SDL_GetTicks();
				}
				LastEnemyKillMoment = SDL_GetTicks();
				*/
			}


			(*it)->OnCleanup();
			delete (*it);
			(*it) = 0;
			it = CEntity::EntityList.erase(it);
		}
		else {
			it++;
		}
	}
}

//------------------------------------------------------------------------------
void CFactory::OnCleanup() {
	Popup.OnCleanup();

	for( unsigned int i = 0; i < CEntity::EntityList.size(); i++ ) {
		CEntity::EntityList.at(i)->OnCleanup();
		delete CEntity::EntityList.at(i);
		CEntity::EntityList.at(i) = 0;
	}
	CEntity::EntityList.clear();

	Players.clear();
}

//==============================================================================
CPlayer* CFactory::GetPlayer(unsigned int PlayerNumber) const {
	if( PlayerNumber == 0 || PlayerNumber > Players.size() ) {
		return NULL;
	}
	else {
		return Players.at(PlayerNumber-1);
	}
}

//------------------------------------------------------------------------------
CEntity* CFactory::GetClosest(int X, int Y, int TargetType, bool Frontal, bool HorizontalSearch, int Offset ) const {
	CEntity* Closest = 0;
	int Distance = 0;
	for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {

		if( TargetType == -1 || CEntity::EntityList.at(i)->Type == TargetType ) {

			CEntity* Candidate = CEntity::EntityList.at(i);
			if( !Candidate->IsActive() ) continue;

			// In frontal search we skip objects that are left from X
			if( Frontal && Candidate->X < X ) {
				continue;			
			}
			// In horizontal search we skip objects farther than offset in Y direction

			if( HorizontalSearch ) {
				SDL_Rect Cbounds = Candidate->GetBounds();
				// Point is within height range
				if( ( Y+Offset < Cbounds.y ) || ( Y > Cbounds.y + Cbounds.h ) ) {
					continue;
				}
			}

			// Get first enemy
			if( Closest == 0 ) {
				Closest = Candidate;
				Distance = (int)sqrt( pow( X - Closest->X, 2 ) + pow( Y - Closest->Y, 2 ) );
			}
			// Calculate distance
			else if( Distance > sqrt( pow( X - Candidate->X, 2 ) + pow( Y - Candidate->Y, 2 ) ) ) {
					Closest = Candidate;
			}
		}
	}

	return Closest;
}
//------------------------------------------------------------------------------
void CFactory::FlagNonPlayerEntities() {
	for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
		if(!CEntity::EntityList[i] || CEntity::EntityList[i]->Type == ENTITY_TYPE_PLAYER) continue;

		CEntity::EntityList[i]->Kill();
    }
}

//------------------------------------------------------------------------------
CPlayer* CFactory::CreatePlayer(int nX, int nY) {
	CPlayer* tmp = new CPlayer;
	if( !tmp->OnLoad( PATH_IMAGES FILENAME_PLAYER, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, PLAYER_MAX_FRAMES) ){
		return false;
	}
	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY+GUI_HEIGHT);

	Players.push_back(tmp);
	CEntity::EntityList.push_back(tmp);

	return tmp;
}

//------------------------------------------------------------------------------
// TODO: Figure out a better way to do this
bool CFactory::CreateEnemy(int type, int nX, int nY) {
	CEnemy* tmp = NULL;
	
	switch( type ) {
		case ENTITY_SUBTYPE_ENEMY_1:
			tmp = new CEnemyShip;
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "ship1.png",ENEMY_SHIP_1_SPRITE_WIDTH, ENEMY_SHIP_1_SPRITE_HEIGHT, ENEMY_SHIP_1_MAX_FRAMES);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->SetHP(ENEMY_SHIP_1_HP);
			break;
		case ENTITY_SUBTYPE_ENEMY_BOSS_1:
			tmp = new CEnemyBoss(1);
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "boss_level1.png",128, 160, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->SetHP(ENEMY_BOSS_1_HP);
			break;
		case ENTITY_SUBTYPE_ENEMY_BOSS_1_LITTLE_BUBBLES:
			tmp = new CEnemyShip;
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "boss_level1_little_bubbles.png",32, 64, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY);
			tmp->SetHP(ENEMY_SHIP_1_HP*2);
			break;
		case ENTITY_SUBTYPE_ENEMY_METEORITE:
			tmp = new CEnemyMeteorite;
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "meteorite.png",128, 128, 12);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY);
			tmp->SetHP(1000000);
			break;
		case ENTITY_SUBTYPE_ENEMY_CANNON_1:
			tmp = new CEnemyCannon();
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "cannon_1.png",58, 56, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->SetHP(ENEMY_CANNON_HP);
			break;
		case ENTITY_SUBTYPE_ENEMY_CANNON_2:
			tmp = new CEnemyCannon();
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "cannon_2.png",58, 56, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->SetHP(ENEMY_CANNON_HP);
			tmp->OnRoof = true;
			break;
		case ENTITY_SUBTYPE_ENEMY_SHOOTING:
			tmp = new CEnemyShipShooting();
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "ship1.png",ENEMY_SHIP_1_SPRITE_WIDTH, ENEMY_SHIP_1_SPRITE_HEIGHT, ENEMY_SHIP_1_MAX_FRAMES);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->SetHP(ENEMY_SHIP_1_HP);
			break;
		case ENTITY_SUBTYPE_ENEMY_BOSS_2:
			tmp = new CEnemyBoss(2);
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "boss_level2.png",128, 160, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->SetHP(ENEMY_BOSS_2_HP);
			break;
		case ENTITY_SUBTYPE_ENEMY_BOSS_2_LITTLE_CLOUD:
			tmp = new CEnemyShip;
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "boss_level2_cloud_small.png",32, 64, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY);
			tmp->SetHP(ENEMY_SHIP_1_HP);
			break;
		case ENTITY_SUBTYPE_ENEMY_BOSS_2_MEDIUM_CLOUD:
			tmp = new CEnemyShip;
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "boss_level2_cloud_medium.png",64, 64, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY);
			tmp->SetHP(ENEMY_SHIP_1_HP*2);
			break;
		default:
			return false;
	}
	tmp->SubType = type;

	CEntity::EntityList.push_back(tmp);

	return true;
}


//------------------------------------------------------------------------------
bool CFactory::CreateRandomItem(int nX, int nY) {
	/* Algorithm based... not very random
	int RandomizedType = 0;
	static const int candidates[] = {
		ENTITY_SUBTYPE_ITEM_WPN_NORMAL,
		ENTITY_SUBTYPE_ITEM_WPN_BEAM,
		ENTITY_SUBTYPE_ITEM_WPN_MISSILE,
		ENTITY_SUBTYPE_ITEM_POINTS,
		ENTITY_SUBTYPE_ITEM_POINTS,
		ENTITY_SUBTYPE_ITEM_POINTS
	};
	std::vector<int> pool (candidates, candidates + sizeof(candidates) / sizeof(candidates[0]) );
	random_shuffle( pool.begin(), pool.end() );
	RandomizedType = pool.at(0);

	if( RandomizedType > 0 ) {
		CreateItem(RandomizedType,nX,nY);
		return true;
	}*/

	// Mod based... mucho better
	int randomValue = SDL_GetTicks() % 100;

	int selectedItem = -1;
	int level = CAppStateGame::Instance.CurrentLevelNumber;

	/* COMMON ITEMS FOR EVERY LEVEL */
	// 10 % chance for extra points
	if (randomValue >= 0 && randomValue <= 9) selectedItem = ENTITY_SUBTYPE_ITEM_POINTS;
	// 10 % chance for normal weapon
	else if (randomValue >= 10 && randomValue <= 19) selectedItem = ENTITY_SUBTYPE_ITEM_WPN_NORMAL;
	// 5 % chance for laser beam
	else if (randomValue >= 20 && randomValue <= 24) selectedItem = ENTITY_SUBTYPE_ITEM_WPN_BEAM;
	// 5 % chance for kill enemies item
	else if (randomValue >= 25 && randomValue <= 29) selectedItem = ENTITY_SUBTYPE_ITEM_KILL_ENEMIES;
	// 5 % chance for 1UP mushr...item
	else if (randomValue >= 30 && randomValue <= 34) selectedItem = ENTITY_SUBTYPE_ITEM_LIFEUP;
	
	// TODO: 2% chance for extra life
	//else if (randomValue >= 98 && randomValue <= 99) selectedItem = ENTITY_SUBTYPE_ITEM_EXTRA_LIFE;


	// In level 2 also MISSILE is possible weapon
	if (level == 2) {
		// 5 % chance for MISSILE
		if (randomValue >= 35 && randomValue <= 39) selectedItem = ENTITY_SUBTYPE_ITEM_WPN_MISSILE;
	}
	
	if (selectedItem != -1) {
		CreateItem(selectedItem,nX,nY);
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
bool CFactory::CreateItem(int type, int nX, int nY) {
	CItem* tmp = new CItem;

	switch( type ) {
		case ENTITY_SUBTYPE_ITEM_WPN_NORMAL:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS FILENAME_ITEM_WPN_NORMAL, 20, 21, 1);
			break;
		case ENTITY_SUBTYPE_ITEM_WPN_BEAM:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS FILENAME_ITEM_WPN_BEAM, 20, 21, 1);
			break;
		case ENTITY_SUBTYPE_ITEM_WPN_MISSILE:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS FILENAME_ITEM_WPN_MISSILES, 20, 21, 1);
			break;
		case ENTITY_SUBTYPE_ITEM_POINTS:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS FILENAME_ITEM_POINTS,13, 23, 7);
			break;
		case ENTITY_SUBTYPE_ITEM_KILL_ENEMIES:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS FILENAME_ITEM_KILL_ENEMIES,24, 22, 1);
			break;
		case ENTITY_SUBTYPE_ITEM_LIFEUP:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS FILENAME_ITEM_LIFEUP,20, 21, 1);
			break;
		default:
			delete tmp;
			return false;
	}
	tmp->SubType = type;
	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY);
	CEntity::EntityList.push_back(tmp);

	return true;
}

//------------------------------------------------------------------------------
CBullet* CFactory::CreateBullet(int type, int nX, int nY) {

	CBullet* tmp = new CBullet;
	bool EntityOK = true;

	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY);

	EntityOK = tmp->OnLoad( type );
	if( EntityOK ) {
		CEntity::EntityList.push_back(tmp);
	}

	return tmp;
}

//------------------------------------------------------------------------------
bool CFactory::CreateExplosion(int nX, int nY, ExplType explosion){
	CSpecialEffect* tmp = new CSpecialEffect;

	if (explosion == EXPLOSION_ENEMY) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "explosion.png",256, 150, 5)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}
	
	else if (explosion == EXPLOSION_ENEMY_BOSS_1) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "animations/red_explosion.png",120, 120, 12)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}
	else if (explosion == EXPLOSION_ENEMY_BOSS_2) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "animations/blue_explosion.png",120, 120, 12)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}
	/*
	// TODO: own explosion .png
	else if (explosion == EXPLOSION_TILE_2) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "breakable_tile_1_break.png",32, 32, 4)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}
	// TODO: own explosion .png
	else if (explosion == EXPLOSION_TILE_3) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "breakable_tile_1_break.png",32, 32, 4)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}
	// TODO: own explosion .png
	else if (explosion == EXPLOSION_TILE_DAMAGE) {
		//if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "breakable_tile_damage.png",16, 16, 3)){
		//	return false;
		//}
		//tmp->Anim_Control.AnimateOnce = true;
	}
	*/
	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
	CEntity::EntityList.push_back(tmp);
	return true;
}


//------------------------------------------------------------------------------
void CFactory::CreateSlowMotion(SlowMotionLevel level, int duration_ms) {
	SMStartMoment = SDL_GetTicks();
	SMDuration = duration_ms;
	SMLevel = level;
}


//------------------------------------------------------------------------------
void CFactory::KillEnemiesOnScreen() {
	for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
		if(!CEntity::EntityList[i] || CEntity::EntityList[i]->Type == ENTITY_TYPE_PLAYER) continue;
		else if (CEntity::EntityList[i]->Type != ENTITY_TYPE_ENEMY) continue;
		else if (CEntity::EntityList[i]->SubType == ENTITY_SUBTYPE_ENEMY_BOSS_1) continue;
		else if (CEntity::EntityList[i]->SubType == ENTITY_SUBTYPE_ENEMY_BOSS_2) continue;

		CEntity::EntityList[i]->Die();
		CEntity::EntityList[i]->Kill();
    }

	return;
}
//==============================================================================

void CFactory::CreateText(std::string& text, int durationMS, int x, int y) {
	Popup.ChangeText(text);
	Popup.SetVisibilityTime(durationMS);
	Popup.X = x;
	Popup.Y = y;
	Popup.Show();
}