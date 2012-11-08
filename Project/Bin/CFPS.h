//==============================================================================
// FPS Class
//==============================================================================
#ifndef _CFPS_H_
    #define _CFPS_H_

#include <SDL.h>

enum SlowMotionLevel {
	LEVEL_NORMAL = 0,
	LEVEL_SLOWMO_2X = 2,
	LEVEL_SLOWMO_4X = 4,
	LEVEL_SLOWMO_8X = 8,
	LEVEL_FREEZE = 10000
};

//==============================================================================
class CFPS {
	public:
		static CFPS FPSControl;

	private:
		int     OldTime;
		int     LastTime;

		float 	SpeedFactor;

		int		NumFrames;
		int     Frames;

		SlowMotionLevel Level;

	public:
		CFPS();

		void    OnLoop();

	public:
		int     GetFPS();

		float   GetSpeedFactor();

		void	SetSlowMo(SlowMotionLevel level);
		SlowMotionLevel GetSMLevel();
};

//==============================================================================
#endif
