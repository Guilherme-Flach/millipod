#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "./libraries/fazendeiro.h"
#include "./libraries/playerControl.h"
#include "./libraries/cogumelo.h"
#include "./libraries/gamestate.h"

int main(void)
{
    GAMESTATE currentGameState = {};

    // Initialize variables
    initializeGameState(&currentGameState, NUM_COGUMELOS);

    // Initialize window first, otherwise we get a segfault for trying to load textures before the GPU is ready
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Mili-pede");

    // Set the random seed for Raylib
    SetRandomSeed(time(0));
    SetTargetFPS(60);

    // Start the game
    bootGame(&currentGameState);

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}