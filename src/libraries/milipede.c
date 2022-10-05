#include "milipede.h"

void initializeMilipede(MILIPEDE_HEAD * milipede){
    int index;

    milipede->position.x = GetRandomValue(2*MILIPEDE_HITBOX_RADIUS, SCREEN_WIDTH - 2*MILIPEDE_HITBOX_RADIUS);
    milipede->position.y = MILIPEDE_STARTING_Y;
    milipede->direction = 1;
    milipede->state = ACTIVE;
    milipede->descendFrames = MILIPEDE_DESCENT_FRAMES;
    for(index=0; index < NUM_MAX_SEGMENTOS; index++)
    {
        initializeMilipedeSegment(&milipede->segments[index]);
    }

}

void initializeMilipedeSegment(MILIPEDE_SEGMENT * segment){
    segment->state = ACTIVE;
    segment->position.x = OFF_SCREEN_XY;
    segment->position.y = OFF_SCREEN_XY;
}
void updateMilipede(MILIPEDE_HEAD * milipede, GAMESTATE * gamestate){
    if(milipede->state == INACTIVE)
        return;

    //test collision on next frame
    if(testMilipedeNextFrameCollision(milipede, gamestate))
    {
        if(milipede->descendFrames == 0) milipede->direction *= -1;
        milipede->descendFrames = MILIPEDE_DESCENT_FRAMES;

    }
    //update position
    if(milipede->descendFrames > 0){
        //descend
        milipede->position.y += MILIPEDE_DESCENT_SPEED;
        milipede->descendFrames -= 1;
        //angle
        milipede->angle = 90; //facing down
    }
    else {
        //move normally
        milipede->position.x += milipede->direction * MILIPEDE_SPEED;
        milipede->angle = 90 + milipede->direction * -90;
    }

    // Check if milipede has gone all the way to the bottom
    if(milipede->position.y >= SCREEN_HEIGTH + MILIPEDE_HITBOX_RADIUS) {
        initializeMilipede(milipede);
    }

    updateSegments(milipede);

    // Kills the milipede if it goes below the screen
    if(milipede->position.y > SCREEN_HEIGTH + KILL_ZONE)
        milipede->state = INACTIVE;
}

void updateSegments(MILIPEDE_HEAD * milipede){
    int index = 1;

    segmentFollow(&milipede->segments[0], milipede->position);
    //gets the angle between the head and the first segment using atan2f, and then corrects it to work in degrees
    milipede->segments[0].angle = 90 + atan2f( milipede->segments[0].position.x - milipede->position.x , milipede->segments[0].position.y - milipede->position.y) * (-360 / (2 * PI));

    while(index < NUM_MAX_SEGMENTOS){
        segmentFollow(&milipede->segments[index], milipede->segments[index - 1].position);
        //gets the angle between the segment and the previous segment using atan2f, and then corrects it to work in degrees
        milipede->segments[index].angle = 90 + atan2f( milipede->segments[index].position.x - milipede->segments[index-1].position.x  , milipede->segments[index].position.y - milipede->segments[index-1].position.y) * (-360 / (2 * 3.1415));
        index++;
    }

}


int testMilipedeNextFrameCollision(MILIPEDE_HEAD *milipede, GAMESTATE * gamestate){
    //Generate the next frame position
    MILIPEDE_HEAD nextFrameMilipede = *milipede;

    if(milipede->descendFrames > 0){
        //descend
        nextFrameMilipede.position.y += MILIPEDE_DESCENT_SPEED;
    }
    else {
        //move normally
        nextFrameMilipede.position.x += milipede->direction * MILIPEDE_SPEED;

    }

    if(milipedeBorderCollision(nextFrameMilipede.position) || milipedeCogumeloCollidesAll(nextFrameMilipede, milipede, gamestate->cogumelos))
        return 1;
    return 0;
}

int milipedeBorderCollision(Vector2 position){

    if(position.x < 0 || position.x > SCREEN_WIDTH - MILIPEDE_BORDER_COLLIDE_X)
        return 1;
    return 0;

}

int milipedeCogumeloCollides(MILIPEDE_HEAD milipede, COGUMELO cogumelo){
    if(cogumelo.state == INACTIVE) // cannot collide with inactive/destroyed mushroom
        return 0;

    if(CheckCollisionCircles(milipede.position, MILIPEDE_HITBOX_RADIUS, cogumelo.position, COGUMELO_HITBOX_RADIUS))
        return 1;

    return 0;

}

// tests the collision of one spider against all of the mushrooms
int milipedeCogumeloCollidesAll(MILIPEDE_HEAD milipede, MILIPEDE_HEAD * real_milipede, COGUMELO cogumelos[]){
    int index;

    for(index=0; index < NUM_COGUMELOS; index++)
    {
        if(milipedeCogumeloCollides(milipede, cogumelos[index])){
            lengthenMilipede(real_milipede);
            destroyCogumelo(cogumelos, index);
            return 1;
            }
    }
    return 0;

}

void killMilipede(GAMESTATE * gamestate){
    gamestate->milipede.state = 0;

}


int milipedeFazendeiroCollides(MILIPEDE_HEAD milipede, FAZENDEIRO player){
    if(milipede.state == INACTIVE) // cannot collide with inactive/destroyed milipede
        return 0;

    if(CheckCollisionCircles(milipede.position, MILIPEDE_HITBOX_RADIUS, player.position, FAZENDEIRO_HITBOX_RADIUS))
        return 1;
    return 0;

}

void segmentFollow(MILIPEDE_SEGMENT * segment, Vector2 prev){
    if(Vector2Distance(segment->position, prev) > MILIPEDE_FOLLOW_DISTANCE){
        segmentMoveTo(segment, prev);
    }
}

void segmentMoveTo(MILIPEDE_SEGMENT * segment, Vector2 prev){
    // Source for equations at https://www.desmos.com/calculator/0al5rllk9m?lang=pt-BR
    double A = segment->position.x;  // segment X
    double B = prev.x;  // prev X
    double C = segment->position.y;  // segment Y
    double D = prev.y;  // prev Y
    double E =  B - A;  // segment vector x
    //double F = ;  // prev vector x
    double G = D - C;  // segment vector y
    //double H = ;  // prev vector y


    double a = pow(E, 2) + pow(G, 2);
    double b = 2 * (A * E + C * G - B * E - D * G);
    double c = pow(A,2) + pow(B,2) + pow(C,2) + pow(D,2) - 2 * A * B - 2 * D * C - pow(MILIPEDE_FOLLOW_DISTANCE,2);


    // quadratic equation (Bhaskara)
    //
    //       __________
    // -b - V b�-4ac
    // -----------------
    //          2a

    double delta = pow(b, 2) - (4 * a * c);

    double t;

    if(delta >= 0) { // Caso algo errado haja acontecido, para a funcao.

        t = (- b - sqrt( delta )) / (2 * a);

        segment->position.x = A + E * t;
        segment->position.y = C + G * t;
    }
    //segment->position.x = 300;
    //segment->position.y = 300;
}

void drawMilipede(MILIPEDE_HEAD milipede, int currentFrame, Texture2D texture){
    int index = 0;

    if(milipede.state == ACTIVE){

    //draw the body
        while(index < NUM_MAX_SEGMENTOS){
            if(milipede.segments[index].state == ACTIVE)
                drawMilipedeSegment(milipede.segments[index], currentFrame, texture);
            index++;
        }

                //draw the head
        drawSprite(texture, milipede.position, milipede.angle, currentFrame, 1, 1);
        drawSprite(texture, milipede.position, milipede.angle, currentFrame, 1, 0);
    }

}

void drawMilipedeSegment(MILIPEDE_SEGMENT segment, int currentFrame, Texture2D texture){
    int pixelMod;

    pixelMod = ((int)(segment.position.x + segment.position.y)/MILIPEDE_WALK_DIST_ANIM % 2);
    //segment hitbox
    //DrawCircleV(segment.position, 25, YELLOW);
    drawSprite(texture, segment.position, segment.angle, pixelMod, 0, 1);
    drawSprite(texture, segment.position, segment.angle, pixelMod, 0, 0);

}

void collideMilipede(FAZENDEIRO fazendeiro, MILIPEDE_HEAD milipede, RAYCOLLISION2D * collision){
  Vector2 fazendeiroAimMaxRange = Vector2Add(fazendeiro.position, Vector2Scale(fazendeiro.aimDirection, MAX_DISTANCE));
  float currentDistance,
        minDistance = collision->collisionDistance;
  int i;

    // Check if milipede head is active and in the aim line
    if (milipede.state == ACTIVE &&
        CheckCollisionPointLine(milipede.position, fazendeiro.position, fazendeiroAimMaxRange, AIM_RADIUS))
    {
      currentDistance = Vector2Distance(milipede.position, fazendeiro.position);
      // Check if it is closer than the current target
      if (currentDistance < minDistance) {
        // Update the collision, minDistance and the closest target position
        collision->collisionDistance = currentDistance;
        collision->collisionType = milipedeHit;
        collision->targetIndex = -1;

      }
    }

}


int shortenMilipede(MILIPEDE_HEAD * milipede){
    int index = 0;
    if(milipede->segments[0].state == INACTIVE){
        milipede->state = INACTIVE;
        return MILIPEDE_HEAD_SCORE;
    }
    // Travels the segments until the last one, be that the max number of segments or the last active segment
    while(index < NUM_MAX_SEGMENTOS && milipede->segments[index].state == ACTIVE){
        index++;
    }
    milipede->segments[index - 1].state = INACTIVE;

    return MILIPEDE_SEGMENT_SCORE;
}
void lengthenMilipede(MILIPEDE_HEAD * milipede) {
    int index = 0;
    // Travels the segments until the last one, be that the max number of segments or the last active segment
    while((index + 1) < NUM_MAX_SEGMENTOS && milipede->segments[index].state == ACTIVE){
        index++;
    }
    milipede->segments[index].state = ACTIVE;
}


void respawnMilipede(MILIPEDE_HEAD * milipede){
    if(milipede->state == INACTIVE){
        initializeMilipede(milipede);
    }

}

// Count the amount of segments the milipede has:
int countSegments(MILIPEDE_HEAD *milipede) {
    int index = 0;
    // Travels the segments until the last one, be that the max number of segments or the last active segment
    while(index < NUM_MAX_SEGMENTOS && milipede->segments[index].state == ACTIVE){
        index++;
    }
    return index+1;
}
