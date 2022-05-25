/*
  Author : Tim Mccolgan
  Date   : 05/13/2022

  Purpose : Logic to create/update level enemies. 
*/

#include "pd_api.h"
#include "../globals.h"
#include "../structs/spritestructs.h"
#include "spriteengine.h"
#include "../tools/tools.h"

int delTime = 0;
int canJump = 1;
int laneSize = 20;
LCDBitmap* bgBMP = NULL;
LCDSprite* bgSprite = NULL;
int bgWidth, bgHeight = 0;
int bgx, bgy = 0;
LCDBitmap* commuterBMP = NULL;
LCDBitmap* blinkBMP = NULL;
int commuterWidth, commuterHeight = 0;
int commWidthOffset, commHeightOffset = 0;
int playerSpeed = 10;
int speedTime = 0;
int speedTimer = 1000;
int canChangeSpeed = 1;
int playerHealth = 3;
int isDamaged = 0;
int damageTimer = 3000;
int damageTime = 0;
int blinkTimer = 450;
int blinkTime = 0;
int blinking = 0;
SpritePlayer* player = NULL;
BaseListNode* baseListHead = NULL;
BaseListNode* baseListCurrent = NULL;

//functions

int isColliding(PDRect* a, PDRect* b)
{
    if (a->x > (b->x + b->width) ||
       (a->x + a->width) < b->x)
        return 0;

    if (a->y > (b->y + b->height) ||
        (a->y + a->height) < b->y)
        return 0;

    return 1;
}

int updateBackground()
{
    bgx -= playerSpeed;
	if ( bgx < ((bgWidth) * -1)) {
		bgx = bgx + bgWidth;
	}

    p->sprite->markDirty(bgSprite);
    return 1;
}

void drawBackground()
{
	p->graphics->drawBitmap(bgBMP, bgx, -30, 0);
    p->graphics->drawBitmap(bgBMP, bgx + bgWidth, -30, 0);
}

void createBackground()
{
    bgSprite = p->sprite->newSprite();

	bgBMP = loadImageAtPath("images/tempbackground1", p);
	p->graphics->getBitmapData(bgBMP, &bgWidth, &bgHeight, NULL, NULL, NULL);

	p->sprite->setUpdateFunction(bgSprite, updateBackground);
	p->sprite->setDrawFunction(bgSprite, drawBackground);

	PDRect bgBounds = PDRectMake(0, 0, bgWidth, bgHeight);
	p->sprite->setBounds(bgSprite, bgBounds);

	p->sprite->setZIndex(bgSprite, 0);

	p->sprite->addSprite(bgSprite);
    
}

int getDistanceTraveled()
{
    if (player == NULL)
      return -1;

    return player->distanceTraveledSP;
}

int updateCommuter(void *s)
{
	SpritePlayer* playerPtr = ((SpriteBase*) s);
	PDButtons current, pushed, released;
    float crankChange = 0;
	int deltaX = 0;
	int deltaY = 0;

	p->system->getButtonState(&current, &pushed, &released);
    crankChange = p->system->getCrankChange();

    if (speedTime <= speedTimer)
      speedTime += delTime;

    if (speedTime > speedTimer)
    {
        canChangeSpeed = 1;
        speedTime = 0;
    }

    //change speed logic
    if (crankChange && canChangeSpeed)
    {
        if (crankChange > 0)
        {
            if (playerSpeed < 20)
                playerSpeed += 2;
        }
        else
        {
            if (playerSpeed > 4)
                playerSpeed -= 2;
        }
        canChangeSpeed = 0;
    }	

    //linear movement logic
	if (kButtonUp & current)
	{
		deltaY = -3;
	}
	if (kButtonDown & current)
	{	
		deltaY = 3;	  
	}
	if (kButtonLeft & current)
	{
		deltaX = -3;
	}
	if (kButtonRight & current)
	{	
		deltaX = 3;	  
	}

    //lane warping logic
    if ((kButtonA & pushed) && canJump)
    {
        deltaY = -laneSize;
        canJump = 0;
    }
    if ((kButtonB & pushed) && canJump)
    {
        deltaY = laneSize;
        canJump = 0;
    }

    if ((kButtonA & released) && !canJump)
    {
        canJump = 1;
    }
    if ((kButtonB & released) && !canJump)
    {
        canJump = 1;
    } 

    //damage state logic
    if  (isDamaged)
    {
        deltaX = -1;
        deltaY = 0;
        blinkTime += delTime;
        if (blinkTime >= blinkTimer)
        {
            blinkTime = 0;
            if (blinking)
            {
                p->sprite->setImage(playerPtr->sb->sprite, commuterBMP, kBitmapUnflipped);
                blinking = 0;
            }
            else
            {
                p->sprite->setImage(playerPtr->sb->sprite, blinkBMP, kBitmapUnflipped);
                blinking = 1;
            }
        }

        damageTime += delTime;
        if (damageTime >= damageTimer)
        {
            p->sprite->setImage(playerPtr->sb->sprite, commuterBMP, kBitmapUnflipped);
            isDamaged = 0;
            damageTime = 0;
        }
    }

    //make sure the player is in the screen bounds
    float x,y;
    p->sprite->getPosition(playerPtr->sb->sprite, &x, &y);

    x += (float) deltaX;
    y += (float) deltaY;

    if (x < commWidthOffset)
        x = commWidthOffset;
    if (x > 400 - commWidthOffset)
        x = 400 - commWidthOffset;
    if (y < commuterHeight)
        y = commuterHeight;
    if (y > 240 - commuterHeight)
        y = 240 - commuterHeight;

    p->sprite->moveTo(playerPtr->sb->sprite, x, y);
    distanceTraveled += playerSpeed;

	return 1;
} //updatePlayer

void commuterDamage()
{
  playerHealth--;
  if (playerHealth == -1)
  {
      ;
    //gameOver
    //gameState = GameOver;
  }

  //isDamaged = 1;

}

void createPlayer(LCDBitmap* bmp)
{
    commuterBMP = bmp;
    blinkBMP = loadImageAtPath("images/blink", p);
	SpriteBase* base = p->system->realloc(NULL, sizeof(SpriteBase));
	SpritePlayer* spritePlayer = p->system->realloc(NULL, sizeof(SpritePlayer));

	LCDSprite* tempSprite = p->sprite->newSprite();

	p->sprite->setImage(tempSprite, bmp, kBitmapUnflipped);
	
	p->graphics->getBitmapData(bmp, &commuterWidth, &commuterHeight, NULL, NULL, NULL);
    commWidthOffset = commuterWidth / 2;
    commuterHeight = commuterHeight / 2;

	PDRect cr = PDRectMake(0, 0, commuterWidth, commuterHeight);
	p->sprite->setCollideRect(tempSprite, cr);

	p->sprite->moveTo(tempSprite, 50, 50);
	p->sprite->addSprite(tempSprite);

	base->sprite = tempSprite;
    base->spriteUpdate = updateCommuter;
	
	spritePlayer->sb = base;
    spritePlayer->distanceTraveledSP = 0;
	spritePlayer->health = 3;
 
	player = spritePlayer;
}

void addBaseEnemy(SpriteBase* enemy)
{
    BaseListNode* newNode = p->system->realloc(NULL, sizeof(BaseListNode));
    newNode->enemy = enemy;
    newNode->next = NULL;

    if (baseListHead == NULL)
    {
        baseListHead = newNode;
        baseListCurrent = baseListHead;
    }
    else
    {
        baseListCurrent->next = newNode;
        baseListCurrent = baseListCurrent->next;
    }
}

void updateBaseEnemy(void* s)
{
	SpriteBase* ptr = ((SpriteBase*) s);

	ptr->x += ptr->dx;
	p->sprite->moveBy(ptr->sprite, ptr->dx, 0);
}

void createBaseEnemy(LCDBitmap* bmp)
{
    SpriteBase* baseEnemy = realloc(NULL, sizeof(SpriteBase));
	LCDSprite* baseSprite = p->sprite->newSprite();
	p->sprite->setImage(baseSprite, bmp, kBitmapUnflipped);	
	
	int w,h;
	p->graphics->getBitmapData(bmp, &w, &h, NULL, NULL, NULL);
	PDRect cr = PDRectMake(0, 0, w, h);
	p->sprite->setCollideRect(baseSprite, cr);	
	p->sprite->moveTo(baseSprite, 360, (rand() % 200) + 50);
	p->sprite->addSprite(baseSprite);

	baseEnemy->sprite = baseSprite;
	baseEnemy->dx = -3;
	baseEnemy->spriteUpdate = updateBaseEnemy;
    
    addBaseEnemy(baseEnemy);
}

int updatePlayer()
{
    player->sb->spriteUpdate(player);
    return 1;
}

void updateSpriteLists(int deltaTime)
{
    delTime = deltaTime;

    PDRect tempRect;
    PDRect playerRect = p->sprite->getBounds(player->sb->sprite);
    BaseListNode* baseListNode = baseListHead;
    
    updatePlayer();
    updateBackground();

    if (baseListNode != NULL)
    {
        while (baseListNode != NULL)
        {
            baseListNode->enemy->spriteUpdate(baseListNode->enemy);

            tempRect = p->sprite->getBounds(baseListNode->enemy->sprite);
            if (isColliding(&playerRect, &tempRect))
            {
                p->sprite->removeSprite(baseListNode->enemy->sprite);
                
                if (!isDamaged)
                {
                    p->sprite->setImage(player->sb->sprite, blinkBMP, kBitmapUnflipped);
                    playerSpeed = 10;
                    blinking = 1;
                    isDamaged = 1;
                }
            }
            baseListNode = baseListNode->next;
        }
    }
}
