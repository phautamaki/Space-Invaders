//=============================================================================
#ifndef __CAPPSTATE_H__
	#define __CAPPSTATE_H__

#include "CEvent.h"

//=============================================================================
class CAppState : public CEvent {
	protected:
		int	NextState;

	public:
		CAppState();

	public:
		virtual void OnActivate() = 0;

		virtual void OnDeactivate() = 0;

		virtual void OnLoop() = 0;

		virtual void OnRender(SDL_Surface* Surf_Display) = 0;
};

//=============================================================================

#endif
