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
		int	    CurrentFrame;
		int     FrameInc;

        int     FrameRate; //Milliseconds
		long    OldTime;

		void	SetSMFrameRate();

	public:
        int	    MaxFrames;

		bool	Oscillate;

		unsigned int LoopCount;

	public:
		CAnimation();

		void OnAnimate();

	public:
        void SetFrameRate(int Rate);

		void SetCurrentFrame(int Frame);

		int GetCurrentFrame();
};

//==============================================================================

#endif
