/* Library containing game state related functions, such as updating the game and entering pause states. */
#include "gamestate.h"

// Initializer all variables related to the game state
void initializeGameState(GAMESTATE *gameState, int numCogumelos) {
  Vector2 playerStartingPos = {(SCREEN_WIDTH  - SPRITE_SIZE) / 2, PLAYER_UPPER_BOUND + 2 * SPRITE_SIZE};
  Rectangle cogumeloSpawnArea = {SPRITE_SIZE, SPRITE_SIZE, SCREEN_WIDTH - 2*SPRITE_SIZE, SCREEN_HEIGTH - 2*SPRITE_SIZE};
  gameState->harvestedCogumelos = 0;
  gameState->remainingCogumelos = numCogumelos;
  gameState->currentAnimationFrame = 0;
  gameState->currentTime = 0;
  gameState->editingTextBox = 0;

  initializeFazendeiro(&gameState->fazendeiro, playerStartingPos);
  initializeCogumelos(gameState->cogumelos, cogumeloSpawnArea, numCogumelos);
  initializeAranhas(gameState);
  initializeMilipede(&(gameState->milipede));
}

// Draws the game area
void drawGame(GAMESTATE *gameState, Texture2D textures[]) {
  // Draw the background
  ClearBackground(DARKPURPLE);
  // Draw the upper line
  drawCenteredText(TextFormat("Cogumelos Colhidos: %d | Cogumelos Restantes: %d | Vidas: %d | Tiros: %d", gameState->harvestedCogumelos, gameState->remainingCogumelos, gameState->fazendeiro.vidas, gameState->fazendeiro.numTiros), 30, 0, WHITE);

  // Draw the Spiders
  drawSpiders(gameState->aranhas);

  // Draw the Milipede
  drawMilipede(gameState->milipede);

  // Draw the mushrooms
  drawCogumelos(gameState->cogumelos, gameState->currentAnimationFrame, textures[COGUMELO_INDEX]);

  // Draw the player
  drawFazendeiro(gameState->fazendeiro, gameState->currentAnimationFrame, textures[FAZENDEIRO_INDEX]);

  // Display the limit of the player movement
  DrawLine(0, PLAYER_UPPER_BOUND, SCREEN_WIDTH, PLAYER_UPPER_BOUND, PURPLE);

  // Render additional features based on stauts
  switch(gameState->gameStatus) {
    case PAUSED:
      displayPauseScreen(gameState);
      break;
    
    case STARTING:
      displayTutorial();
      break;
    
    case DISPLAYING_RANKING:
      displayRanking(*gameState);
      break;
  }
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

// Switches the gameStatus based on current status and transitions
void updateGameStatus(GAMESTATE *gameState, PLAYERINPUT playerInput) {
  switch (gameState->gameStatus)
  {
  case STARTING:
    // Check if any key has been pressed to start the game
    if (Vector2LengthSqr(playerInput.movement) > 0)
      gameState->gameStatus = RUNNING;
    break;
  
  case RUNNING:
    // Enter pause state
    if (playerInput.pauseButtonPressed)
      gameState->gameStatus = PAUSED;
    else if (playerInput.rankingButtonPressed)
      gameState->gameStatus = DISPLAYING_RANKING;
    break;
  
  case PAUSED:
    // Exit pause state
    if (playerInput.pauseButtonPressed)
      gameState->gameStatus = RUNNING;
    break;

  case DISPLAYING_RANKING:
  // Exit display ranking state
  if (playerInput.rankingButtonPressed)
      gameState->gameStatus = RUNNING;
  break;

  default:
    break;
  }

}

// Runs the actual game 
void gameRun(GAMESTATE *gameState, PLAYERINPUT playerInput) {
  updateFazendeiroPosition(&(gameState->fazendeiro), playerInput.movement);
  updateFazendeiroDirection(&(gameState->fazendeiro), playerInput.mousePosition);
  updateFazendeiroFiringDelay(&(gameState->fazendeiro));

  monsterHit(gameState);
  updateAllSpiders(gameState->aranhas, gameState);
  updateMilipede(&(gameState->milipede), gameState);

  gameState->remainingCogumelos = countRemainingCogumelos(gameState->cogumelos, NUM_COGUMELOS);

  respawnAranha(gameState->aranhas);
  respawnMilipede(&(gameState->milipede));

  // Check if the player is shooting and has ammo
  if (playerInput.shooting && gameState->fazendeiro.numTiros > 0 && gameState->fazendeiro.firing_delay_frames <= 0)
    shoot(gameState);
}

// Update loop, switching between the multiple game states (paused, running, etc.)
void gameLoop(GAMESTATE *gameState, PLAYERINPUT playerInput) {
  // Read info from player
  getInputFromPlayer(&playerInput);

  // Update the game status based on player aciton
  updateGameStatus(gameState, playerInput);

  // Run the game functions if the game is running
  if (gameState->gameStatus == RUNNING) {
    gameRun(gameState, playerInput);

    // Only update animation frames when the game is actually running
    updateFrameCount(gameState);
  }
}

// Loads assets, initializes variables and starts the game
void bootGame(GAMESTATE *gameState) {
  gameState->gameStatus = STARTING;
  Texture2D textures[NUM_TEXTURES];
  PLAYERINPUT playerInput;

  Image tutorialImg = LoadImage("./sprites/tutorial.png");

  // Initialize the game textures:
  textures[FAZENDEIRO_INDEX] = LoadTexture("./sprites/fazendeiro.png");
  textures[COGUMELO_INDEX] = LoadTexture("./sprites/cogumelo.png");
  
  // Main game loop
  while (!WindowShouldClose())
    {
      BeginDrawing();
     	gameLoop(gameState, playerInput);
      drawGame(gameState, textures);
      EndDrawing();
    }
}

// Checks if the player has been hit by a monster
void monsterHit(GAMESTATE *gameState) {
    //test hit against spiders
    if(aranhaFazendeiroCollidesAll(gameState->aranhas, gameState->fazendeiro))
        gameState->fazendeiro.doente = 1;
    //test hit against milipede
    if(milipedeFazendeiroCollides(gameState->milipede, gameState->fazendeiro))
        gameState->fazendeiro.doente = 1;
}

// Counts the number of remaining mushrooms for displaying
int countRemainingCogumelos(COGUMELO cogumelos[], int startingCogumelo) {
  int cogumeloIndex = 0, cogumeloCount = 0;

  for (cogumeloIndex = 0; cogumeloIndex < startingCogumelo; cogumeloIndex++) {
    if (cogumelos[cogumeloIndex].state != ATIVO) {
      cogumeloCount++;
    }
  }

  return startingCogumelo - cogumeloCount;
}