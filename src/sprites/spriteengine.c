/*
  Author : Tim Mccolgan
  Date   : 05/13/2022

  Purpose : Logic to create/update level enemies. 
*/

#include "../structs/spritestructs.h"
#include "spriteengine.h"
#include "backgroundgenerator.h"
#include "commuter.h"
#include "enemygenerator.h"
#include "../tools/tools.h"
#include "../particles/particles.h"
#include "../globals.h"

int delTime = 0;
LCDBitmap* commuterBMP = NULL;
SpritePlayer* player = NULL;
BaseListNode* baseListHead = NULL;
BaseListNode* baseListCurrent = NULL;

//functions

/*
*  Logic to see if 2 PDRects are colliding
*
* @return int - 0 if false 1 if true
*/
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

/*
*  Initialize the sprite engine.
*
*  @return - void
*/
void _spriteEngineInitialize()
{
    _backgroundGeneratorInitialize();
	_enemyGeneratorInitialize();
    createBackground();
    createCommuter();
}

/*
*  Function file  - commuter.c
*
*  Create the player character.
*
*  @return - void
*/
void createCommuter()
{
    createPlayer();
}

/*
* Make a call to the enemy generator to create a sprite and
*   add it to the sprite list
*
* @param enum enemyType - Type of enemy to add
* @param int y - The y to launch the enemy
*/
void addBaseEnemy(EnemyTypes enemyType, int y)
{
    BaseListNode* newNode = p->system->realloc(NULL, sizeof(BaseListNode));
    newNode->enemy = createBaseEnemy(Coupe, y);
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

/*
*  Update all the sprites in the current scene. Delete sprites where neccessary
*
*  @param deltaTime - time passed since last draw
*  @return - void
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
                destroyBaseEnemy(baseListNode->enemy->x, baseListNode->enemy->y);
                baseListNode->enemy->x = -50;
                baseListNode->enemy->y = -50;
                p->sprite->moveTo(baseListNode->enemy->sprite, baseListNode->enemy->x, baseListNode->enemy->y);
                p->sprite->removeSprite(baseListNode->enemy->sprite);
            }
            baseListNode = baseListNode->next;
        }
    }
} //updateSpriteLists