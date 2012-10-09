//=============================================================================
#ifndef _CMANOUVER_H_
    #define _CMANOUVER_H_

#include "CEntity.h"

//==============================================================================
// Turn left, turn right, speed up etc.
enum {
	M_WAIT = 0,
	M_MOVE_LEFT,
	M_MOVE_RIGHT,
	M_MOVE_UP,
	M_MOVE_DOWN,
	M_STOP_MOVE,
	M_TURN
};

/* 
When turning:
0 = right
90 = down
-90 = up
180 & -180 = left

Always turn towards the smaller change
ie. from 90 to 180, not -180
*/

//=============================================================================
class CManouver {
    public:
        CManouver(CEntity* parent);

		bool OnLoad(int type);
		bool OnLoad(int type, int time);

		bool OnLoop();

        void OnCleanup();

	private:
		CEntity* Parent;

		int Type;	

		int StartTime;
		int Wait;

		int OriginalAngle;
};

//=============================================================================

#endif