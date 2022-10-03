/* Helper library for displaying info sreens like pause and tutorial */
#include "arquivos.h"

#ifndef SCREENS_H
#define SCREENS_H
#include "definitions.h"

// Draws text vertically centered on the screen
void drawCenteredText(const char text[], int fontSize, int posY, Color color);

// Display the pause screen and starts the load/save game data 
void displayPauseScreen(GAMESTATE *gameState);

// Display the tutorial screen
void displayTutorial();

// Display Rankings
void displayRanking(GAMESTATE gameState);

// Display the ending screen
void displayEndingScreen(GAMESTATE *gameState);

#endif