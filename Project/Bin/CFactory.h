//=============================================================================
#ifndef _CFACTORY_H_
#define _CFACTORY_H_

#include <vector>

#include "CPlayer.h"
#include "CEnemyShip.h"
#include "CItem.h"
#include "CBullet.h"
#include "CSpecialEffect.h"
#include "CPopup.h"

//=============================================================================
enum ExplType {			// TODO: move this code to SpecialEffect.h
	EXPLOSION_ENEMY = 0,
	EXPLOSION_TILE_1,
	EXPLOSION_TILE_2,
	EXPLOSION_TILE_3,
	EXPLOSION_TILE_DAMAGE
};

class CFactory {
	private:
		unsigned int SMStartMoment;
		unsigned int SMDuration;
		SlowMotionLevel SMLevel;

		unsigned int LastEnemyKillMoment;
		std::vector<CPlayer*> Players;

	public:
		CPopup Popup;

	public:

		static CFactory Factory;
		
        CFactory();

		bool OnInit();

        void OnLoop();

        void OnCleanup();

		CPlayer* GetPlayer(unsigned int PlayerNumber = 1) const;

		/* Parameters:
		X = Searcher's X coordinate
		Y = Searcher's Y coordinate
		TargetType = Entity type of object to be searched (-1 = any)
		Frontal = Ignore Entities behind X
		HorizontalSearch = Limit search in Y direction by Offset
		Offset = Limits search +/- from Y
		*/
		CEntity* GetClosest(int X, int Y, int TargetType = -1, bool Frontal = false, bool HorizontalSearch = false, int Offset = 0) const;

		// Flag non-player entities as dead
		void FlagNonPlayerEntities();
		
		// Create objects
		CPlayer* CreatePlayer(int nX, int nY);
		bool CreateEnemyShip(int type, int nX, int nY);
		bool CreateRandomItem(int nX, int nY); 
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