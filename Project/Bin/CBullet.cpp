//=============================================================================
#include "CBullet.h"
#include "Paths.h"
#include "CFactory.h"

#include "functions.h"


//=============================================================================
CBullet::CBullet() {
}

//=============================================================================
bool CBullet::OnLoad(int nType) {
	char* File	  = "";
	int Width	  = 0;
	int Height	  = 0;
	int MaxFrames = 0;

	CManouver* tmpMan = 0;
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_RIGHT);
	Manouvers.push_back(tmpMan);
	CEntity* tmpEntity = 0;

	switch( nType ) {
		case ENTITY_SUBTYPE_BULLET_NORMAL: 
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_NORMAL;
			Width = 22;
			Height = 11;
			MaxFrames = 1;
			DamageOutput = BULLET_NORMAL_STR;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_SMALL_45U: 
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_SMALL;
			Width = 10;
			Height = 7;
			MaxFrames = 1;
			DamageOutput = BULLET_SMALL_STR;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;
			SpeedY = -5;
			MoveUp = true;
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_SMALL_45D: 
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_SMALL;
			Width = 10;
			Height = 7;
			MaxFrames = 1;
			DamageOutput = BULLET_SMALL_STR;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;
			SpeedY = 5;
			MoveDown = true;
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_CHARGE1:
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_CHARGE;
			Width = 54;
			Height = 17;
			MaxFrames = 3;
			DamageOutput = BULLET_CHARGE1_STR;

			MaxSpeedX = SpeedX = PLAYER_BULLET_CHARGE1_SPEED;
			HP = 3;

			break;
		case ENTITY_SUBTYPE_BULLET_BEAM:
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_BEAM_BODY;
			Width = 15;
			Height = 4;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_BEAM_SPEED;
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_MISSILE:
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_MISSILE;
			Width = 12;
			Height = 12;
			MaxFrames = 1;
			DamageOutput = BULLET_HOMING_STR;

			MaxSpeedX = SpeedX = PLAYER_BULLET_MISSILE_SPEED;
			HP = 1;

			tmpEntity = CFactory::Factory.GetClosest((int)X, (int)Y, ENTITY_TYPE_ENEMY);
	
			tmpMan = new CManouver(this);
			tmpMan->OnLoad(M_WAIT,200);
			Manouvers.push_back(tmpMan);

			tmpMan = new CManouver(this);
			tmpMan->OnLoad(M_FOLLOW);
			tmpMan->TargetX = tmpEntity != 0 ? (int)tmpEntity->X : -1;
			tmpMan->TargetY = tmpEntity != 0 ? (int)tmpEntity->Y : -1;
			Manouvers.push_back(tmpMan);

			break;
		case ENTITY_SUBTYPE_BULLET_ENEMY_MISSILE:
			debug("Entity enemy missile.");
			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_MISSILE;
			Width = 12;
			Height = 12;
			MaxFrames = 1;
			DamageOutput = BULLET_HOMING_STR;

			MaxSpeedX = SpeedX = PLAYER_BULLET_MISSILE_SPEED + 5;
			HP = 1;
	
			tmpMan = new CManouver(this);
			tmpMan->OnLoad(M_WAIT,200);
			Manouvers.push_back(tmpMan);

			tmpMan = new CManouver(this);
			tmpMan->TargetX = (int)CFactory::Factory.GetPlayer()->X;
			tmpMan->TargetY = (int)CFactory::Factory.GetPlayer()->Y;
			tmpMan->OnLoad(M_AIM);
			Manouvers.push_back(tmpMan);

			break;
		case ENTITY_SUBTYPE_BULLET_GENERIC:
			delete Manouvers.at(0);
			Manouvers.clear();
			tmpMan = new CManouver(this);
			tmpMan->OnLoad(M_MOVE_LEFT);
			Manouvers.push_back(tmpMan);

			File = PATH_IMAGES PATH_ITEMS FILENAME_BULLET_GENERIC;
			Width = 12;
			Height = 14;
			MaxFrames = 5;
			DamageOutput = 1;

			MaxSpeedX = SpeedX = MaxSpeedY = SpeedY = 0;
			HP = 1;

			break;
		default:
			return false;
	}

    if(CManouvarableEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

	Type	= ENTITY_TYPE_BULLET;
	SubType = nType;
	AccelX	= 1;

	Angle = 0;
	CurrentManouver = Manouvers.begin();
	
    return true;
}

void CBullet::OnLoop() {

	CManouvarableEntity::OnLoop();

	for(unsigned int i = 0; i < Manouvers.size(); i++) {
		if(Manouvers.at(i)->GetType() == M_FOLLOW) {

			CEntity* tmpEntity = CFactory::Factory.GetClosest((int)X, (int)Y, ENTITY_TYPE_ENEMY);
			Manouvers.at(i)->TargetX = tmpEntity != 0 ? (int)tmpEntity->X : -1;
			Manouvers.at(i)->TargetY = tmpEntity != 0 ? (int)tmpEntity->Y : -1;
		}
	}

	// Pick correct direction for missile
	if( SubType == ENTITY_SUBTYPE_BULLET_MISSILE || SubType == ENTITY_SUBTYPE_BULLET_ENEMY_MISSILE ) {
		int bAngle = Angle;//static_cast<int>(atan2(SpeedY,SpeedX) * (180.0/3.14159265));
		if( bAngle > -25 && bAngle <= 25 ) {
			CurrentFrameCol = 0;
		}
		else if( bAngle > 25 && bAngle <= 65 ) {
			CurrentFrameCol = 6;
		}
		else if( bAngle > 65 && bAngle <= 115 ) {
			CurrentFrameCol = 2;
		}
		else if( bAngle > 115 && bAngle <= 155 ) {
			CurrentFrameCol = 5;
		}
		else if( bAngle <= -25 && bAngle > -65 ) {
			CurrentFrameCol = 7;
		}
		else if( bAngle <= -65 && bAngle > -115 ) {
			CurrentFrameCol = 3;
		}
		else if( bAngle <= -115 && bAngle > -155 ) {
			CurrentFrameCol = 4;
		}
		else {
			CurrentFrameCol = 1;
		}
	}

}

//------------------------------------------------------------------------------
void CBullet::OnRender(SDL_Surface* Surf_Display) {

	if (Y > GUI_HEIGHT && 
		Y < WHEIGHT) {
		CEntity::OnRender(Surf_Display);
	}
	else {
		Die();
	}
}

//------------------------------------------------------------------------------
void CBullet::OnCollision(CEntity* Entity) {
	// Prevent multiple handlings for same collissions
	if( !IsActive() || !Entity->IsActive() ) return;

	switch(Entity->Type) {
		case ENTITY_TYPE_GENERIC:
			Die();
			break;
		case ENTITY_TYPE_ENEMY:
			if( SubType != ENTITY_SUBTYPE_BULLET_GENERIC ) {
				Damage(1);
				if( HP <= 0 ){
					Entity->Damage(DamageOutput);
				}
			}
			break;
		case ENTITY_TYPE_ITEM:
			break;
		case ENTITY_TYPE_PLAYER:
			if( SubType == ENTITY_SUBTYPE_BULLET_GENERIC ) {
				Entity->Damage(DamageOutput);
				Die();
			}
			break;
		default: 
			return;
	}

    return;
}

//------------------------------------------------------------------------------
void CBullet::OnCollision(CTile* Tile) {

	// Prevent multiple handlings for same collissions
	if( !IsActive() ) return;

	switch( Tile->TypeID ){
		case TILE_TYPE_BLOCK:
			Die();
			break;
		case TILE_TYPE_BLOCK_BREAKABLE:
			Tile->Damage(DamageOutput);

			Die();
			break;
		default:
			break;
	}
	return;
}

//=============================================================================