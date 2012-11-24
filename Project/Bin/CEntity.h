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
enum Types {
	ENTITY_TYPE_GENERIC = 0,
	ENTITY_TYPE_PLAYER = 1,
	ENTITY_TYPE_ENEMY = 2,
	ENTITY_TYPE_ITEM = 3,
	ENTITY_TYPE_BULLET = 4,
	ENTITY_TYPE_SPECIAL_EFFECT = 5
};
enum SubTypes {
	ENTITY_SUBTYPE_NONE = 0,

	ENTITY_SUBTYPE_BULLET_NORMAL = 1,
	ENTITY_SUBTYPE_BULLET_SMALL_45U = 2,
	ENTITY_SUBTYPE_BULLET_SMALL_45D = 3,
	ENTITY_SUBTYPE_BULLET_CHARGE1 = 4,
	ENTITY_SUBTYPE_BULLET_BEAM = 5,
	ENTITY_SUBTYPE_BULLET_MISSILE = 6,

	ENTITY_SUBTYPE_ENEMY_1 = 100,
	ENTITY_SUBTYPE_ENEMY_BOSS_1 = 101,
	ENTITY_SUBTYPE_ENEMY_BOSS_1_LITTLE_BUBBLES = 102,
	ENTITY_SUBTYPE_ENEMY_METEORITE = 103,

	ENTITY_SUBTYPE_ITEM_WPN_NORMAL = 1000,
	ENTITY_SUBTYPE_ITEM_WPN_BEAM = 1001,
	ENTITY_SUBTYPE_ITEM_WPN_MISSILE = 1002,
	ENTITY_SUBTYPE_ITEM_POINTS = 1003

};

//==============================================================================
/* 
None = Regular
Ghost = Doesn't collide
*/
enum {
	ENTITY_FLAG_NONE 	= 0,
	ENTITY_FLAG_GHOST	= 1
};

//==============================================================================
class CEntity {

protected:
	
	SDL_Surface*    Surf_Entity;

public:
	static std::vector<CEntity*>    EntityList;

	CAnimation      Anim_Control;

	/* Movement and positioning */

	// Top left corner coordinate of the entity
	float X;
	float Y;

	bool MoveLeft;
	bool MoveRight;
	bool MoveUp;
	bool MoveDown;

	int Angle;
	int TargetAngle;

	/* Speed */
	float SpeedX;
	float SpeedY;

	float AccelX;
	float AccelY;

	float MaxSpeedX;
	float MaxSpeedY;

	/* Flags */
	int	Type;
	int	SubType;

	int	Flags;

public:
	CEntity();
	virtual ~CEntity();

	/* Event handlers */
	virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);
	virtual void OnLoop();
	virtual void OnRender(SDL_Surface* Surf_Display);
	virtual void OnCleanup();
	virtual void OnAnimate();
	virtual void OnCollision(CEntity* Entity);
	virtual void OnCollision(CTile* Tile);

	/* Movement */
	void OnMove(float MoveX, float MoveY);
	void StopMove();

	/* Collission */
	SDL_Rect GetBounds();
	static SDL_Rect Intersection(const SDL_Rect& boundsA, const SDL_Rect& boundsB);
	static bool CheckCollision(CEntity* entityA, CEntity* entityB);

	/* Misc */
	bool IsActive();
	virtual void Die();
	virtual bool IsDead();
	void Kill(); // Sets Dead = true
	void Damage(int Amount);
	void SetHP(int Amount);

protected:
	bool	Dead;
	int		HP;

	int	Width;
	int	Height;

	/* Animation */
	// Are these actually used?
	int	CurrentFrameCol;
	int	CurrentFrameRow; 

private:

	/* Collission */
	SDL_Rect GetFrameBounds();
	SDL_Rect NormalizeBounds(const SDL_Rect& rect);
	static bool GetAlphaXY(CEntity* entity, int x, int y);
	bool GetAlphaXYTile(SDL_Surface* tileset, int x, int y);
	bool Collides(int oX, int oY, int oW, int oH);
	void CheckCollisions(int NewX, int NewY);

	void QueuePossibleEntityCollision(CEntity* Entity, int NewX, int NewY);
	void CheckPossibleTileCollision(int NewX, int NewY);
	bool CheckTileCollision(CTile* tile, int tileX, int tileY);
};

//==============================================================================
class CEntityCol {
public:
	static std::vector<CEntityCol> EntityColList;

	CEntity* EntityA;
	CEntity* EntityB;

	CEntityCol();
};

//==============================================================================

#endif
