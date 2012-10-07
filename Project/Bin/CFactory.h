//=============================================================================
#ifndef _CFACTORY_H_
    #define _CFACTORY_H_

#include <vector>

#include "CPlayer.h"
#include "CEnemyShip.h"
#include "CItem.h"

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
		bool CreatePlayer(CPlayer& player, int nX, int nY);
		bool CreateEnemyShip(int type, int nX, int nY);
		bool CreateItem(int type, int nX, int nY);

};

//=============================================================================

#endif