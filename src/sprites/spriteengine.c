/*
  Author : Tim Mccolgan
  Date   : 05/13/2022

  Purpose : Logic to create/update level enemies. 
*/

#include "../structs/spritestructs.h"
#include "spriteengine.h"
#include "commuter.h"
#include "../tools/tools.h"
#include "../particles/particles.h"
#include "../globals.h"

int delTime = 0;
LCDBitmap* bgBMP = NULL;
LCDSprite* bgSprite = NULL;
int bgWidth, bgHeight = 0;
int bgx, bgy = 0;
LCDBitmap* commuterBMP = NULL;
LCDBitmap* blinkBMP = NULL;
LCDBitmap* teleportParticleBMP = NULL;
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

void _spriteEngineInitialize()
{
    createCommuter();
    bgBMP = loadImageAtPath("images/tempbackground1");
    createBackground();
}

int updateBackground()
{
    bgx -= 10; //need to reflect player speed
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

	p->graphics->getBitmapData(bgBMP, &bgWidth, &bgHeight, NULL, NULL, NULL);

	p->sprite->setUpdateFunction(bgSprite, updateBackground);
	p->sprite->setDrawFunction(bgSprite, drawBackground);

	PDRect bgBounds = PDRectMake(0, 0, bgWidth, bgHeight);
	p->sprite->setBounds(bgSprite, bgBounds);

	p->sprite->setZIndex(bgSprite, 0);

	p->sprite->addSprite(bgSprite);
    
}

/*
  Function file  - commuter.c

  Create the player character.

  @return - void
*/
void createCommuter()
{
    createPlayer();
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
	p->sprite->moveTo(baseSprite, 410, (rand() % 200) + 50);
	p->sprite->addSprite(baseSprite);

	baseEnemy->sprite = baseSprite;
	baseEnemy->dx = -3;
	baseEnemy->spriteUpdate = updateBaseEnemy;
    p->sprite->setUpdateFunction(baseSprite, updateBaseEnemy);
    addBaseEnemy(baseEnemy);
}

void createBaseEnemyWithY(LCDBitmap* bmp, int y)
{
    SpriteBase* baseEnemy = realloc(NULL, sizeof(SpriteBase));
	LCDSprite* baseSprite = p->sprite->newSprite();
	p->sprite->setImage(baseSprite, bmp, kBitmapUnflipped);	
	
	int w,h;
	p->graphics->getBitmapData(bmp, &w, &h, NULL, NULL, NULL);
	PDRect cr = PDRectMake(0, 0, w, h);
	p->sprite->setCollideRect(baseSprite, cr);	
	p->sprite->moveTo(baseSprite, 410, y);
	p->sprite->addSprite(baseSprite);

	baseEnemy->sprite = baseSprite;
	baseEnemy->dx = -3;
	baseEnemy->spriteUpdate = updateBaseEnemy;
    
    addBaseEnemy(baseEnemy);
}

/*
  Update all the sprites in the current scene. Delete sprites where neccessary

  @param deltaTime - time passed since last draw
  @return - void
*/
void updateSpriteLists(int deltaTime)
{
    delTime = deltaTime;

    PDRect tempRect;
    PDRect playerRect = getCommuterRect();
    BaseListNode* baseListNode = baseListHead;
    
    updateBackground();
    updateCommuter(delTime);
    updateParticles(deltaTime);
    
    if (baseListNode != NULL)
    {
        while (baseListNode != NULL)
        {
            baseListNode->enemy->spriteUpdate(baseListNode->enemy);

            tempRect = p->sprite->getBounds(baseListNode->enemy->sprite);
            if (isColliding(&playerRect, &tempRect))
            {
                //isColliding keeps returning true becaseu the sprite isn't being removed from the
                //custom sprite list
                setDamaged(1);
                //TODO - actually delete the sprite rather than moving it out of the way
                baseListNode->enemy->x = -50;
                baseListNode->enemy->y = -50;
                p->sprite->moveTo(baseListNode->enemy->sprite, baseListNode->enemy->x, baseListNode->enemy->y);
                p->sprite->removeSprite(baseListNode->enemy->sprite);
            }
            baseListNode = baseListNode->next;
        }
    }
} //updateSpriteLists