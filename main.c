#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#define SPRITE_SIZE 64
#define ANIMATION_FPS 8.0f
#define TEXTURE_SCALE 1.5f

#define SCREEN_WIDTH 800
#define SCREEN_HEIGTH 900

#define NUM_COGUMELOS 40
#define NUM_TEXTURES 2
#define NUM_ANIMATION_FRAMES 2

typedef enum TextureIndex {fazendeiroTexture, cogumeloTexture};
typedef enum Direction {baixo, cima, esquerda, direita};
typedef enum CogumeloEstados {ativo, colhido, destruido};

typedef struct {
  Vector2 position;
  float speed;
  int direction;
  char nome[12];
  int pontuacao;
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
  int shooting;
} KeyboardInput;


// Gets info from the keyboard and updates a KeyboardInput variable (used for movement, shooting, etc.)
void getInputFromKeyboard(KeyboardInput *keyboardInput) {
  // Set the x part of the movement
  if (IsKeyDown(KEY_RIGHT)) keyboardInput->movement.x = 1.0f;
  else if (IsKeyDown(KEY_LEFT)) keyboardInput->movement.x = -1.0f;
  else keyboardInput->movement.x = 0;

  // Set the y part of the movement
  if (IsKeyDown(KEY_UP)) keyboardInput->movement.y = -1.0f;
  else if (IsKeyDown(KEY_DOWN)) keyboardInput->movement.y = 1.0f;
  else keyboardInput->movement.y = 0;

  // Set the shooting variable
  keyboardInput->shooting = IsKeyDown(KEY_Z);
}

// Initializes the player
Fazendeiro initializeFazendeiro(Vector2 position, float speed, char nome[]) {
  Fazendeiro fazendeiro;
  fazendeiro.position = position;
  fazendeiro.speed = speed;
  strcpy(fazendeiro.nome, nome);
  fazendeiro.pontuacao = 0;
  return fazendeiro;
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

  // Update the player direction based on movment; 
  if (movimento.y > 0) fazendeiro->direction = baixo;
  else if (movimento.y < 0) fazendeiro->direction = cima;
  else if (movimento.x > 0) fazendeiro->direction = direita;
  else if (movimento.x < 0) fazendeiro->direction = esquerda;
}


// Draws and animates the player
void drawFazendeiro(Fazendeiro fazendeiro, int currentFrame, Texture2D texture){
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  // Draws the player sprite based on its direction and animation frame
  animationRect.x = SPRITE_SIZE *
                    (currentFrame + (fazendeiro.direction *
                                     NUM_ANIMATION_FRAMES));

  // EX1 frame 0 of 1, up (1), -> get third sprite (2) (starts at 192px)
  // EX2 frame 1 of 1, down (0) -> get first sprite (0) (starts at 0px)
  // EX3 frame 1 of 1, left (2) -> get the sixth sprite (5) (starts at 384px) 

  animationRect.y = 0.0f;
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

// Generates a random integer between the lower and upper bounds  (inclusive)
int generateRandom(int lowerBound, int upperBound) {
    return lowerBound + (int) (rand() / (double) RAND_MAX * (upperBound - lowerBound + 1));
}

// Initializes all mushrooms with a random position
void initializeCogumelos(Cogumelo cogumelos[], Rectangle coverArea) {
  int i, random_x, random_y;


  for (i = 0; i < NUM_COGUMELOS; i++) {
    // Generate a random Vector2 inside of the coverage area
    random_x = generateRandom(coverArea.x, coverArea.x + coverArea.width);
    random_y = generateRandom(coverArea.x, coverArea.x + coverArea.width);

    cogumelos[i].position.x = random_x;
    cogumelos[i].position.y = random_y;
    cogumelos[i].state = ativo;
  }

}

void drawCogumelos(Cogumelo cogumelos[], int currentFrame, Texture2D texture) {
  int i;
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  // Draws the mushroom sprite based on its animation frame
  animationRect.x = SPRITE_SIZE/2 * currentFrame;
  animationRect.y = 0.0f;
  animationRect.width = (float) SPRITE_SIZE/2;
  animationRect.height = (float) SPRITE_SIZE/2;

  // Shift the sprite by half its size as to draw it on its center
  originVector.x = (float) SPRITE_SIZE * TEXTURE_SCALE / 4.0f;
  originVector.y = (float) SPRITE_SIZE * TEXTURE_SCALE / 4.0f;

  destRect.width = SPRITE_SIZE * TEXTURE_SCALE / 2;
  destRect.height = SPRITE_SIZE * TEXTURE_SCALE / 2;

  for (i = 0; i < NUM_COGUMELOS; i++) {
    if (cogumelos[i].state == ativo) {     
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

// Initializer all variables related to the game state
void initializeGameState(GameState *gameState) {
  Vector2 playerStartingPos = {200.0f, 200.0f};
  Rectangle cogumeloSpawnArea = {SPRITE_SIZE, SPRITE_SIZE, SCREEN_WIDTH - 2 * SPRITE_SIZE, SCREEN_HEIGTH - 2 * SPRITE_SIZE};

  gameState->fazendeiro = initializeFazendeiro(playerStartingPos, 2.0, "Wanderley");
  initializeCogumelos(gameState->cogumelos, cogumeloSpawnArea);

  srand(time(0));
}

void initializeTextures (Texture2D textures[]) {
  textures[fazendeiroTexture] = LoadTexture("./sprites/fazendeiro.png");
  textures[cogumeloTexture] = LoadTexture("./sprites/cogumelo.png");
}

// Draws the game area
void drawGame(GameState gameState, Texture2D textures[]) {
  BeginDrawing();

    ClearBackground(RAYWHITE);
    drawCogumelos(gameState.cogumelos, gameState.currentAnimationFrame, textures[cogumeloTexture]);
    drawFazendeiro(gameState.fazendeiro, gameState.currentAnimationFrame, textures[fazendeiroTexture]);
    

  EndDrawing();
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
    KeyboardInput keyboardInput = {{0, 0}, 0};
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
        getInputFromKeyboard(&keyboardInput);
        updateFazendeiroPosition(&(currentGameState.fazendeiro), keyboardInput.movement);
        
        // Draw
        drawGame(currentGameState, textures);
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}