//==============================================================================
#include "CFactory.h"
#include "Paths.h"

//==============================================================================
CFactory CFactory::Factory;

//==============================================================================
CFactory::CFactory() {
}

//------------------------------------------------------------------------------
bool CFactory::OnLoad() {
}

//------------------------------------------------------------------------------
void CFactory::OnLoop() {
	std::vector<CEntity*>::iterator it = Entities.begin();
	while( it != Entities.end() ) {
		if( (*it)->Dead ) {
			(*it)->OnCleanup();
			delete (*it);
			(*it) = 0;
			it = Entities.erase(it);
		}
		else {
			it++;
		}
	}
}

//------------------------------------------------------------------------------
void CFactory::OnCleanup() {
	for( unsigned int i = 0; i < Entities.size(); i++ ) {
		Entities.at(i)->OnCleanup();
		delete Entities.at(i);
		Entities.at(i) = 0;
	}
	Entities.clear();
}

//==============================================================================
CEnemyShip* CFactory::CreateEnemyShip(int type, int nX, int nY) {
	CEnemyShip* tmp = new CEnemyShip;
	if( type == SHIP_1 )
	{
		tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "ship1.png",34, 30, 1);
		tmp->X = static_cast<float>(nX);
		tmp->Y = static_cast<float>(nY);
	}
	else {
		return 0;
	}

	Entities.push_back(tmp);
	return tmp;
}

//==============================================================================
