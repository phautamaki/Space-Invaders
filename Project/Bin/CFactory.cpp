//==============================================================================
#include "CFactory.h"
#include "Paths.h"
#include "functions.h"

//==============================================================================
CFactory CFactory::Factory;

//==============================================================================
CFactory::CFactory() {
}

//------------------------------------------------------------------------------
bool CFactory::OnInit() {

	return true;
}

//------------------------------------------------------------------------------
void CFactory::OnLoop() {
	std::vector<CEntity*>::iterator it = CEntity::EntityList.begin();
	while( it != CEntity::EntityList.end() ) {
		if( (*it)->Dead ) {
			(*it)->OnCleanup();
			delete (*it);
			(*it) = 0;
			it = CEntity::EntityList.erase(it);

			debug("Factory: Destroyed an object!");
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
}

//==============================================================================
bool CFactory::CreatePlayer(CPlayer& player, int nX, int nY) {
	if( !player.OnLoad( PATH_IMAGES FILENAME_PLAYER, 64, 64, 8) ){
		return false;
	}
	player.X = static_cast<float>(nX);
	player.Y = static_cast<float>(nY);

	CEntity::EntityList.push_back(&player);

	return true;
}

//------------------------------------------------------------------------------
bool CFactory::CreateEnemyShip(int type, int nX, int nY) {
	CEnemyShip* tmp = new CEnemyShip;
	if( type == SHIP_1 )
	{
		tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "ship1.png",34, 30, 1);
		tmp->X = static_cast<float>(nX);
		tmp->Y = static_cast<float>(nY);
	}
	else {
		return false;
	}

	CEntity::EntityList.push_back(tmp);

	return true;
}

//------------------------------------------------------------------------------
bool CFactory::CreateItem(int type, int nX, int nY) {
	CItem* tmp = new CItem;
	if( type == ITEM_1 )
	{
		tmp->OnLoad( PATH_IMAGES PATH_ITEMS "star.png",15, 16, 1);
		tmp->X = static_cast<float>(nX);
		tmp->Y = static_cast<float>(nY);
	}
	else {
		return false;
	}
	CEntity::EntityList.push_back(tmp);

	return true;
}

//==============================================================================
