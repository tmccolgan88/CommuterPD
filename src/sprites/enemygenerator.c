/*
  sprites/enemygenerator.c

  Creates the level enemies and update functions

  Author : Tim McColgan
*/

#include "../globals.h"
#include "../tools/tools.h"
#include "../structs/spritestructs.h"
#include "../particles/particles.h"
#include "enemygenerator.h"

//internal prototypes
int loadBitmaps(void);
void updateBaseEnemy(void *);
int resolveDX(EnemyTypes);
LCDBitmap* resolveBitmap(EnemyTypes);


LCDBitmap** bmpseg;

/*
*  Load the bitmaps used by the enemy generator
*
*  @param - void
*  @return - int (status code, 1 - pass, 0 - fail)
*/
int loadBitmapsEG()
{
    bmpseg = p->system->realloc(NULL, sizeof(LCDBitmap *) * 3);
    bmpseg[0] = loadImageAtPath("images/coupe");
    bmpseg[1] = loadImageAtPath("images/bigrig");
    bmpseg[2] = loadImageAtPath("images/van");
    bmpseg[3] = loadImageAtPath("images/debris_particle1");

    return 1;
 }

/*
*  Initialize the enemey generator.
*
*  @return - void
*/
void _enemyGeneratorInitialize()
{
  loadBitmapsEG();
}

/*
* Replace the  enemy sprite with a particle burst
*
* return - void
*/
void destroyBaseEnemy(int x, int y)
{
    addParticleBurst(bmpseg[3], x, y);
}

/*
* Update the base enemy type
*
* return - void
*/
void updateBaseEnemy(void* s)
{
	  SpriteBase* ptr = ((SpriteBase*) s);
    int* actualY, actualX, len;
	  ptr->x += ptr->dx;
	  p->sprite->moveBy(ptr->sprite, ptr->dx, 0);
    //p->sprite->moveWithCollisions(ptr->sprite, ptr->x, ptr->y, &actualY, &actualX, &len);
    
}

/*
*  Return the speed of the enemy type
*
*  @param enemyType : the enum of the type of enemy
*
* r eturn int - the speed of the enemy type
*/
int resolveDX(EnemyTypes enemyType)
{
    switch(enemyType)
    {
      case Coupe : return -2;
      case Van : return -3;
      case BigRig : return -5;
      default : return -5;
    }

    return 0;
}

/*
*  Return Bitmap of the enemy type
*
*  @param enemyType : the enum of the type of enemy
*
* return LCDbitmap* - a pointer to the bitmap of the enemy type
*/
LCDBitmap* resolveBitmap(EnemyTypes enemyType)
{
    switch(enemyType)
    {
      case Coupe : return bmpseg[0];
      case BigRig : return bmpseg[1];
      case Van : return bmpseg[2];
      default : return bmpseg[3];
    }

    return NULL;
}

/*
*  Create the base enemy types
*
*  @param enemyType - the enum of the type of enemy
*  @param y - the y position the enemy spawns at
*
*  @param SpriteBase* - the SpriteBase object created
*/
SpriteBase* createBaseEnemy(EnemyTypes enemyType, int x, int y)
{
    SpriteBase* baseEnemy = realloc(NULL, sizeof(SpriteBase));
	  LCDSprite* baseSprite = p->sprite->newSprite();
	  LCDBitmap* bmpmy = resolveBitmap(enemyType);

    p->sprite->setImage(baseSprite, bmpmy, kBitmapUnflipped);	
	
	  int w,h;
	  p->graphics->getBitmapData(bmpmy, &w, &h, NULL, NULL, NULL);
	  PDRect cr = PDRectMake(0, 0, w, h);
	  p->sprite->setCollideRect(baseSprite, cr);	
	  p->sprite->moveTo(baseSprite, x, y);
	  p->sprite->addSprite(baseSprite);
  
	  baseEnemy->sprite = baseSprite;
    baseEnemy->x = x;
    baseEnemy->y = y;
	  baseEnemy->dx = resolveDX(enemyType);
	  baseEnemy->spriteUpdate = updateBaseEnemy;
    p->sprite->setUpdateFunction(baseSprite, updateBaseEnemy);

    return baseEnemy;
}