//=============================================================================
#ifndef _CGUN_H_
    #define _CGUN_H_

#include <SDL.h>

//=============================================================================
enum GunTypes {
	GUN_NORMAL = 0,
	GUN_BEAM
};

//=============================================================================
class CGun {
    public:
        CGun();

		bool OnLoad();
        void OnLoop();
		void OnRender(SDL_Surface* Surf_Display);
		void OnCleanup();

		void ChangeType(int nType);
		void Reset();

		void Activate();
		void Deactivate();

	private:
		void Shoot();
		void Charge();

	public:
		int X;
		int Y;

	private:
		unsigned int Type;
		unsigned int Level;

		unsigned int ChargeStart;
		unsigned int ChargeLevel;
		unsigned int LastShot;

		bool		 BeamOn;
};

//=============================================================================

#endif