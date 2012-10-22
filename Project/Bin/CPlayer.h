//=============================================================================
#ifndef _CPLAYER_H_
    #define _CPLAYER_H_

#include "CEntity.h"
#include <SDL_mixer.h>

//=============================================================================
class CPlayer : public CEntity {
	// Custom
    public:
        CPlayer();

		void StopMoveX();
		void StopMoveY();

		void ChargeGun();
		void Shoot();

	// Events
	public:
		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        bool OnCollision(CEntity* Entity);

		bool OnCollision(CTile* Tile);

		void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

       	void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	public:
		int	ChargeLevel;

		bool TookHit;

	private:
		int ChargeStart;
		int LastShot;

		Mix_Chunk* ShootingSoundBasic;
		Mix_Chunk* ShootingSoundBig;
		
		Mix_Chunk* PlayerCrashingSound;
};

//=============================================================================

#endif


