#ifndef _DEFINE_H_
    #define _DEFINE_H_

//=============================================================================
// Window size in pixels
#define WWIDTH		960
#define	WHEIGHT		640

#define GUI_HEIGHT	160

#define FULLSCREEN_MODE false

//-----------------------------------------------------------------------------
/* Level specific settings */
#define MAP_WIDTH	30 // Number of tiles horizontally in a map
#define MAP_HEIGHT	15 // Number of tiles vertically in a map
#define TILE_SIZE	32 // Size of one tile in pixels

//-----------------------------------------------------------------------------
/* Background specific settings */
#define BG_SPEED 5
#define BG_WIDTH 960

//-----------------------------------------------------------------------------
/* Camera specific settings */
#define CAMERA_SPEED 5 // Camera speed in pixels per second

//-----------------------------------------------------------------------------
/* Common settings for all entities */
#define ENTITY_KILLDISTANCE 200 // Entity status is set to dead after camera gets this far past it

//-----------------------------------------------------------------------------
/* Player specific settings */
#define PLAYER_STARTING_LIVES 3

#define PLAYER_ACCEL_LEFT	-3
#define PLAYER_ACCEL_RIGHT	 3
#define PLAYER_ACCEL_UP		-3
#define PLAYER_ACCEL_DOWN	 3

#define PLAYER_MAX_SPEED_X 10
#define PLAYER_MAX_SPEED_Y 10

#define PLAYER_SPRITE_WIDTH		100
#define PLAYER_SPRITE_HEIGHT	64
#define PLAYER_MAX_FRAMES		1

#define PLAYER_SHOOT_DELAY			300
#define PLAYER_SHOOT_DELAY_MISSILE	1000

#define PLAYER_BULLET_NORMAL_SPEED	25
#define PLAYER_BULLET_CHARGE1_SPEED	20
#define PLAYER_BULLET_BEAM_SPEED	3
#define PLAYER_BULLET_MISSILE_SPEED	9

//-----------------------------------------------------------------------------
/* Enemy specific settings */

// Small weak ship
#define ENEMY_SHIP_1_SPRITE_WIDTH	33
#define ENEMY_SHIP_1_SPRITE_HEIGHT	39
#define ENEMY_SHIP_1_MAX_FRAMES		3
#define ENEMY_SHIP_1_HP				100

//-----------------------------------------------------------------------------
/* Bullet strengths */
#define BULLET_NORMAL_STR	100
#define BULLET_SMALL_STR	50
#define BULLET_CHARGE1_STR	300
#define BULLET_BEAM_STR		1
#define BULLET_HOMING_STR	100


#define ANIMATION_FRAMERATE_MS 100 // 100 milliseconds per frame

//=============================================================================
#endif
