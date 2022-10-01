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
    cogumelos[i].state = ATIVO;
  }
}

// Draws all mushrooms
void drawCogumelos(COGUMELO cogumelos[], int currentFrame, Texture2D texture) {
  int i;
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  // Draws the mushroom sprite based on its animation frame
  animationRect.x = SPRITE_SIZE * currentFrame;
  animationRect.y = 0.0f;
  animationRect.width = (float) SPRITE_SIZE;
  animationRect.height = (float) SPRITE_SIZE;

  // Shift the sprite by half its size as to draw it on its center
  originVector.x = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;
  originVector.y = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;

  destRect.width = SPRITE_SIZE * TEXTURE_SCALE;
  destRect.height = SPRITE_SIZE * TEXTURE_SCALE;

  for (i = 0; i < NUM_COGUMELOS; i++) {
    if (cogumelos[i].state == ATIVO) {     
      destRect.x = cogumelos[i].position.x;
      destRect.y = cogumelos[i].position.y;
      
      DrawCircle(cogumelos[i].position.x, cogumelos[i].position.y, 32, GREEN);
      DrawTexturePro(texture,
                    animationRect,
                    destRect,
                    originVector,
                    0,
                    WHITE);
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
    if (cogumelos[i].state == ATIVO &&
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
    cogumelos[index].state = INATIVO;
}
