//=============================================================================
#include "CBullet.h"
#include "Paths.h"

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
		case ENTITY_TYPE_BULLET_NORMAL: 
			File = PATH_IMAGES PATH_ITEMS "bullet_normal.png";
			Width = 22;
			Height = 11;
			MaxFrames = 1;

			MaxSpeedX = SpeedX = PLAYER_BULLET_NORMAL_SPEED;

			break;
		case ENTITY_TYPE_BULLET_CHARGE1:
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

	Type = 	nType;
	AccelX = 1;

	Angle = 0;
	CManouver* tmpMan = 0;
	tmpMan = new CManouver(this);
	tmpMan->OnLoad(M_MOVE_RIGHT);
	Manouvers.push_back(tmpMan);
	CurrentManouver = Manouvers.begin();
	
    return true;
}

//=============================================================================