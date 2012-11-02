//=============================================================================
#include "CBullet.h"
#include "Paths.h"

#include "functions.h"
#include "CFactory.h"

//=============================================================================
CBullet::CBullet() {
}

//=============================================================================
bool CBullet::OnLoad(int nType) {
	char* File	  = "";
	int Width	  = 0;
	int Height	  = 0;
	int MaxFrames = 0;

	switch( nType ) {
		case ENTITY_SUBTYPE_BULLET_NORMAL: 
			File = PATH_IMAGES PATH_ITEMS "bullet_normal.png";
			Width = 22;
			Height = 11;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;

			break;
		case ENTITY_SUBTYPE_BULLET_CHARGE1:
			File = PATH_IMAGES PATH_ITEMS "bullet_charge1.png";
			Width = 44;
			Height = 22;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_CHARGE1_SPEED;

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
	CManouver* tmpMan = 0;
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_RIGHT);
	Manouvers.push_back(tmpMan);
	CurrentManouver = Manouvers.begin();
	
    return true;
}

//------------------------------------------------------------------------------
bool CBullet::OnCollision(CEntity* Entity) {

	// Prevent multiple handlings for same collissions
	if( Dead ) return false;

	int whatHitMe = Entity->Type;

	switch(whatHitMe) {
		case ENTITY_TYPE_GENERIC:
			Dead = true;
			break;
		case ENTITY_TYPE_ENEMY: 
			if (SubType == ENTITY_SUBTYPE_BULLET_NORMAL || SubType == ENTITY_SUBTYPE_NONE) {
				Dead = true;
			}
			
			break;
		case ENTITY_TYPE_ITEM:
			break;
		default: 
			break;
	}

    return true;
}

//------------------------------------------------------------------------------
bool CBullet::OnCollision(CTile* Tile) {
	bool PassThrough = false;

	// Prevent multiple handlings for same collissions
	if( Dead ) return false;

	switch( Tile->TypeID ){
		case TILE_TYPE_BLOCK:
			Dead = true;
			break;
		case TILE_TYPE_BLOCK_BREAKABLE:
			CFactory::Factory.CreateExplosion(Tile->X-8,Tile->Y-8, EXPLOSION_TILE);
			Tile->TypeID = TILE_TYPE_NONE;
			CArea::AreaControl.BrokenTiles.push_back(Tile);

			if (SubType == ENTITY_SUBTYPE_BULLET_NORMAL || SubType == ENTITY_SUBTYPE_NONE) {
				Dead = true;
			}
			break;
		default:
			PassThrough = true;
			break;
	}
	return PassThrough;
}

//=============================================================================