//=============================================================================
#ifndef _CFACTORY_H_
#define _CFACTORY_H_

#include <vector>

#include "CPlayer.h"
#include "CEnemyShip.h"
#include "CItem.h"
#include "CBullet.h"
#include "CSpecialEffect.h"

//=============================================================================
enum ExplType {
	EXPLOSION_ENEMY = 0,
	EXPLOSION_TILE
};

enum {
	DURATION_500MS = 500,
	DURATION_1S = 1000,
	DURATION_2S = 2000
};

class CFactory {
	public:
		static CFactory Factory;

		int SlowMotionStartMoment;
		int SlowMotionDuration;
		
		int LastEnemyKillMoment;

    public:
        CFactory();

		bool OnInit();

        void OnLoop();

        void OnCleanup();

	public:
		// Flag non-player entities as dead
		void FlagNonPlayerEntities();
		
		// Create objects
		CPlayer* CreatePlayer(int nX, int nY);
		bool CreateEnemyShip(int type, int nX, int nY);
		bool CreateItem(int type, int nX, int nY);
		bool CreateBullet(int type, int nX, int nY);

		// Create effects
		bool CreateExplosion(int nX, int nY, ExplType explosion);

	private:
		std::vector<CPlayer*> Players;
};

//=============================================================================

#endif