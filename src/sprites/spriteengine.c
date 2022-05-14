/*
  Author : Tim Mccolgan
  Date   : 05/13/2022

  Purpose : Logic to create/update level enemies. 
*/

#include "pd_api.h"
#include "../structs/spritestructs.h"
#include "spriteengine.h"

//fields
PlaydateAPI* p;
BaseListNode* baseListHead = NULL;
BaseListNode* baseListCurrent = NULL;

//functions
void setPlaydateAPISE(PlaydateAPI* pd)
{
    p = pd;
}

void addBaseEnemy(SpriteBase* enemy)
{
    BaseListNode* newNode = p->system->realloc(NULL, sizeof(BaseListNode));
    newNode->enemy = enemy;
    newNode->next = NULL;

    if (baseListHead == NULL)
    {
        baseListHead = newNode;
        baseListCurrent = baseListHead->next;
    }
    else
    {
        baseListCurrent = newNode;
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
	p->sprite->moveTo(baseSprite, 360, 50);
	p->sprite->addSprite(baseSprite);

	baseEnemy->sprite = baseSprite;
	baseEnemy->dx = -3;
	baseEnemy->spriteUpdate = updateBaseEnemy;
    
    addBaseEnemy(baseEnemy);
}

void updateSpriteLists()
{
    BaseListNode* baseListNode = baseListHead;

    if (baseListNode != NULL)
    {
        while (baseListNode != NULL)
        {
            baseListNode->enemy->spriteUpdate(baseListNode->enemy);
            baseListNode = baseListNode->next;
        }
    }
}
