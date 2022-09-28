#include "screens.h"

// Draws centered text on the screen
void drawCenteredText(const char text[], int fontSize, int posY) {
  int textWidth = MeasureText(text, fontSize);

  DrawText(text, (SCREEN_WIDTH - textWidth)/2,  posY, fontSize, WHITE);
}

// Display the tutorial screen showing controls
void displayTutorial() {
  const int RECT_HEIGHT = 250;
  const int RECT_START_Y = (SCREEN_HEIGTH - RECT_HEIGHT)/2;
  const int STEP = 55;
  // Draw a black box with text inside
  DrawRectangleV((Vector2){0, RECT_START_Y}, (Vector2){SCREEN_WIDTH, RECT_HEIGHT}, DARKGRAY);

  drawCenteredText("Controles:", 60, RECT_START_Y);
  drawCenteredText("Movimento: W A S D!", 40, RECT_START_Y + STEP);
  drawCenteredText("Mirar e Atirar: Botao esquerdo do mouse!", 40, RECT_START_Y + 2 * STEP);
  drawCenteredText("Pausar: P ", 40, RECT_START_Y + 5 + 3 * STEP);
  drawCenteredText("Pressione qualquer movimento para comecar!", 25, RECT_START_Y + RECT_HEIGHT - 30);
}

// Display the pause screen
void displayPauseScreen() {
  const int RECT_HEIGHT = 100;
  const int RECT_START_Y = (SCREEN_HEIGTH - RECT_HEIGHT)/2;
  // Draw a black box with text inside
  DrawRectangleV((Vector2){0, RECT_START_Y}, (Vector2){SCREEN_WIDTH, RECT_HEIGHT},BLACK);

  drawCenteredText("Jogo Pausado - Pressione P para continuar.", 40, RECT_START_Y + 5);
  drawCenteredText("Ou, pressione C para carregar um jogo salvo.", 40, RECT_START_Y + 55);
}