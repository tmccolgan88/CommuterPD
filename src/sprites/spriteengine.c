/*
  Author : Tim Mccolgan
  Date   : 05/13/2022

  Purpose : Logic to create/update level enemies. 
*/

#include "pd_api.h"
#include "../structs/spritestructs.h"
#include "spriteengine.h"
#include "../tools/tools.h"

//fields
PlaydateAPI* p;

LCDBitmap* bgBMP = NULL;
LCDSprite* bgSprite = NULL;
int bgWidth, bgHeight = 0;
int bgx, bgy = 0;
SpritePlayer* player = NULL;
BaseListNode* baseListHead = NULL;
BaseListNode* baseListCurrent = NULL;

//functions
void setPlaydateAPISE(PlaydateAPI* pd)
{
    p = pd;
}

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
    bgx -= 10;
	if ( bgx < ((bgWidth) * -1)) {
		bgx = 0;
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

	PDRect bgBounds = PDRectMake(0, 0, 800, 300);
	p->sprite->setBounds(bgSprite, bgBounds);

	p->sprite->setZIndex(bgSprite, 0);

	p->sprite->addSprite(bgSprite);
    
}

int updateCommuter(void *s)
{
	SpritePlayer* playerPtr = ((SpriteBase*) s);
	PDButtons current;
	int deltaX = 0;
	int deltaY = 0;

	p->system->getButtonState(&current, NULL, NULL);	
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

	int collisionLen = 0;
	float x, y = 0;
	p->sprite->getPosition(playerPtr->sb->sprite, &x, &y);
	SpriteCollisionInfo* collisions = p->sprite->moveWithCollisions(playerPtr->sb->sprite, x + deltaX, y + deltaY, NULL, NULL, &collisionLen);

    PDRect pdr = p->sprite->getBounds(playerPtr->sb->sprite);
	return 1;
}

void createPlayer(LCDBitmap* bmp)
{
	SpriteBase* base = p->system->realloc(NULL, sizeof(SpriteBase));
	SpritePlayer* spritePlayer = p->system->realloc(NULL, sizeof(SpritePlayer));

	LCDSprite* tempSprite = p->sprite->newSprite();

	p->sprite->setImage(tempSprite, bmp, kBitmapUnflipped);
	
	int w,h;
	p->graphics->getBitmapData(bmp, &w, &h, NULL, NULL, NULL);
	PDRect cr = PDRectMake(0, 0, w, h);
	p->sprite->setCollideRect(tempSprite, cr);

	p->sprite->moveTo(tempSprite, 50, 50);
	p->sprite->addSprite(tempSprite);

	base->sprite = tempSprite;
    base->spriteUpdate = updateCommuter;
	
	spritePlayer->sb = base;
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

void updateSpriteLists()
{
    PDRect tempRect;
    PDRect playerRect = p->sprite->getBounds(player->sb->sprite);
    BaseListNode* baseListNode = baseListHead;

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
            }
            baseListNode = baseListNode->next;
        }
    }
}
