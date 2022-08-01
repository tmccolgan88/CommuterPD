/*
  sprites/enemygenerator.c

  Creates the level enemies and update functions
*/

#include "../globals.h"
#include "../tools/tools.h"
#include "../structs/spritestructs.h"
#include "../particles/particles.h"
#include "enemygenerator.h"

//internal prototypes
int loadBitmaps(void);
void updateBaseEnemy(void *);

LCDBitmap** bmpseg;

int loadBitmapsEG()
{
  bmpseg = p->system->realloc(NULL, sizeof(LCDBitmap *) * 2);
  bmpseg[0] = loadImageAtPath("images/commuter");
  bmpseg[1] = loadImageAtPath("images/bigrig");
 }

/*
  Initialize the enemey generator.

  @return - void
*/
void _enemyGeneratorInitialize()
{
  loadBitmapsEG();
}

void destroyBaseEnemy()
{

}

void updateBaseEnemy(void* s)
{
	SpriteBase* ptr = ((SpriteBase*) s);

	ptr->x += ptr->dx;
	p->sprite->moveBy(ptr->sprite, ptr->dx, 0);
}

SpriteBase* createBaseEnemy(EnemyTypes enemyType, int y)
{
    SpriteBase* baseEnemy = realloc(NULL, sizeof(SpriteBase));
	LCDSprite* baseSprite = p->sprite->newSprite();
	p->sprite->setImage(baseSprite, bmpseg[0], kBitmapUnflipped);	
	
	int w,h;
	p->graphics->getBitmapData(bmpseg[0], &w, &h, NULL, NULL, NULL);
	PDRect cr = PDRectMake(0, 0, w, h);
	p->sprite->setCollideRect(baseSprite, cr);	
	p->sprite->moveTo(baseSprite, 410, y);
	p->sprite->addSprite(baseSprite);

	baseEnemy->sprite = baseSprite;
	baseEnemy->dx = -3;
	baseEnemy->spriteUpdate = updateBaseEnemy;
    p->sprite->setUpdateFunction(baseSprite, updateBaseEnemy);

    return baseEnemy;
}