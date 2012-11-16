//==============================================================================
#include "CFactory.h"
#include "Paths.h"
#include "functions.h"
#include <cmath>

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
		// Only check enemies
		if( TargetType == -1 || CEntity::EntityList.at(i)->Type == TargetType ) {
			CEntity* Candidate = CEntity::EntityList.at(i);
			// In frontal search we skip objects that are left from X
			if( Frontal && Candidate->X < X ) {
				continue;
			}
			// In horizontal search we skip objects farther than offset in Y direction
			/*if( HorizontalSearch && abs(Y - Candidate->Y) > Offset ) {
				continue;
			}*/
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
		case ENTITY_SUBTYPE_ENEMY_1:
			tmp->OnLoad( PATH_IMAGES PATH_ENEMIES "ship1.png",ENEMY_SHIP_1_SPRITE_WIDTH, ENEMY_SHIP_1_SPRITE_HEIGHT, ENEMY_SHIP_1_MAX_FRAMES);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			tmp->HP = ENEMY_SHIP_1_HP;
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
		case ENTITY_SUBTYPE_ITEM_1:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS "star.png",15, 16, 1);
			tmp->X = static_cast<float>(nX);
			tmp->Y = static_cast<float>(nY+GUI_HEIGHT);
			break;
		case ENTITY_SUBTYPE_ITEM_2:
			tmp->OnLoad( PATH_IMAGES PATH_ITEMS "special_item_1.png",16, 16, 1);
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
	CBullet* tmp = new CBullet;
	bool EntityOK = true;

	EntityOK = tmp->OnLoad( type );
	tmp->X = static_cast<float>(nX);
	tmp->Y = static_cast<float>(nY);
	
	CEntity::EntityList.push_back(tmp);

	return EntityOK;
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
	else if (explosion == EXPLOSION_TILE_1) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "breakable_tile_1_break.png",32, 32, 4)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}
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
	else if (explosion == EXPLOSION_TILE_DAMAGE) {
		if(!tmp->OnLoad( PATH_IMAGES PATH_SPECIALEFFECTS "breakable_tile_damage.png",16, 16, 3)){
			return false;
		}
		tmp->Anim_Control.AnimateOnce = true;
	}

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
void CFactory::FreezeEnemies(SlowMotionLevel level, int duration_ms) {
	return; // TODO: make
}

//------------------------------------------------------------------------------
// TODO: seems to be very unstable
void CFactory::KillEnemiesOnScreen() {
	std::vector<CEntity*>::iterator it = CEntity::EntityList.begin();
	while( it != CEntity::EntityList.end() ) {
		if ((*it) != NULL && (*it)->Type == ENTITY_TYPE_ENEMY && !((*it)->IsDead())) {
			int x = (int)((*it)->X);
			int y = (int)((*it)->Y);
			//CreateExplosion(x, y, EXPLOSION_ENEMY);
			(*it)->Kill();
		}
		++it;
	}

	return;
}
//==============================================================================
