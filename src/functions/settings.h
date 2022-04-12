#ifndef SETTINGS_H
#define SETTINGS_H

#define FULLSCREEN_ON false
#define MAX_FPS 150
#define SCREEN_WIDTH 850
#define SCREEN_HEIGHT 480

#define CENTER_X 0
#define CENTER_Y 0

// ---WORLD GENERATION---

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32

// World dimensions in blocks
#define WORLD_WIDTH 600
#define WORLD_HEIGHT 100
// 0- no cave
// 1- no blocks
// How wide is a cave
#define CAVE_HOLLOWNESS 0.4
// Shrinks the cave width but makes more of them
#define CAVE_DENSITY 0.08
// How frequent the terrain changes in height
#define TERRAIN_RUGGEDNESS 0.05
// How rapidly the terrain changes in height
#define TERRAIN_STEEPNESS 25
// How many block below top are dirt blocks
#define TERRAIN_DIRTLAYER 5
#define TERRAIN_HEIGHT 25

// ---PLAYER---
#define PLAYER_HITBOX_WIDTH 42
#define PLAYER_HITBOX_HEIGHT 60
#define PLAYER_WALK_ACCEL_RATE 20 
#define PLAYER_MAX_SPEED 30
#define PLAYER_JUMP_HEIGHT 45
#define PLAYER_JUMP_COOLDOWN 0.5
#define PLAYER_ATTACK_COOLDOWN 0.7
#define PLAYER_MAX_HEALTH 100


// ---WRAITH---
#define WRAITH_HITBOX_WIDTH 64
#define WRAITH_HITBOX_HEIGHT 64
#define WRAITH_WALK_ACCEL_RATE 10 
#define WRAITH_MAX_SPEED 25
#define WRAITH_JUMP_HEIGHT 35
#define WRAITH_JUMP_COOLDOWN 1
#define WRAITH_ATTACK_COOLDOWN 1
#define WRAITH_MAX_HEALTH 50

#define FRICTION_FACTOR 7
#define MIN_SPEED 0.3

#endif