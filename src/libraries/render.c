#include "render.h"

// Draw sprite:
// texture          (Texture2D)
// position         (Vector2)
// angle            (float)
// drawSpriteX      (int)
// drawSpriteY      (int)
// base             (Color)
void drawSprite(Texture2D texture, Vector2 position, int angle, int drawSpriteX, int drawSpriteY, int isShadow){
    Rectangle animationRect;
    Rectangle destRect;
    Vector2 originVector;

    Color color;

    // Draws the spider sprite based on its animation frame
    animationRect.x = SPRITE_SIZE * drawSpriteX;
    animationRect.y = SPRITE_SIZE * drawSpriteY;
    animationRect.width = (float) SPRITE_SIZE;
    animationRect.height = (float) SPRITE_SIZE;

    // Shift the sprite by half its size as to draw it on its center
    originVector.x = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;
    originVector.y = (float) SPRITE_SIZE * TEXTURE_SCALE / 2.0f;

    destRect.width = SPRITE_SIZE * TEXTURE_SCALE;
    destRect.height = SPRITE_SIZE * TEXTURE_SCALE;
    destRect.x = position.x;
    destRect.y = position.y;

    if(isShadow != 0) {
        color = SHADOW;
        destRect.x += SHADOW_X_OFFSET;
        destRect.y += SHADOW_Y_OFFSET;
    } else {
        color = WHITE;
    }

    DrawTexturePro(texture,
                animationRect,
                destRect,
                originVector,
                angle,
                color);
}
