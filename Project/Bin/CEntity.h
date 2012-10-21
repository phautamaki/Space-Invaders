//==============================================================================
#ifndef _CENTITY_H_
    #define _CENTITY_H_

#include <vector>

#include "CArea.h"
#include "CAnimation.h"
#include "CCamera.h"
#include "CFPS.h"
#include "CSurface.h"

//==============================================================================
// All the different types should be added here
// Should perhaps separate different enemy types and items?
enum Types {
	ENTITY_TYPE_GENERIC = 0,
	ENTITY_TYPE_PLAYER,
	ENTITY_TYPE_ENEMY,
	ENTITY_TYPE_ITEM,
	ENTITY_TYPE_BULLET_NORMAL,
	ENTITY_TYPE_BULLET_CHARGE1
};

//==============================================================================
/* 
None = Regular
Gravity = Affected by gravity.. duh
Ghost = Doesn't collide
MapOnly = Doesn't move
*/
enum {
	ENTITY_FLAG_NONE 	= 0,

	ENTITY_FLAG_GRAVITY	= 0x00000001,
	ENTITY_FLAG_GHOST	= 0x00000002,
	ENTITY_FLAG_MAPONLY	= 0x00000004
};

//==============================================================================
class CEntity {
	public:
		static std::vector<CEntity*>    EntityList;

	protected:
		CAnimation      Anim_Control;

		SDL_Surface*    Surf_Entity;

	public:
		float	X;
		float	Y;
		float	originX;
		float	originY;

		int		Width;
		int		Height;

		bool	MoveLeft;
		bool	MoveRight;
		bool	MoveUp;
        bool	MoveDown;

		int		Angle;
		int		TargetAngle;

	public:
		int		Type;

		bool	Dead;
		int		Flags;

	public:
		float	SpeedX;
		float	SpeedY;

		float	AccelX;
		float	AccelY;

	public:
		float	MaxSpeedX;
		float	MaxSpeedY;

	protected:
		int		CurrentFrameCol;
		int		CurrentFrameRow;

	public:
		CEntity();

		virtual ~CEntity();

	public:
		virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

		virtual void OnLoop();

		virtual void OnRender(SDL_Surface* Surf_Display);

		virtual void OnCleanup();

		virtual void OnAnimate();

		virtual bool OnCollision(CEntity* Entity);

	public:
		void    OnMove(float MoveX, float MoveY);

		void 	StopMove();

	public:
		SDL_Rect GetBounds();
		SDL_Rect GetFrameBounds();
		SDL_Rect NormalizeBounds(const SDL_Rect& rect);
        static SDL_Rect Intersection(const SDL_Rect& boundsA, const SDL_Rect& boundsB);
        static bool CheckCollision(CEntity* entityA, CEntity* entityB);
        static bool GetAlphaXY(CEntity* entity, int x, int y);

		bool    Collides(int oX, int oY, int oW, int oH);

	private:
		bool 	PosValid(int NewX, int NewY);

		bool 	PosValidTile(CTile* Tile);

		bool 	PosValidEntity(CEntity* Entity, int NewX, int NewY);
};

//==============================================================================
class CEntityCol {
    public:
        static std::vector<CEntityCol>	EntityColList;

	public:
		CEntity* EntityA;
		CEntity* EntityB;

	public:
		CEntityCol();
};

//==============================================================================

#endif
