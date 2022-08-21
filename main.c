#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
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

typedef enum RenderOrder {FAZENDEIRO_INDEX, COGUMELO_INDEX, NUM_RENDER_LAYERS}RenderIndex;
typedef enum RenderDirections {BAIXO, CIMA, ESQUERDA, DIREITA, NUM_DIRECOES}RenderDirection;
typedef enum CogumeloStates {ATIVO, COLHIDO}CogumeloState;
typedef enum HitTypes {NOTHING = -1, COGUMELO, MILIPEDE, ARANHA}HitType;

typedef struct {
  Vector2 position;
  float speed;
  int direction;
  Vector2 aimDirection;
  char name[12];
  int score;
} Fazendeiro;

typedef struct {
  Vector2 position;
  int state;
} Cogumelo;

typedef struct {
  Fazendeiro fazendeiro;
  Cogumelo cogumelos[NUM_COGUMELOS];
  float currentTime;
  int currentAnimationFrame;
} GameState;

typedef struct {
  Vector2 movement;
  Vector2 mousePosition;
  int shooting;
} PlayerInput;

typedef struct {
  float collisionDistance;
  int targetIndex;
  HitType collisionType;
} RayCollision2D;

// Gets info from the keyboard and updates a PlayerInput variable (used for movement, shooting, etc.)
void getInputFromKeyboard(PlayerInput *playerInput) {
  // Set the x part of the movement
  if (IsKeyDown(KEY_D)) playerInput->movement.x = 1.0f;
  else if (IsKeyDown(KEY_A)) playerInput->movement.x = -1.0f;
  else playerInput->movement.x = 0;

  // Set the y part of the movement
  if (IsKeyDown(KEY_W)) playerInput->movement.y = -1.0f;
  else if (IsKeyDown(KEY_S)) playerInput->movement.y = 1.0f;
  else playerInput->movement.y = 0;

  // Set the shooting variable
  playerInput->shooting = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

  // Set the aiming variable to the mouse position
  playerInput->mousePosition = GetMousePosition();
}

// Initializes the player
void initializeFazendeiro(Fazendeiro *fazendeiro, Vector2 position, char name[]) {
  fazendeiro->position = position;
  fazendeiro->speed = 5.0f;
  strcpy(fazendeiro->name, name);
  fazendeiro->score = 0;
}

// Uptades the player position
void updateFazendeiroPosition(Fazendeiro *fazendeiro, Vector2 movimento){
  fazendeiro->position.x += movimento.x * fazendeiro->speed;
  // Stop the player from going out of bounds horizontally
  if (fazendeiro->position.x + (SPRITE_SIZE * TEXTURE_SCALE)/2 > SCREEN_WIDTH)
  	fazendeiro->position.x = SCREEN_WIDTH - (SPRITE_SIZE * TEXTURE_SCALE)/2;
   else if (fazendeiro->position.x - (SPRITE_SIZE * TEXTURE_SCALE)/2 < 0)
  	fazendeiro->position.x = (SPRITE_SIZE * TEXTURE_SCALE)/2;

  fazendeiro->position.y += movimento.y * fazendeiro->speed;
  // Stop the player from going out of bounds vertically
  if (fazendeiro->position.y + (SPRITE_SIZE * TEXTURE_SCALE)/2 > SCREEN_HEIGTH)
  	fazendeiro->position.y = SCREEN_HEIGTH - (SPRITE_SIZE * TEXTURE_SCALE)/2;
   else if (fazendeiro->position.y - (SPRITE_SIZE * TEXTURE_SCALE)/2 < 0)
  	fazendeiro->position.y = (SPRITE_SIZE * TEXTURE_SCALE)/2;
}

void updateFazendeiroDirection(Fazendeiro *fazendeiro, Vector2 mousePosition) {
  // Get the vector representing the direction between the player and the mouse
  Vector2 direction = Vector2Subtract(mousePosition, fazendeiro->position);
  // Normalize the vector for future calculations
  direction = Vector2Normalize(direction);

  fazendeiro->aimDirection = direction;

  if (direction.y <= -PI/4)  fazendeiro->direction = CIMA;
  else if (direction.y >= PI/4) fazendeiro->direction = BAIXO;
  else if (direction.x <= -PI/4) fazendeiro->direction = ESQUERDA;
  else if (direction.x >= PI/4) fazendeiro->direction = DIREITA;
  
}


// Draws and animates the player
void drawFazendeiro(Fazendeiro fazendeiro, int currentFrame, Texture2D texture){
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  // Draws the player sprite based on its direction and animation frame
  animationRect.x = SPRITE_SIZE * fazendeiro.direction;

  // EX1 frame 0 of 1, up (1), -> get third sprite (2) (starts at 192px)
  // EX2 frame 1 of 1, down (0) -> get first sprite (0) (starts at 0px)
  // EX3 frame 1 of 1, left (2) -> get the sixth sprite (5) (starts at 384px) 

  animationRect.y = SPRITE_SIZE * currentFrame;
  animationRect.width = (float) SPRITE_SIZE;
  animationRect.height = (float) SPRITE_SIZE;

  destRect.x = fazendeiro.position.x;
  destRect.y = fazendeiro.position.y;
  destRect.width = SPRITE_SIZE * TEXTURE_SCALE;
  destRect.height = SPRITE_SIZE * TEXTURE_SCALE;

  // Shift the sprite by half its size as to draw it on its center
  originVector.x = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;
  originVector.y = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;

  DrawCircle(fazendeiro.position.x, fazendeiro.position.y, 32, BLUE);
  DrawTexturePro(texture,
                 animationRect,
                 destRect,
                 originVector,
                 0,
                WHITE);
  
};

// Initializes all mushrooms with a random position
void initializeCogumelos(Cogumelo cogumelos[], Rectangle coverArea) {
  int i, random_x, random_y;


  for (i = 0; i < NUM_COGUMELOS; i++) {
    // Generate a random Vector2 inside of the coverage area
    random_x = GetRandomValue(coverArea.x, coverArea.x + coverArea.width);
    random_y = GetRandomValue(coverArea.x, coverArea.x + coverArea.width);

    cogumelos[i].position.x = random_x;
    cogumelos[i].position.y = random_y;
    cogumelos[i].state = ATIVO;
  }
}

void drawCogumelos(Cogumelo cogumelos[], int currentFrame, Texture2D texture) {
  int i;
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  // Draws the mushroom sprite based on its animation frame
  animationRect.x = SPRITE_SIZE * currentFrame;
  animationRect.y = 0.0f;
  animationRect.width = (float) SPRITE_SIZE;
  animationRect.height = (float) SPRITE_SIZE;

  // Shift the sprite by half its size as to draw it on its center
  originVector.x = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;
  originVector.y = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;

  destRect.width = SPRITE_SIZE * TEXTURE_SCALE;
  destRect.height = SPRITE_SIZE * TEXTURE_SCALE;

  for (i = 0; i < NUM_COGUMELOS; i++) {
    if (cogumelos[i].state == ATIVO) {     
      destRect.x = cogumelos[i].position.x;
      destRect.y = cogumelos[i].position.y;
      
      DrawCircle(cogumelos[i].position.x, cogumelos[i].position.y, 32, GREEN);
      DrawTexturePro(texture,
                    animationRect,
                    destRect,
                    originVector,
                    0,
                    WHITE);

      
    }
  }
}

// Returns the index of the closest mushroom to the player that collides with the player
RayCollision2D collideCogumelos(Fazendeiro fazendeiro, Cogumelo cogumelos[]) {
  RayCollision2D returnCollision = {MAX_DISTANCE, -1, NOTHING};
  Vector2 fazendeiroAimMaxRange = Vector2Add(fazendeiro.position, Vector2Scale(fazendeiro.aimDirection, MAX_DISTANCE));
  float currentDistance,
        minDistance = MAX_DISTANCE;
  int i,
      returnIndex = -1;

  for (i = 0; i < NUM_COGUMELOS; i++) {
    // Check if mushroom is active and in the aim line
    if (cogumelos[i].state == ATIVO &&
        CheckCollisionPointLine(cogumelos[i].position, fazendeiro.position, fazendeiroAimMaxRange, AIM_RADIUS))
    {
      currentDistance = Vector2Distance(cogumelos[i].position, fazendeiro.position);
      // Check if it is closer than the current target
      if (currentDistance < minDistance) {
        // Update the return, minDistance and the closest target position
        returnIndex = i;
        minDistance = currentDistance;
        returnCollision.collisionType = COGUMELO;
      }
    }
  }

  returnCollision.collisionDistance = minDistance;
  returnCollision.targetIndex = returnIndex;

  return returnCollision;
}

void drawShot(Vector2 origin, Vector2 target) {
  DrawLineV(origin, target, MAGENTA);

}

void shoot(GameState *gameState) {
  RayCollision2D shotCollision;

  // Make a shot
  shotCollision = collideCogumelos(gameState->fazendeiro, gameState->cogumelos);

  // See if the shot actually hit something
  if (shotCollision.collisionType != NOTHING) {
    gameState->cogumelos[shotCollision.targetIndex].state = COLHIDO;
    gameState->fazendeiro.score += 100;
  }

  drawShot(gameState->fazendeiro.position, Vector2Add(gameState->fazendeiro.position, Vector2Scale(gameState->fazendeiro.aimDirection, MAX_DISTANCE)));
}

// Initializer all variables related to the game state
void initializeGameState(GameState *gameState) {
  Vector2 playerStartingPos = {200.0f, 200.0f};
  Rectangle cogumeloSpawnArea = {SPRITE_SIZE, SPRITE_SIZE, SCREEN_WIDTH - 2 * SPRITE_SIZE, SCREEN_HEIGTH - 2 * SPRITE_SIZE - 100};

  initializeFazendeiro(&gameState->fazendeiro, playerStartingPos, "Wanderley");
  initializeCogumelos(gameState->cogumelos, cogumeloSpawnArea);

  SetRandomSeed(time(0));
}

void initializeTextures (Texture2D textures[]) {
  textures[FAZENDEIRO_INDEX] = LoadTexture("./sprites/fazendeiro.png");
  textures[COGUMELO_INDEX] = LoadTexture("./sprites/cogumelo.png");
}

// Draws the game area
void drawGame(GameState gameState, Texture2D textures[]) {
  ClearBackground(DARKPURPLE);
  DrawText(TextFormat("Nome: %s | Pontuacao: %d", gameState.fazendeiro.name, gameState.fazendeiro.score), 0, 0, 40, WHITE);
  drawCogumelos(gameState.cogumelos, gameState.currentAnimationFrame, textures[COGUMELO_INDEX]);
  drawFazendeiro(gameState.fazendeiro, gameState.currentAnimationFrame, textures[FAZENDEIRO_INDEX]);    
}

void updateFrameCount(GameState *gameState)
{
	  // Update
        gameState->currentTime += GetFrameTime();
        // update current animation frames every 1/fps seconds
        if (gameState->currentTime > 1 / ANIMATION_FPS) {
          if (gameState->currentAnimationFrame < NUM_ANIMATION_FRAMES-1)
            gameState->currentAnimationFrame++;
          else gameState->currentAnimationFrame = 0;

          gameState->currentTime = 0;
        }

}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    PlayerInput playerInput = {{0, 0}, 0};
    Texture2D textures[NUM_TEXTURES];
    GameState currentGameState;
    
    // Init window first, otherwise we get a segfault for trying to load textures before the GPU is ready
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Mili-pede");

    // Initialize variables
    initializeTextures(textures);
    initializeGameState(&currentGameState);

    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
     	updateFrameCount(&currentGameState);   
      getInputFromKeyboard(&playerInput);
      updateFazendeiroPosition(&(currentGameState.fazendeiro), playerInput.movement);
      updateFazendeiroDirection(&(currentGameState.fazendeiro), playerInput.mousePosition);
      BeginDrawing();
      if (playerInput.shooting) {
        shoot(&currentGameState);
      }
      
      // Draw
      drawGame(currentGameState, textures);
      EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}