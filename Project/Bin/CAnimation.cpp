//==============================================================================
#include "CAnimation.h"

//==============================================================================
CAnimation::CAnimation() {
    CurrentFrame    = 0;
    MaxFrames       = 0;
    FrameInc        = 1;

    FrameRate       = ANIMATION_FRAMERATE_MS; //Milliseconds
    OldTime         = 0;

    Oscillate       = false;

	LoopCount		= 0;
}

//------------------------------------------------------------------------------
void CAnimation::OnAnimate() {
	SetSMFrameRate();

    if(OldTime + FrameRate > SDL_GetTicks()) {
        return;
    }

    OldTime = SDL_GetTicks();

    CurrentFrame += FrameInc;

    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= MaxFrames - 1) {
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= MaxFrames - 1) {
            CurrentFrame = 0;
			LoopCount++;
        }
    }
}

//==============================================================================
void CAnimation::SetFrameRate(int Rate) {
    FrameRate = Rate;
}

//------------------------------------------------------------------------------
void CAnimation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame >= MaxFrames) return;

    CurrentFrame = Frame;
}

//------------------------------------------------------------------------------
int CAnimation::GetCurrentFrame() {
    return CurrentFrame;
}

//------------------------------------------------------------------------------

void CAnimation::SetSMFrameRate() {
	SlowMotionLevel level = CFPS::FPSControl.GetSMLevel();
	if (level == LEVEL_NORMAL) {
		FrameRate = ANIMATION_FRAMERATE_MS;
	}
	else if (level == LEVEL_SLOWMO_2X) {
		FrameRate = ANIMATION_FRAMERATE_MS * LEVEL_SLOWMO_2X;
	}
	else if (level == LEVEL_SLOWMO_4X) {
		FrameRate = ANIMATION_FRAMERATE_MS * LEVEL_SLOWMO_4X;
	}
	else if (level == LEVEL_SLOWMO_8X) {
		FrameRate = ANIMATION_FRAMERATE_MS * LEVEL_SLOWMO_8X;
	}
	else if (level == LEVEL_FREEZE) {
		FrameRate = ANIMATION_FRAMERATE_MS * LEVEL_FREEZE;
	}

	return;
}

//==============================================================================
