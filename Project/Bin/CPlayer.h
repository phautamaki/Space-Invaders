//=============================================================================
#ifndef _CPLAYER_H_
    #define _CPLAYER_H_

#include "CEntity.h"
#include "CGun.h"

//=============================================================================
class CPlayer : public CEntity {
	// Custom
    public:
        CPlayer();

		void StopMoveX();
		void StopMoveY();

		bool			MakeDeathScene;
		int				DeathMoment;

	// Events
		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        void OnCollision(CEntity* Entity);

		void OnCollision(CTile* Tile);

		void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

       	void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	private:
		void Die();

	public:
		unsigned int Lives;

		unsigned int Points;

		bool TookHit;

	private:
		CGun	Gun;
};

//=============================================================================

#endif


