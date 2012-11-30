//=============================================================================
#ifndef _CGUN_H_
    #define _CGUN_H_

#include <SDL.h>

//=============================================================================
enum GunTypes {
	GUN_NORMAL = 0,
	GUN_BEAM,
	GUN_MISSILES,
	GUN_ENEMY_MISSILES
};

//=============================================================================
class CGun {
    public:
        CGun();

		bool OnLoad(bool Enemy = false);
        void OnLoop();
		void OnRender(SDL_Surface* Surf_Display);
		void OnCleanup();

		void ChangeType(int nType);
		unsigned int GetType() const;
		void Reset();

		void Activate();
		void Deactivate(int X = 0, int Y = 0);

	private:
		void Shoot(int GivenX = 0, int GivenY = 0);
		void Charge();

		// Returns maximum beam width
		int UpdateBeamWidth(int StartX, int StartY);

		SDL_Surface* LaserBody;
		SDL_Surface* LaserEnd;

		unsigned int LaserStartTime;
		unsigned int LaserInterval;

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

		bool Enemy;

};

//=============================================================================

#endif