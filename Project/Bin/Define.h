#ifndef _DEFINE_H_
    #define _DEFINE_H_

// Window size in pixels
#define WWIDTH		960
#define	WHEIGHT		480

#define MAP_WIDTH	60 // Number of tiles horizontally in a map
#define MAP_HEIGHT	30 // Number of tiles vertically in a map
#define TILE_SIZE	16 // Size of one tile in pixels

/* Common settings for all entities */
#define ENTITY_KILLDISTANCE 200 // Entity status is set to dead after camera gets this far past it

/* Player specific settings */
#define PLAYER_ACCEL_LEFT	-3
#define PLAYER_ACCEL_RIGHT	 3
#define PLAYER_ACCEL_UP		-3
#define PLAYER_ACCEL_DOWN	 3

/* Camera specific settings */
#define CAMERA_SPEED 2 // Camera speed in pixels per second

#endif
