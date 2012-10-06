//=============================================================================
#ifndef _CMANOUVER_H_
    #define _CMANOUVER_H_

//==============================================================================
// Turn left, turn right, speed up etc.
enum {
	M_START_MOVE = 0,
	M_STOP_MOVE,
	M_WAIT,
	M_TURN_LEFT,
	M_TURN_RIGHT
};

//=============================================================================
class CManouver {
    public:
        CManouver();

		bool OnLoad(int type);
		bool OnLoad(int type, int time);

		bool OnLoop(bool& moveleft, bool& moveright);

        void OnCleanup();

	private:
		int Type;	

		int StartTime;
		int Wait;
};

//=============================================================================

#endif