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
#define LEVEL_LENGTH (BG_WIDTH*9)
//-----------------------------------------------------------------------------
/* Camera specific settings */
#define CAMERA_SPEED 5 // Camera speed in pixels per second

//-----------------------------------------------------------------------------
/* Common settings for all entities */
#define ENTITY_KILLDISTANCE 200 // Entity status is set to dead after camera gets this far past it

//-----------------------------------------------------------------------------
/* Player specific settings */
#define PLAYER_STARTING_LIVES	3
#define PLAYER_STARTING_X		100
#define PLAYER_STARTING_Y		240

#define PLAYER_ACCEL_LEFT	-5
#define PLAYER_ACCEL_RIGHT	 5
#define PLAYER_ACCEL_UP		-5
#define PLAYER_ACCEL_DOWN	 5

#define PLAYER_MAX_SPEED_X 10
#define PLAYER_MAX_SPEED_Y 10

#define PLAYER_SPRITE_WIDTH		52
#define PLAYER_SPRITE_HEIGHT	45
#define PLAYER_MAX_FRAMES		1

#define PLAYER_SHOOT_DELAY			300
#define PLAYER_SHOOT_DELAY_MISSILE	1200

#define PLAYER_BULLET_NORMAL_SPEED	25
#define PLAYER_BULLET_CHARGE1_SPEED	20
#define PLAYER_BULLET_BEAM_SPEED	3
#define PLAYER_BULLET_MISSILE_SPEED	9

//-----------------------------------------------------------------------------
/* Enemy specific settings */

// Small weak ship
#define ENEMY_SHIP_1_SPRITE_WIDTH	40
#define ENEMY_SHIP_1_SPRITE_HEIGHT	28
#define ENEMY_SHIP_1_MAX_FRAMES		1
#define ENEMY_SHIP_1_HP				100

// Cannon
#define ENEMY_CANNON_HP				1
#define ENEMY_CANNON_INTERVAL		300

// Boss
#define ENEMY_BOSS_1_HP				3000

//-----------------------------------------------------------------------------
/* Bullet strengths */
#define BULLET_NORMAL_STR	100
#define BULLET_SMALL_STR	50
#define BULLET_CHARGE1_STR	300
#define BULLET_BEAM_STR		3
#define BULLET_HOMING_STR	50


#define ANIMATION_FRAMERATE_MS 100 // 100 milliseconds per frame

//=============================================================================
#endif
