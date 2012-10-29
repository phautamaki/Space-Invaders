//==============================================================================
// Animation Class
//==============================================================================
#ifndef _CANIMATION_H_
    #define _CANIMATION_H_

#include <SDL.h>

//==============================================================================
class CAnimation {
	private:
		int	    CurrentFrame;
		int     FrameInc;

        int     FrameRate; //Milliseconds
		long    OldTime;

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
