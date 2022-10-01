#ifndef FAZENDEIRO_H
#define FAZENDEIRO_H

#include "definitions.h"
#include "raylib.h"
#include "cogumelo.h"

// Initializes the player
void initializeFazendeiro(FAZENDEIRO *fazendeiro, Vector2 position);

// Updates the player position
void updateFazendeiroPosition(FAZENDEIRO *fazendeiro, Vector2 movimento);

// Updates the direction at which the player is looking
void updateFazendeiroDirection(FAZENDEIRO *fazendeiro, Vector2 mousePosition);

// Draws and animates the player
void drawFazendeiro(FAZENDEIRO fazendeiro, int currentFrame, Texture2D texture);

// Updates the firing delay
void updateFazendeiroFiringDelay(FAZENDEIRO *fazendeiro);

// Makes a shot from the player into its target direction
void shoot(GAMESTATE *gameState);

#endif