#include "gamestate.h"

// Makes a shot from the player into its target direction
void shoot(GAMESTATE *gameState) {
  RAYCOLLISION2D shotCollision;

  // Make a shot
  shotCollision = collideCogumelos(gameState->fazendeiro, gameState->cogumelos);

  // See if the shot actually hit something
  if (shotCollision.collisionType != nothing) {
    gameState->cogumelos[shotCollision.targetIndex].state = INATIVO;
    gameState->fazendeiro.score += 100;
  }

  // Draw the shot
  DrawLineV(gameState->fazendeiro.position, Vector2Add(gameState->fazendeiro.position, Vector2Scale(gameState->fazendeiro.aimDirection, MAX_DISTANCE)), MAGENTA);
}

// Initializer all variables related to the game state
void initializeGameState(GAMESTATE *gameState) {
  Vector2 playerStartingPos = {200.0f, 200.0f};
  Rectangle cogumeloSpawnArea = {SPRITE_SIZE, SPRITE_SIZE, SCREEN_WIDTH - 2 * SPRITE_SIZE, SCREEN_HEIGTH - 2 * SPRITE_SIZE - 100};

  initializeFazendeiro(&gameState->fazendeiro, playerStartingPos, "Wanderley");
  initializeCogumelos(gameState->cogumelos, cogumeloSpawnArea);

  SetRandomSeed(time(0));
}

// Draws the game area
void drawGame(GAMESTATE gameState, Texture2D textures[]) {
  ClearBackground(DARKPURPLE);
  DrawText(TextFormat("Nome: %s | Pontuacao: %d", gameState.fazendeiro.name, gameState.fazendeiro.score), 0, 0, 40, WHITE);
  drawCogumelos(gameState.cogumelos, gameState.currentAnimationFrame, textures[COGUMELO_INDEX]);
  drawFazendeiro(gameState.fazendeiro, gameState.currentAnimationFrame, textures[FAZENDEIRO_INDEX]);    
}

// Updates the frame count used in animations
void updateFrameCount(GAMESTATE *gameState)
{
	  // Update
        gameState->currentTime += GetFrameTime();
        // update current animation frames every 1/fps seconds
        if (gameState->currentTime > 1 / ANIMATION_FPS) {
          if (gameState->currentAnimationFrame < NUM_ANIMATION_FRAMES-1)
            gameState->currentAnimationFrame++;
          else gameState->currentAnimationFrame = 0;
          gameState->currentTime = 0;
        }

}
