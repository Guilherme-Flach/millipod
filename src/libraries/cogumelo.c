#include "cogumelo.h"

// Initializes all mushrooms with a random position
void initializeCogumelos(COGUMELO cogumelos[], Rectangle coverArea, int numCogumelos) {
  int i, random_x, random_y;

  for (i = 0; i < numCogumelos; i++) {
    // Generate a random Vector2 inside of the coverage area
    random_x = GetRandomValue(coverArea.x, coverArea.x + coverArea.width);
    random_y = GetRandomValue(coverArea.y, coverArea.y + coverArea.height);
    cogumelos[i].position.x = random_x;
    cogumelos[i].position.y = random_y;
    cogumelos[i].state = ACTIVE;
  }
}

// Draws all mushrooms
void drawCogumelos(COGUMELO cogumelos[], int currentFrame, Texture2D texture) {
  int i;
  // Draws the mushroom sprite based on its animation frame
   for (i = 0; i < NUM_COGUMELOS; i++) {
    if (cogumelos[i].state == ACTIVE) {
      //DrawCircle(cogumelos[i].position.x, cogumelos[i].position.y, 32, GREEN);
      drawSprite(texture, cogumelos[i].position, 0, currentFrame, 0, 1);
      drawSprite(texture, cogumelos[i].position, 0, currentFrame, 0, 0);
    }
  }
}

// Returns the index of the closest mushroom to the player that collides with the player
RAYCOLLISION2D collideCogumelos(FAZENDEIRO fazendeiro, COGUMELO cogumelos[]) {
  RAYCOLLISION2D returnCollision = {MAX_DISTANCE, -1, nothing};
  Vector2 fazendeiroAimMaxRange = Vector2Add(fazendeiro.position, Vector2Scale(fazendeiro.aimDirection, MAX_DISTANCE));
  float currentDistance,
        minDistance = MAX_DISTANCE;
  int i,
      returnIndex = -1;

  for (i = 0; i < NUM_COGUMELOS; i++) {
    // Check if mushroom is active and in the aim line
    if (cogumelos[i].state == ACTIVE &&
        CheckCollisionPointLine(cogumelos[i].position, fazendeiro.position, fazendeiroAimMaxRange, AIM_RADIUS))
    {
      currentDistance = Vector2Distance(cogumelos[i].position, fazendeiro.position);
      // Check if it is closer than the current target
      if (currentDistance < minDistance) {
        // Update the return, minDistance and the closest target position
        returnIndex = i;
        minDistance = currentDistance;
        returnCollision.collisionType = cogumeloHit;
      }
    }
  }

  returnCollision.collisionDistance = minDistance;
  returnCollision.targetIndex = returnIndex;

  return returnCollision;
}

void destroyCogumelo(COGUMELO * cogumelos, int index){
    cogumelos[index].state = INACTIVE;
}
