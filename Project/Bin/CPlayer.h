//=============================================================================
#ifndef _CPLAYER_H_
    #define _CPLAYER_H_

#include "CEntity.h"

//=============================================================================
class CPlayer : public CEntity {
    public:
        CPlayer();

		void StopMoveX();
		void StopMoveY();

		bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        void OnLoop();

        void OnRender(SDL_Surface* Surf_Display);

        void OnCleanup();

        void OnAnimate();

        bool OnCollision(CEntity* Entity);

		void PlaySoundFly();
	public:
		int SoundFly;

		bool MoveUp;
        bool MoveDown;
};

//=============================================================================

#endif


