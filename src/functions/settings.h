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

// ---BLOCKS---
#define BOX_HEALTH 5
#define DIRT_HEALTH 5
#define STONE_HEALTH 20
#define TRUNK_HEALTH 10

// ---PLAYER---
#define PLAYER_HITBOX_WIDTH 42
#define PLAYER_HITBOX_HEIGHT 60
#define PLAYER_WALK_ACCEL_RATE 20 
#define PLAYER_MAX_SPEED 30
#define PLAYER_JUMP_HEIGHT 45
#define PLAYER_JUMP_COOLDOWN 0.5
#define PLAYER_ATTACK_COOLDOWN 0.7
#define PLAYER_MAX_HEALTH 100
// animations:
#define PLAYER_IDLE_ANIM 4
#define PLAYER_WALK_ANIM 6 
#define PLAYER_JUMP_ANIM 4
#define PLAYER_FALL_ANIM 2 
#define PLAYER_ATTACK1_ANIM 5
#define PLAYER_HURT_ANIM 3
#define PLAYER_DIE_ANIM 7

// ---WRAITH---
#define WRAITH_HITBOX_WIDTH 64
#define WRAITH_HITBOX_HEIGHT 64
#define WRAITH_WALK_ACCEL_RATE 10 
#define WRAITH_MAX_SPEED 25
#define WRAITH_JUMP_HEIGHT 35
#define WRAITH_JUMP_COOLDOWN 1
#define WRAITH_ATTACK_COOLDOWN 1
#define WRAITH_MAX_HEALTH 50
// animations:
#define WRAITH_IDLE_ANIM 8
#define WRAITH_WALK_ANIM 0 
#define WRAITH_JUMP_ANIM 0
#define WRAITH_FALL_ANIM 0 
#define WRAITH_ATTACK1_ANIM 7
#define WRAITH_HURT_ANIM 4
#define WRAITH_DIE_ANIM 2

// ---PHYSICS---
#define FRICTION_FACTOR 7
#define MIN_SPEED 0.3

// GUI
#define HP_WIDTH 2
#define HP_HEIGHT 20

#endif