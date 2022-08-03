/*
  Author : Tim Mccolgan
  Date   : 08/02/2022

  Purpose : Background bitmap logic 
*/

#include "../globals.h"
#include "../tools/tools.h"

void loadBitmapsBG(void);

LCDBitmap** bmpsbg;
LCDSprite* bgSprite = NULL;
int bgWidth, bgHeight, bgx = 0;

/*
*  Initialize the background generator.
*
*  @return - void
*/
void _backgroundGeneratorInitialize()
{
  loadBitmapsBG();
}

/*
*  Load the bitmaps used by the background generator
*
*  @param - void
*  @return - void
*/
void loadBitmapsBG()
{
    bmpsbg = p->system->realloc(NULL, sizeof(LCDBitmap *) * 1);
    bmpsbg[0] = loadImageAtPath("images/tempbackground1");
}

/*
*  update logic
*
* @return int - status code
*/
int updateBackground()
{
    bgx -= 10; //need to reflect player speed
	if ( bgx < ((bgWidth) * -1)) {
		bgx = bgx + bgWidth;
	}

    p->sprite->markDirty(bgSprite);
    return 1;
}

/*
*  Draw logic
*
*  @return - void
*/
void drawBackground()
{
	p->graphics->drawBitmap(bmpsbg[0], bgx, -40, 0);
    p->graphics->drawBitmap(bmpsbg[0], bgx + bgWidth, -40, 0);
}

/*
*  Create the background, initialize all related sprite data
*
*  @param - void
*  @return - void
*
*  TODO - this will need to be made dynamic to change roads mid level?
*/
void createBackground()
{
    bgSprite = p->sprite->newSprite();

	p->graphics->getBitmapData(bmpsbg[0], &bgWidth, &bgHeight, NULL, NULL, NULL);

	p->sprite->setUpdateFunction(bgSprite, updateBackground);
	p->sprite->setDrawFunction(bgSprite, drawBackground);

	PDRect bgBounds = PDRectMake(0, 0, bgWidth, bgHeight);
	p->sprite->setBounds(bgSprite, bgBounds);
	p->sprite->setZIndex(bgSprite, 0);
	p->sprite->addSprite(bgSprite);
}