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