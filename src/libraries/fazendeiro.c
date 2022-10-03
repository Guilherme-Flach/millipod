#include "fazendeiro.h"

// Initializes the player
void initializeFazendeiro(FAZENDEIRO *fazendeiro, Vector2 position) {
  fazendeiro->position = position;
  fazendeiro->speed = 5.0f;
  fazendeiro->doente = 0;
  fazendeiro->numTiros = STARTING_TIROS;
  fazendeiro->vidas = 3;
  fazendeiro->firing_delay_frames = 0;
  fazendeiro->iframes = 0;
  fazendeiro->state = ACTIVE;
  TextCopy(fazendeiro->nome, "");
}

// Updates the player position
void updateFazendeiroPosition(FAZENDEIRO *fazendeiro, Vector2 movimento){
  if (fazendeiro->state == ACTIVE) {
    fazendeiro->position.x += movimento.x * fazendeiro->speed;
    // Stop the player from going out of bounds horizontally
    if (fazendeiro->position.x + (SPRITE_SIZE * TEXTURE_SCALE)/2 > SCREEN_WIDTH)
      fazendeiro->position.x = SCREEN_WIDTH - (SPRITE_SIZE * TEXTURE_SCALE)/2;
    else if (fazendeiro->position.x - (SPRITE_SIZE * TEXTURE_SCALE)/2 < 0)
      fazendeiro->position.x = (SPRITE_SIZE * TEXTURE_SCALE)/2;

    fazendeiro->position.y += movimento.y * fazendeiro->speed;
    // Stop the player from going out of bounds vertically
    if (fazendeiro->position.y - (SPRITE_SIZE * TEXTURE_SCALE)/2 < PLAYER_UPPER_BOUND)
      fazendeiro->position.y = PLAYER_UPPER_BOUND + (SPRITE_SIZE * TEXTURE_SCALE)/2;
    else if (fazendeiro->position.y + (SPRITE_SIZE * TEXTURE_SCALE)/2 > SCREEN_HEIGTH)
      fazendeiro->position.y = SCREEN_HEIGTH - (SPRITE_SIZE * TEXTURE_SCALE)/2;
  }
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

if(fazendeiro.iframes <= 0)
    DrawCircle(fazendeiro.position.x, fazendeiro.position.y, 32, BLUE);
  else
    DrawCircle(fazendeiro.position.x, fazendeiro.position.y, 32, RED);
  DrawTexturePro(texture,
                animationRect,
                destRect,
                originVector,
                0,
                WHITE);
  
};

// Updates the firing delay
void updateFazendeiroFiringDelay(FAZENDEIRO *fazendeiro) {

    // Decresases the firing delay by 1 frame, if it is not already 0.
    if (fazendeiro->firing_delay_frames > 0) fazendeiro->firing_delay_frames -= 1;
}

// Makes a shot from the player into its target direction
void shoot(GAMESTATE *gameState) {
  RAYCOLLISION2D shotCollision;
  
  // Remove ammo from player
  gameState->fazendeiro.numTiros--;

    // Make a shot
  shotCollision = collideCogumelos(gameState->fazendeiro, gameState->cogumelos);
  collideAranhas(gameState->fazendeiro, gameState->aranhas, &shotCollision);
  collideMilipede(gameState->fazendeiro, gameState->milipede, &shotCollision);
  // See if the shot actually hit something
  if (shotCollision.collisionType == cogumeloHit) {
    gameState->cogumelos[shotCollision.targetIndex].state = INACTIVE;
    gameState->harvestedCogumelos++;
  }

  if (shotCollision.collisionType == aranhaHit) {
    gameState->aranhas[shotCollision.targetIndex].state = INACTIVE;
  }

  if (shotCollision.collisionType == milipedeHit) {
      shortenMilipede(&gameState->milipede);
  }


  // Draw the shot
  DrawLineV(gameState->fazendeiro.position, Vector2Add(gameState->fazendeiro.position, Vector2Scale(gameState->fazendeiro.aimDirection, shotCollision.collisionDistance)), MAGENTA);

  // Reset the firing delay
  gameState->fazendeiro.firing_delay_frames = FIRING_DELAY;
}

// Make the player take damage, returns how many mushrooms were eaten
void playerTakeDamage(FAZENDEIRO *fazendeiro, int damage) {
  fazendeiro->state = INACTIVE;
  fazendeiro->iframes = PLAYER_INVUNERABILITY_FRAMES;
  fazendeiro->doente += damage;

}

// Updates the player state from active to inactive
void updateFazendeiroState(FAZENDEIRO *fazendeiro) {
  // If the player is inactive, reduce its iframes
  if (fazendeiro->state == INACTIVE) {
    fazendeiro->iframes--;
    // If the iframes drop to 0, change the state
    if(fazendeiro->iframes <= 0)
      fazendeiro->state = ACTIVE;
  }
}
