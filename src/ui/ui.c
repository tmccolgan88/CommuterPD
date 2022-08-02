/*
*  src/ui/ui.c
*
*  Draw the ui to the bottom of the screen
*/

#include <stdio.h>
#include <string.h>

#include "../globals.h"
#include "../sprites/commuter.h"

void drawHUD()
{
    char scoreText[20];
    char speedText[20];
  
    sprintf(scoreText, "Score : %i", getDistanceTraveled());
    sprintf(speedText, "Speed : %i", getSpeed());

    p->graphics->drawText(scoreText, strlen(scoreText), kASCIIEncoding, 30, 210);
    p->graphics->drawText(speedText, strlen(speedText), kASCIIEncoding, 140, 210);
} //drawHUD
