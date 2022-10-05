#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "./libraries/gamestate.h"

// Requisitos nao implementados: Fase 2

int main(void)
{
    GAMESTATE currentGameState = {};

    // Initialize variables
    initializeGameState(&currentGameState, NUM_COGUMELOS);

    // Initialize window first, otherwise we get a segfaudlt for trying to load textures before the GPU is ready
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Mili-pede");

    // Set the random seed for Raylib
    SetRandomSeed(time(0));
    SetTargetFPS(60);

    // Start & Run the game
    bootGame(&currentGameState);
    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}