//=============================================================================
#ifndef _CGUN_H_
    #define _CGUN_H_

#include <SDL.h>

//=============================================================================
enum GunTypes {
	GUN_NORMAL = 0,
	GUN_BEAM,
	GUN_MISSILES
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
		unsigned int GetType() const;
		void Reset();

		void Activate();
		void Deactivate();

	private:
		void Shoot();
		void Charge();

		// Returns maximum beam width
		int UpdateBeamWidth(int StartX, int StartY);

		SDL_Surface* LaserBody;
		SDL_Surface* LaserEnd;

	public:
		int X;
		int Y;
		unsigned int ChargeLevel;

	private:
		unsigned int Type;
		unsigned int Level;

		unsigned int ChargeStart;
		unsigned int LastShot;
		unsigned int LastMissileShot;
		unsigned int MissileDelay;

		int  BeamChannel;
		bool BeamOn;
		int  BeamWidth;

};

//=============================================================================

#endif