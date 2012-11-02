//=============================================================================
#include "CFPS.h"
#include <string>
#include "functions.h"
#include <sstream>
//=============================================================================
CFPS CFPS::FPSControl;

//=============================================================================
CFPS::CFPS() {
	OldTime     = 0;
	LastTime    = 0;

	SpeedFactor	= 0;

	Frames      = 0;
	NumFrames   = 0;

	Level = LEVEL_NORMAL;
}

//=============================================================================
void CFPS::OnLoop() {
	if(OldTime + 1000 < SDL_GetTicks()) {
		OldTime = SDL_GetTicks();

		NumFrames = Frames;

		Frames = 0;
	}

	SpeedFactor = ((SDL_GetTicks() - LastTime) / 1000.0f) * 32.0f;
	
	if (Level == LEVEL_SLOWMO_2X) {
		SpeedFactor = SpeedFactor / 2.0f;
	}
	else if (Level == LEVEL_SLOWMO_4X) {
		SpeedFactor = SpeedFactor / 4.0f;
	}
	else if (Level == LEVEL_SLOWMO_8X) {
		SpeedFactor = SpeedFactor / 8.0f;
	}

	LastTime = SDL_GetTicks();

	Frames++;
}

//=============================================================================
int CFPS::GetFPS() {
    return NumFrames;
}

//------------------------------------------------------------------------------
float CFPS::GetSpeedFactor() {
    return SpeedFactor;
}

//==============================================================================

void CFPS::SetSlowMo(SlowMotionLevel level) {
	Level = level;
}