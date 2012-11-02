//==============================================================================
#include "CFactory.h"
#include "Paths.h"

//==============================================================================
CFactory CFactory::Factory;

//==============================================================================
CFactory::CFactory() {
	SlowMotionStartMoment = 0;
	SlowMotionDuration = 0;
	LastEnemyKillMoment = 0;
}

//------------------------------------------------------------------------------
bool CFactory::OnInit() {
	return true;
}

//------------------------------------------------------------------------------
void CFactory::OnLoop() {
	
	if (SDL_GetTicks() > SlowMotionStartMoment + SlowMotionDuration) {
		CFPS::FPSControl.SetSlowMo(LEVEL_NORMAL);
	}
	

	std::vector<CEntity*>::iterator it = CEntity::EntityList.begin();
	while( it != CEntity::EntityList.end() ) {
		if( (*it)->Dead ) {

			// Create slow motion effect if 2 enemies die within
			// one second. TODO: make better way to create these,
			// because now even if player doesn't kill these enemies
			// the effect might go on if they die other ways (collision etc)
			if ((*it)->Type == ENTITY_TYPE_ENEMY) {
				if (SDL_GetTicks() - LastEnemyKillMoment < 1000) {
					CFPS::FPSControl.SetSlowMo(LEVEL_SLOWMO_8X);
					SlowMotionDuration = 2000; // <- effect lasts 2 seconds
					SlowMotionStartMoment = SDL_GetTicks();
				}
				LastEnemyKillMoment = SDL_GetTicks();
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
	for( unsigned int i = 0; i < CEntity::EntityList.size(); i++ ) {
		CEntity::EntityList.at(i)->OnCleanup();
		delete CEntity::EntityList.at(i);
		CEntity::EntityList.at(i) = 0;
	}
	CEntity::EntityList.clear();

	// TODO: Make factory keep player data while changing levels
	Players.clear();
}

//==============================================================================
void CFactory::FlagNonPlayerEntities() {
	for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
		if(!CEntity::EntityList[i] || CEntity::EntityList[i]->Type == ENTITY_TYPE_PLAYER) continue;

		CEntity::EntityList[i]->Dead = true;
    }
}

//------------------------------------------------------------------------------
// TODO: Store player in factory, so the player data can be saved between levels
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
// TODO: Could make a single function of these?
bool CFactory::CreateEnemyShip(int type, int nX, int nY) {
	CEnemyShip* tmp = new CEnemyShip;

	switch( type ) {
		case ENITTY_SUBTYPE_ENEMY_1:
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "ship1.png",ENEMY_SHIP_1_SPRITE_WIDTH, ENEMY_SHIP_1_SPRITE_HEIGHT, ENEMY_SHIP_1_MAX_FRAMES);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			break;
		default:
			return false;
	}

	CEntity::EntityList.push_back(tmp);

	return true;
}

//------------------------------------------------------------------------------
bool CFactory::CreateItem(int type, int nX, int nY) {
	CItem* tmp = new CItem;

	switch( type ) {
		case ITEM_1:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS "star.png",15, 16, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			break;
		default:
			return false;
	}
	CEntity::EntityList.push_back(tmp);

	return true;
}

//------------------------------------------------------------------------------
bool CFactory::CreateBullet(int type, int nX, int nY) {
	if( type != ENTITY_SUBTYPE_BULLET_NORMAL &&
		type != ENTITY_SUBTYPE_BULLET_CHARGE1 ) {
		return false;
	}

	CBullet* tmp = new CBullet;
	bool EntityOK = true;

	EntityOK = tmp->OnLoad( type );
	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY);
	
	CEntity::EntityList.push_back(tmp);

	return EntityOK;
}

//==============================================================================
bool CFactory::CreateExplosion(int nX, int nY, ExplType explosion){
	CSpecialEffect* tmp = new CSpecialEffect;

	if (explosion == EXPLOSION_ENEMY) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "explosion.png",256, 150, 5)){
			return false;
		}
	}
	else if (explosion == EXPLOSION_TILE) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "explosion_tile.png",32, 32, 4)){
			return false;
		}
	}

	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
	CEntity::EntityList.push_back(tmp);

	return true;
}

//==============================================================================
