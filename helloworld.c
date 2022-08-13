#include <stdio.h>
#include "raylib.h"

#define SPRITE_SIZE 64
#define ANIMATION_FPS 8.0f
#define FAZENDEIRO_ANIMATION_FRAMES 3
#define TEXTURE_SCALE 3.0f

typedef struct {
  Vector2 position;
  float speed;
  Texture2D spriteSheet;
  int currentAnimationFrame;

} Fazendeiro;

typedef struct {
  Vector2 movement;
  int shooting;
} KeyboardInput;


Fazendeiro createFazendeiro(Vector2 position, float speed, char spriteSheetURL[]) {
  Fazendeiro fazendeiro;
  fazendeiro.position = position;
  fazendeiro.speed = speed;
  fazendeiro.spriteSheet = LoadTexture(spriteSheetURL);
  fazendeiro.currentAnimationFrame = 0;
  return fazendeiro;
}

void updateFazendeiroPosition(Fazendeiro *fazendeiro, Vector2 movimento){
  fazendeiro->position.x += movimento.x * fazendeiro->speed;
  fazendeiro->position.y += movimento.y * fazendeiro->speed;
}

void drawFazendeiro(Fazendeiro fazendeiro){
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  animationRect.x = (float) fazendeiro.currentAnimationFrame  * SPRITE_SIZE;
  animationRect.y = 0.0f;
  animationRect.width = (float) SPRITE_SIZE;
  animationRect.height = (float) SPRITE_SIZE;

  destRect.x = fazendeiro.position.x;
  destRect.y = fazendeiro.position.y;
  destRect.width = SPRITE_SIZE * TEXTURE_SCALE;
  destRect.height = SPRITE_SIZE * TEXTURE_SCALE;

  originVector.x = (float) SPRITE_SIZE / 2.0f;
  originVector.y = (float) SPRITE_SIZE / 2.0f;

  DrawTexturePro(fazendeiro.spriteSheet,
                 animationRect,
                 destRect,
                 originVector,
                 0,
                WHITE);

};

void updateAnimationFrameCount (float timer, int *frameCount, int animationFrameAmount) {
  // Loop around at the last frame;
  if (*frameCount == animationFrameAmount-1) *frameCount = 0;
  else *frameCount += 1;
}

void drawGame(Fazendeiro fazendeiro, float currentTime) {
  BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

    drawFazendeiro(fazendeiro);

  EndDrawing();
}

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

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const Vector2 startingPos = {200.0f, 200.0f};
    float globalTimer = 0.0f;
    KeyboardInput keyboardInput = {{0, 0}, 0};
    
    // Init window first, otherwise we get a segfault for trying to load textures before the GPU is ready
    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    
    Fazendeiro fazendeiro = createFazendeiro(startingPos, 2.0, "./sprites/fazendeiro.png");

    
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        globalTimer += GetFrameTime();
        // update current animation frames every 1/fps seconds
        if (globalTimer > 1 / ANIMATION_FPS) {
          updateAnimationFrameCount(globalTimer, &(fazendeiro.currentAnimationFrame), FAZENDEIRO_ANIMATION_FRAMES);
          globalTimer = 0;
        }
        getInputFromKeyboard(&keyboardInput);
        updateFazendeiroPosition(&fazendeiro, keyboardInput.movement);
        
        // Draw
        drawGame(fazendeiro, globalTimer);
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}