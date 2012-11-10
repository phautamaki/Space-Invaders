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
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_SMALL_45U: 
			File = PATH_IMAGES PATH_ITEMS "bullet_small.png";
			Width = 10;
			Height = 7;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;
			SpeedY = -5;
			MoveUp = true;
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_SMALL_45D: 
			File = PATH_IMAGES PATH_ITEMS "bullet_small.png";
			Width = 10;
			Height = 7;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;
			SpeedY = 5;
			MoveDown = true;
			HP = 1;

			break;
		case ENTITY_SUBTYPE_BULLET_CHARGE1:
			File = PATH_IMAGES PATH_ITEMS "bullet_charge1.png";
			Width = 44;
			Height = 22;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_CHARGE1_SPEED;
			HP = 3;

			break;
		case ENTITY_SUBTYPE_BULLET_BEAM:
			File = PATH_IMAGES PATH_ITEMS "bullet_beam.png";
			Width = 15;
			Height = 4;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_BEAM_SPEED;
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
	CManouver* tmpMan = 0;
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_RIGHT);
	Manouvers.push_back(tmpMan);
	CurrentManouver = Manouvers.begin();
	
    return true;
}

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
			Dead = true;
			break;
		case ENTITY_TYPE_ENEMY: 
			HP--;
			if( HP <= 0 ){
				Entity->HP--;
			}
			break;
		case ENTITY_TYPE_ITEM:
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
			Dead = true;
			break;
		case TILE_TYPE_BLOCK_BREAKABLE:
			CFactory::Factory.CreateExplosion(Tile->X-8,Tile->Y-8, EXPLOSION_TILE);
			Tile->TypeID = TILE_TYPE_NONE;
			CArea::AreaControl.BrokenTiles.push_back(Tile);

			Dead = true;
			break;
		default:
			return;
			break;
	}
	return;
}

//=============================================================================