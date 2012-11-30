//==============================================================================
#include "CTile.h"
#include "CArea.h"
#include "CFactory.h"
#include "CSoundBank.h"

//==============================================================================
CTile::CTile() {
    TileID = 0;
    TypeID = TILE_TYPE_NONE;
	HP = 0;
}

//==============================================================================
void CTile::Damage(int amount) {
	if( TypeID == TILE_TYPE_BLOCK_BREAKABLE ){
		HP = HP - amount;
		if( HP <= 0 ) {
			ExplType explosionType;
			if (TileID == 1) explosionType = EXPLOSION_TILE_1;
			if (TileID == 2) explosionType = EXPLOSION_TILE_2;
			if (TileID == 3) explosionType = EXPLOSION_TILE_3;

			CFactory::Factory.CreateExplosion(X-8,Y-8, explosionType);
			CSoundBank::SoundControl.Play(CSoundBank::EFFECT, "TileBreakingSound");
			TypeID = TILE_TYPE_NONE;

			CFactory::Factory.GetPlayer()->Points = CFactory::Factory.GetPlayer()->Points + 5;
		}
		else {
			CFactory::Factory.CreateExplosion(X,Y, EXPLOSION_TILE_DAMAGE);
		}
	}
}

//------------------------------------------------------------------------------
void CTile::SetHP(int nHP) {
	if( nHP >= 0 ) {
		HP = nHP;
	}
}

//==============================================================================
