#ifndef RENDER_H
#define RENDER_H

#include "definitions.h"
#include "raylib.h"

// Draw sprite:
// texture          (Texture2D)
// position         (Vector2)
// angle            (float)
// drawSpriteX      (int)
// drawSpriteY      (int)
// scale            (float)
// base             (Color)
void drawSprite(Texture2D texture, Vector2 position, int angle, int drawSpriteX, int drawSpriteY, int isShadow);

#endif
