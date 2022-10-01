#ifndef ARANHA_H
#define ARANHA_H

#include "definitions.h"
#include "raylib.h"
#include "cogumelo.h"

void initializeAranhas(GAMESTATE * gamestate);

void initializeAranha(ARANHA * aranha);

void respawnAranha(ARANHA aranhas[]);

void updateAllSpiders(ARANHA aranhas[], GAMESTATE * gamestate);

//updates
void updateSpiderPositionAndMushrooms(ARANHA *aranha, GAMESTATE * gamestate);

Vector2 testAranhaNextFrameCollision(ARANHA aranha, GAMESTATE * gamestate);

int aranhaBorderCollides(ARANHA aranha);

int aranhaCogumeloCollides(ARANHA aranha, COGUMELO cogumelo);

int aranhaCogumeloCollidesAll(ARANHA aranha, COGUMELO cogumelos[]);

int aranhaFazendeiroCollides(ARANHA aranha, FAZENDEIRO player);

int aranhaFazendeiroCollidesAll(ARANHA aranhas[], FAZENDEIRO player);

void drawSpiders(ARANHA aranhas[]);

void collideAranhas(FAZENDEIRO fazendeiro, ARANHA aranhas[], RAYCOLLISION2D * collision);

#endif

