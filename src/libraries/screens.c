#include "screens.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define RECT_HEIGHT 250
#define RECT_START_Y (SCREEN_HEIGTH - RECT_HEIGHT)/2
#define ELEMENT_WIDTH 200
#define ELEMENT_HEIGHT 45

// Draws centered text on the screen
void drawCenteredText(const char text[], int fontSize, int posY, Color color) {
  int textWidth = MeasureText(text, fontSize);

  DrawText(text, (SCREEN_WIDTH - textWidth)/2,  posY, fontSize, color);
}

// Display the tutorial screen showing controls
void displayTutorial() {
  // Draw a black box with text inside
  DrawRectangleV((Vector2){0, RECT_START_Y}, (Vector2){SCREEN_WIDTH, RECT_HEIGHT}, DARKGRAY);

  drawCenteredText("Controles:", 3*DEFAULT_FONT_SIZE, RECT_START_Y, WHITE);
  drawCenteredText("Movimento: W A S D!", DEFAULT_FONT_SIZE, RECT_START_Y + ELEMENT_HEIGHT + SPACING, WHITE);

  drawCenteredText("Mirar e Atirar: Botao esquerdo do mouse!", 1.5*DEFAULT_FONT_SIZE, RECT_START_Y + 2 * ELEMENT_HEIGHT + SPACING, WHITE);

  drawCenteredText("Pausar: P | Ranking: R", 1.5*DEFAULT_FONT_SIZE, RECT_START_Y + 3 * ELEMENT_HEIGHT + 2 * SPACING, WHITE);

  drawCenteredText("Pressione qualquer movimento para comecar!", 1.25*DEFAULT_FONT_SIZE, RECT_START_Y + RECT_HEIGHT - ELEMENT_HEIGHT + SPACING, WHITE);
}

// Display the pause screen and starts the load/save game data 
void displayPauseScreen(GAMESTATE *gameState) {
  // Constants for clearance on drawing
  const Rectangle TEXTBOX_BOUNDS = {(SCREEN_WIDTH-ELEMENT_WIDTH)/2, RECT_START_Y + 2*ELEMENT_HEIGHT + 3*SPACING, ELEMENT_WIDTH, ELEMENT_HEIGHT};
  const int BUTTONBOX_WIDTH = 2 * ELEMENT_WIDTH + 3*SPACING;

  const Rectangle BUTTON1_BOUNDS = {SCREEN_WIDTH/2 + 3*SPACING, RECT_START_Y + 3*ELEMENT_HEIGHT + 6*SPACING, ELEMENT_WIDTH, ELEMENT_HEIGHT};

  const Rectangle BUTTON2_BOUNDS = {(SCREEN_WIDTH - BUTTONBOX_WIDTH)/2, RECT_START_Y + 3*ELEMENT_HEIGHT + 6*SPACING, ELEMENT_WIDTH, ELEMENT_HEIGHT};

  // Draw a black box with text inside
  DrawRectangleV((Vector2){0, RECT_START_Y}, (Vector2){SCREEN_WIDTH, RECT_HEIGHT},BLACK);

  // Draw text for unpausing
  drawCenteredText("Jogo Pausado - Pressione P para continuar.", 1.5*DEFAULT_FONT_SIZE, RECT_START_Y + SPACING, YELLOW);
  drawCenteredText("Ou digite seu nome para salvar/carregar um jogo.", 1.5*DEFAULT_FONT_SIZE, RECT_START_Y + 11*SPACING, LIGHTGRAY);

  // Draw the player name input box
  DrawRectangleRec(TEXTBOX_BOUNDS, LIGHTGRAY);
  GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
  if(GuiTextBox(TEXTBOX_BOUNDS, gameState->fazendeiro.nome, PLAYER_NAME_SIZE, gameState->editingTextBox))
    gameState->editingTextBox = !gameState->editingTextBox; // Switch focus in and out
  
  // Draw the load game button
  GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  if (GuiButton(BUTTON1_BOUNDS, GuiIconText(ICON_FILE_OPEN, "Carregar Jogo"))){
    loadGame(gameState);
  };

  // Draw the save game button
  GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  if (GuiButton(BUTTON2_BOUNDS, GuiIconText(ICON_FILE_SAVE, "Salvar Jogo"))){
    saveGame(*gameState);
  };
}

void displayRanking(GAMESTATE gameState) {
  JOGADOR playersList[NUM_PLAYERS];
  JOGADOR currentPlayer, tempPlayer;
  int playerIndex = 0;

  // Copy the current player's info
  TextCopy(currentPlayer.nome, gameState.fazendeiro.nome);
  currentPlayer.pontuacao = gameState.harvestedCogumelos;

  initializeList(playersList);
  loadRankingList(playersList);
  insertPlayer(playersList, currentPlayer);
  sort(playersList);

  // Draw a dark screen
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH, BLACK);
  drawCenteredText("RANKING:", 60, 200, YELLOW);

  tempPlayer = playersList[playerIndex];
  while (!isEmptyPlayer(tempPlayer) && playerIndex < NUM_PLAYERS) {
    drawCenteredText(TextFormat("%d - %s : %d", playerIndex+1, tempPlayer.nome, tempPlayer.pontuacao), 1.5*DEFAULT_FONT_SIZE, 215 + 10*SPACING * (playerIndex + 1), WHITE);
    playerIndex++;
    tempPlayer = playersList[playerIndex];
  }
}

// Display the ending screen
void displayEndingScreen(GAMESTATE *gameState) {
  const Rectangle TEXTBOX_BOUNDS = {(SCREEN_WIDTH-ELEMENT_WIDTH)/2, 630, ELEMENT_WIDTH, ELEMENT_HEIGHT};
  const Rectangle BUTTON_BOUNDS = {(SCREEN_WIDTH-ELEMENT_WIDTH)/2, 650 + ELEMENT_HEIGHT, ELEMENT_WIDTH, ELEMENT_HEIGHT};
  const int BUTTONBOX_WIDTH = 2 * ELEMENT_WIDTH + 3*SPACING;

  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH, BLACK);
  drawCenteredText("Fim de Jogo!", 4*DEFAULT_FONT_SIZE, 300, RED);
  drawCenteredText(TextFormat("Você colheu %d cogumelos;", gameState->harvestedCogumelos), 1.5*DEFAULT_FONT_SIZE, 400, LIGHTGRAY);

  drawCenteredText(TextFormat("Você teve que comer %d cogumelos para se curar;", gameState->eatenCogumelos), 1.5*DEFAULT_FONT_SIZE, 450, LIGHTGRAY);

  drawCenteredText(TextFormat("Para uma pontuacao final de:", gameState->eatenCogumelos), 1.5*DEFAULT_FONT_SIZE, 500, LIGHTGRAY);

  drawCenteredText(TextFormat("%d pontos!", gameState->harvestedCogumelos-gameState->eatenCogumelos), 3.5*DEFAULT_FONT_SIZE, 550, YELLOW);

  // Draw the player name input box
  DrawRectangleRec(TEXTBOX_BOUNDS, LIGHTGRAY);
  GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
  if(GuiTextBox(TEXTBOX_BOUNDS, gameState->fazendeiro.nome, PLAYER_NAME_SIZE, gameState->editingTextBox))
    gameState->editingTextBox = !gameState->editingTextBox; // Switch focus in and out

    // Draw the save game button
  GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  if (GuiButton(BUTTON_BOUNDS, GuiIconText(ICON_FILE_SAVE, "Salvar Pontuacao"))){
    gameState->gameStatus = ENDED;
  };
}