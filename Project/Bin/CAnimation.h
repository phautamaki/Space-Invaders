//==============================================================================
// Animation Class
//==============================================================================
#ifndef _CANIMATION_H_
    #define _CANIMATION_H_

#include <SDL.h>
#include "CFPS.h"
#include "Define.h"

//==============================================================================
class CAnimation {
	private:
		int	CurrentFrame;
        int FrameRate; //Milliseconds
		long OldTime;

		void SetSMFrameRate();

	public:
        int MaxFrames;
		bool KeepAnimating;
		bool AnimateOnce;

		CAnimation();
		void OnAnimate();
        void SetFrameRate(int Rate);
		void SetCurrentFrame(int Frame);
		int GetCurrentFrame();
};

//==============================================================================

#endif
