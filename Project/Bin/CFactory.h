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
	EXPLOSION_TILE_1,
	EXPLOSION_TILE_2,
	EXPLOSION_TILE_3
};



class CFactory {
	private:
		unsigned int SMStartMoment;
		unsigned int SMDuration;
		SlowMotionLevel SMLevel;

		unsigned int LastEnemyKillMoment;
		std::vector<CPlayer*> Players;

	public:

		static CFactory Factory;
		
        CFactory();

		bool OnInit();

        void OnLoop();

        void OnCleanup();

		CPlayer* GetPlayer(unsigned int PlayerNumber = 1) const;

		// Flag non-player entities as dead
		void FlagNonPlayerEntities();
		
		// Create objects
		CPlayer* CreatePlayer(int nX, int nY);
		bool CreateEnemyShip(int type, int nX, int nY);
		bool CreateItem(int type, int nX, int nY);
		bool CreateBullet(int type, int nX, int nY);

		// Create effects
		bool CreateExplosion(int nX, int nY, ExplType explosion);
		void CreateSlowMotion(SlowMotionLevel level, int duration_ms);

		// Create special effects (acquired from s-box)
		void FreezeEnemies(SlowMotionLevel level, int duration_ms);
		void KillEnemiesOnScreen();

};

//=============================================================================

#endif