#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "fazendeiro.h"
#include "playerControl.h"
#include "cogumelo.h"
#include "gamestate.h"

// Load Textures
void initializeTextures (Texture2D textures[]) {
  textures[FAZENDEIRO_INDEX] = LoadTexture("./sprites/fazendeiro.png");
  textures[COGUMELO_INDEX] = LoadTexture("./sprites/cogumelo.png");
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    PLAYERINPUT playerInput = {{0, 0}, 0};
    Texture2D textures[NUM_TEXTURES];
    GAMESTATE currentGameState;
    
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