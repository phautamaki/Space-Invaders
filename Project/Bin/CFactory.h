//=============================================================================
#ifndef _CFACTORY_H_
#define _CFACTORY_H_

#include <vector>

#include "CPlayer.h"
#include "CEnemyShip.h"
#include "CItem.h"
#include "CBullet.h"

//=============================================================================
class CFactory {
	public:
		static CFactory Factory;

    public:
        CFactory();

		bool OnInit();

        void OnLoop();

        void OnCleanup();

	public:
		CPlayer* CreatePlayer(int nX, int nY);
		bool CreateEnemyShip(int type, int nX, int nY);
		bool CreateItem(int type, int nX, int nY);
		bool CreateBullet(int type, int nX, int nY);

	private:
		std::vector<CPlayer*> Players;
};

//=============================================================================

#endif