//=============================================================================
#ifndef _CFACTORY_H_
    #define _CFACTORY_H_

#include <vector>

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
		CEnemyShip* CreateEnemyShip(int type, int nX, int nY);
		CItem* CreateItem(int type, int nX, int nY);

	private:
		std::vector< CEntity* > Entities;

};

//=============================================================================

#endif