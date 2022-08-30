#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "raylib.h"
#include "raymath.h"

#define SPRITE_SIZE 64
#define ANIMATION_FPS 8.0f
#define TEXTURE_SCALE 1.5f

#define SCREEN_WIDTH 800
#define SCREEN_HEIGTH 900
#define MAX_DISTANCE SCREEN_WIDTH + SCREEN_HEIGTH

#define NUM_COGUMELOS 30
#define NUM_TEXTURES 2
#define NUM_ANIMATION_FRAMES 2

#define AIM_RADIUS SPRITE_SIZE*TEXTURE_SCALE/2
#define COGUMELO_COLLECT_DISTANCE SCREEN_WIDTH + SCREEN_HEIGTH

typedef enum RenderOrder {FAZENDEIRO_INDEX, COGUMELO_INDEX, NUM_RENDER_LAYERS}RENDERINDEX;
typedef enum RenderDirections {BAIXO, CIMA, ESQUERDA, DIREITA, NUM_DIRECOES}RENDERDIRECTION;
typedef enum CogumeloStates {ATIVO, INATIVO}COGUMELOSTATE;
typedef enum HitTypes {nothing = -1, cogumeloHit, milipedeHit, aranhaHit}HITTYPE;

typedef struct {
  Vector2 position;
  float speed;
  int direction;
  Vector2 aimDirection;
  char name[12];
  int score;
} FAZENDEIRO;


typedef struct {
  Vector2 position;
  int state;
} COGUMELO;

typedef struct {
  FAZENDEIRO fazendeiro;
  COGUMELO cogumelos[NUM_COGUMELOS];
  float currentTime;
  int currentAnimationFrame;
} GAMESTATE;

typedef struct {
  Vector2 movement;
  Vector2 mousePosition;
  int shooting;
} PLAYERINPUT;

typedef struct {
  float collisionDistance;
  int targetIndex;
  HITTYPE collisionType;
} RAYCOLLISION2D;


#endif