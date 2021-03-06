//==============================================================================
#include "CAnimation.h"

//==============================================================================
CAnimation::CAnimation() {
    CurrentFrame    = -1;
    MaxFrames       = 0;

    FrameRate       = ANIMATION_FRAMERATE_MS; //Milliseconds
    OldTime         = 0;

	KeepAnimating = true;
	AnimateOnce = false;

}

//------------------------------------------------------------------------------
void CAnimation::OnAnimate() {
	SetSMFrameRate();

    if((unsigned int)(OldTime + FrameRate) > SDL_GetTicks()) {
        return;
    }

    OldTime = SDL_GetTicks();

	++CurrentFrame; // Get next frame

	if(CurrentFrame >= MaxFrames - 1) {
		CurrentFrame = 0;

		if (AnimateOnce) {
		KeepAnimating = false;
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
