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

#define ARANHA_SPEED 5

#define MILIPEDE_SPEED 4
#define MILIPEDE_DESCENT_SPEED 2
#define MILIPEDE_FOLLOW_DISTANCE 46
#define MILIPEDE_DESCENT_FRAMES 25

#define NUM_COGUMELOS 60
#define NUM_ARANHAS 2
#define NUM_MAX_SEGMENTOS 10

#define NUM_TEXTURES 2
#define NUM_ANIMATION_FRAMES 2
#define STARTING_TIROS 100

#define AIM_RADIUS SPRITE_SIZE*TEXTURE_SCALE/2
#define COGUMELO_COLLECT_DISTANCE SCREEN_WIDTH + SCREEN_HEIGTH
#define PLAYER_NAME_SIZE 25
#define FIRING_DELAY 15

#define FAZENDEIRO_HITBOX_RADIUS 30
#define MILIPEDE_HITBOX_RADIUS 30
#define ARANHA_HITBOX_RADIUS 20
#define COGUMELO_HITBOX_RADIUS 50

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
  int firing_delay_frames;
  int score;
} FAZENDEIRO;

typedef struct {
  Vector2 position;
  int state;
} COGUMELO;

typedef struct {
  Vector2 position;
  Vector2 velocity;
  int state;
} ARANHA;

typedef struct {
  Vector2 position;
  int state;
} MILIPEDE_SEGMENT;

typedef struct {
  Vector2 position;
  int direction;
  int descendFrames;
  int state;
  MILIPEDE_SEGMENT segments[NUM_MAX_SEGMENTOS];
} MILIPEDE_HEAD;

typedef struct {
  FAZENDEIRO fazendeiro;
  COGUMELO cogumelos[NUM_COGUMELOS];
  ARANHA aranhas[NUM_ARANHAS];
  MILIPEDE_HEAD milipede;
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