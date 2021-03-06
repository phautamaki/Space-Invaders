//==============================================================================
#ifndef _CTILE_H_
	#define _CTILE_H_

#include "Define.h"

//==============================================================================
enum {
    TILE_TYPE_NONE = 0,
    TILE_TYPE_BLOCK,
	TILE_TYPE_BLOCK_BREAKABLE
};

//==============================================================================
class CTile {
    public:
        int     TileID;
        int     TypeID;

		int X;
		int Y;

		int oldTypeID;

	private:
		int HP;

    public:
        CTile();

		// Cause damage to tile
		void Damage(int amount);
		// Replace old HP value with nHP
		void SetHP(int nHP);
};

//==============================================================================
#endif
