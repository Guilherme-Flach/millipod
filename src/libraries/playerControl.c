#include "playerControl.h"

// Gets info from the keyboard and updates a PlayerInput variable (used for movement, shooting, etc.)
void getInputFromPlayer(PLAYERINPUT *playerInput) {
  // Set the x part of the movement
  if (IsKeyDown(KEY_D)) playerInput->movement.x = 1.0f;
  else if (IsKeyDown(KEY_A)) playerInput->movement.x = -1.0f;
  else playerInput->movement.x = 0;

  // Set the y part of the movement 
  if (IsKeyDown(KEY_W)) playerInput->movement.y = -1.0f;
  else if (IsKeyDown(KEY_S)) playerInput->movement.y = 1.0f;
  else playerInput->movement.y = 0;

  // Set the shooting variable
  playerInput->shooting = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

  // Set the aiming variable to the mouse position
  playerInput->mousePosition = GetMousePosition();

  // Check if the pause button has been pressed
  playerInput->pauseButtonPressed = IsKeyPressed(KEY_P);

  // Check if player wants to see the ranking screen
  playerInput->rankingButtonPressed = IsKeyPressed(KEY_R);
}