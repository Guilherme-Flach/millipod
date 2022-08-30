#include "fazendeiro.h"

// Initializes the player
void initializeFazendeiro(FAZENDEIRO *fazendeiro, Vector2 position, char name[]) {
  fazendeiro->position = position;
  fazendeiro->speed = 5.0f;
  TextCopy(fazendeiro->name, name);
  fazendeiro->score = 0;
}


// Updates the player position
void updateFazendeiroPosition(FAZENDEIRO *fazendeiro, Vector2 movimento){
  fazendeiro->position.x += movimento.x * fazendeiro->speed;
  // Stop the player from going out of bounds horizontally
  if (fazendeiro->position.x + (SPRITE_SIZE * TEXTURE_SCALE)/2 > SCREEN_WIDTH)
  	fazendeiro->position.x = SCREEN_WIDTH - (SPRITE_SIZE * TEXTURE_SCALE)/2;
   else if (fazendeiro->position.x - (SPRITE_SIZE * TEXTURE_SCALE)/2 < 0)
  	fazendeiro->position.x = (SPRITE_SIZE * TEXTURE_SCALE)/2;

  fazendeiro->position.y += movimento.y * fazendeiro->speed;
  // Stop the player from going out of bounds vertically
  if (fazendeiro->position.y + (SPRITE_SIZE * TEXTURE_SCALE)/2 > SCREEN_HEIGTH)
  	fazendeiro->position.y = SCREEN_HEIGTH - (SPRITE_SIZE * TEXTURE_SCALE)/2;
   else if (fazendeiro->position.y - (SPRITE_SIZE * TEXTURE_SCALE)/2 < 0)
  	fazendeiro->position.y = (SPRITE_SIZE * TEXTURE_SCALE)/2;
}

// Updates the direction at which the player is looking
void updateFazendeiroDirection(FAZENDEIRO *fazendeiro, Vector2 mousePosition) {
  // Get the vector representing the direction between the player and the mouse
  Vector2 direction = Vector2Subtract(mousePosition, fazendeiro->position);
  // Normalize the vector for future calculations
  direction = Vector2Normalize(direction);

  fazendeiro->aimDirection = direction;

  if (direction.y <= -PI/4)  fazendeiro->direction = CIMA;
  else if (direction.y >= PI/4) fazendeiro->direction = BAIXO;
  else if (direction.x <= -PI/4) fazendeiro->direction = ESQUERDA;
  else if (direction.x >= PI/4) fazendeiro->direction = DIREITA;
  
}



// Draws and animates the player
void drawFazendeiro(FAZENDEIRO fazendeiro, int currentFrame, Texture2D texture){
  Rectangle animationRect;
  Rectangle destRect;
  Vector2 originVector;

  // Draws the player sprite based on its direction and animation frame
  animationRect.x = SPRITE_SIZE * fazendeiro.direction;

  // EX1 frame 0 of 1, up (1), -> get third sprite (2) (starts at 192px)
  // EX2 frame 1 of 1, down (0) -> get first sprite (0) (starts at 0px)
  // EX3 frame 1 of 1, left (2) -> get the sixth sprite (5) (starts at 384px) 

  animationRect.y = SPRITE_SIZE * currentFrame;
  animationRect.width = (float) SPRITE_SIZE;
  animationRect.height = (float) SPRITE_SIZE;

  destRect.x = fazendeiro.position.x;
  destRect.y = fazendeiro.position.y;
  destRect.width = SPRITE_SIZE * TEXTURE_SCALE;
  destRect.height = SPRITE_SIZE * TEXTURE_SCALE;

  // Shift the sprite by half its size as to draw it on its center
  originVector.x = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;
  originVector.y = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;

  DrawCircle(fazendeiro.position.x, fazendeiro.position.y, 32, BLUE);
  DrawTexturePro(texture,
                 animationRect,
                 destRect,
                 originVector,
                 0,
                WHITE);
  
};
