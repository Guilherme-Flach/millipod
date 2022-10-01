#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "raylib.h"
#include "raymath.h"

#define SPRITE_SIZE 64
#define ANIMATION_FPS 8.0f
#define TEXTURE_SCALE 1.5f

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGTH 1000
#define PLAYER_UPPER_BOUND 3 * SCREEN_HEIGTH / 4
#define MAX_DISTANCE SCREEN_WIDTH + SCREEN_HEIGTH

#define NUM_COGUMELOS 60
#define NUM_TEXTURES 2
#define NUM_ANIMATION_FRAMES 2
#define STARTING_TIROS 100

#define AIM_RADIUS SPRITE_SIZE*TEXTURE_SCALE/2
#define COGUMELO_COLLECT_DISTANCE SCREEN_WIDTH + SCREEN_HEIGTH
#define PLAYER_NAME_SIZE 25

typedef enum RenderOrder {FAZENDEIRO_INDEX, COGUMELO_INDEX, NUM_RENDER_LAYERS}RENDERINDEX;

typedef enum RenderDirections {BAIXO, CIMA, ESQUERDA, DIREITA, NUM_DIRECOES}RENDERDIRECTION;

typedef enum CogumeloStates {ATIVO, INATIVO}COGUMELOSTATE;

typedef enum HitTypes {nothing = -1, cogumeloHit, milipedeHit, aranhaHit}HITTYPE;

typedef enum GameStatus {STARTING, RUNNING, PAUSED, DISPLAYING_RANKING, ENDING}GAMESTATUS;

typedef enum FileOperations {LOAD, SAVE}FILEOPERATION;

typedef struct {
  Vector2 position;
  float speed;
  int direction;
  Vector2 aimDirection;
  char nome[PLAYER_NAME_SIZE];
  int vidas;
  int doente;
  int numTiros;
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
  GAMESTATUS gameStatus;
  int harvestedCogumelos;
  int remainingCogumelos;
  int editingTextBox;
} GAMESTATE;

typedef struct {
  Vector2 movement;
  Vector2 mousePosition;
  int shooting;
  int pauseButtonPressed;
  int rankingButtonPressed;
} PLAYERINPUT;

typedef struct {
  float collisionDistance;
  int targetIndex;
  HITTYPE collisionType;
} RAYCOLLISION2D;


#endif