/* Library containing game state related functions, such as updating the game and entering pause states. */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "definitions.h"
#include "cogumelo.h"
#include "fazendeiro.h"
#include "playerControl.h"
#include "aranha.h"
#include "milipede.h"
#include "screens.h"

// Initialize all variables related to the game state
void initializeGameState(GAMESTATE *gameState, int numCogumelos);

// Draws the game area
void drawGame(GAMESTATE *gameState, Texture2D textures[]);

// Updates the frame count used in animations
void updateFrameCount(GAMESTATE *gameState);

// Loads assets,initializes variables and starts the game
void bootGame(GAMESTATE *gameState);

// Update loop, switching between the multiple game states (paused, running, etc.)
void gameLoop(GAMESTATE *gameState, PLAYERINPUT playerInput);

// Runs the actual game 
void gameRun(GAMESTATE *gameState, PLAYERINPUT playerInput);

// Switches the gameStatus from paused to running and vice versa, when the pause button is pressed
void updateGameStatus(GAMESTATE *gameState, PLAYERINPUT playerInput);

// Checks if the player has been hit by a monster
void monsterHit(GAMESTATE *gameState);

// Counts the number of remaining mushrooms for displaying
int countRemainingCogumelos(COGUMELO cogumelos[], int startingCogumelos);

#endif