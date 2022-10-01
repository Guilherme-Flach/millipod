#ifndef COGUMELO_H
#define COGUMELO_H

#include "definitions.h"

// Initializes all mushrooms with a random position inside the coverArea
void initializeCogumelos(COGUMELO cogumelos[], Rectangle coverArea, int numCogumelos);

// Draws all mushrooms
void drawCogumelos(COGUMELO cogumelos[], int currentFrame, Texture2D texture);

// Returns the index of the closest mushroom to the player that collides with the player
RAYCOLLISION2D collideCogumelos(FAZENDEIRO fazendeiro, COGUMELO cogumelos[]);

// Changes the status of the selected mushroom to inactive
void destroyCogumelo(COGUMELO * cogumelos, int index);

#endif