/*
*  src/ui/ui.c
*
*  Draw the ui to the bottom of the screen
*/

#include <stdio.h>
#include <string.h>

#include "../globals.h"
#include "../tools/tools.h"
#include "../sprites/commuter.h"

LCDBitmap* healthBMP = NULL;
LCDBitmap* flashBMP = NULL;

void _uiInitialize()
{
  healthBMP = loadImageAtPath("images/health");
  //flashBMP = loadImageAtPath("");
}

/*
*  Draw the HUD to the screen
*
*  @return - void
*/
void drawHUD()
{
    char scoreText[20];
    char speedText[20];
    char healthText[20];
    char flashText[20];
    
    sprintf(healthText, "Health : ");
    p->graphics->drawText(healthText, strlen(healthText), kASCIIEncoding, 7, 190);
    int i = 0;
    for (i = getPlayerHealth(); i > 0; i--)
    {
        p->graphics->drawBitmap(healthBMP, 50 + (15 * i), 195, kBitmapUnflipped);
    }

    sprintf(flashText, "Flashes : ");
    p->graphics->drawText(flashText, strlen(flashText), kASCIIEncoding, 7, 215);
    for(i = getPlayerFlashes(); i > 0; i--)
    {
        p->graphics->drawBitmap(healthBMP, 60 + (15 * i), 220, kBitmapUnflipped);
    }

    sprintf(scoreText, "Score : %i", getPlayerScore());
    sprintf(speedText, "Speed : %i", getSpeed());

    p->graphics->drawText(scoreText, strlen(scoreText), kASCIIEncoding, 180, 210);
    p->graphics->drawText(speedText, strlen(speedText), kASCIIEncoding, 300, 210);
} //drawHUD
