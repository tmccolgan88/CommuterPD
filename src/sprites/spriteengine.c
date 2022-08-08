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
    newNode->enemy = createBaseEnemy(enemyType, y);
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
*  Delete a base sprite that has been collided into
*
*  @param SpriteBase* enemyToDelete - pointer to a SpriteBase to delete
*  @return - void
*/
void removeBaseEnemy(SpriteBase* enemyToDelete)
{
    //p->system->realloc(enemyToDelete->bmp, 0);
    //p->system->realloc(enemyToDelete->type, 0);
    p->system->realloc(enemyToDelete, 0);
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
    BaseListNode* saveNode = baseListNode;

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
                setDamaged(1);
                destroyBaseEnemy(baseListNode->enemy->x, baseListNode->enemy->y);
                p->sprite->removeSprite(baseListNode->enemy->sprite);
                removeBaseEnemy(baseListNode->enemy);

                if (baseListNode == baseListHead)
                {
                    baseListHead = baseListHead->next;
                    p->system->realloc(baseListNode, 0);
                    baseListNode = baseListHead;
                    saveNode = baseListNode;
                }
                else if (baseListNode->next != NULL)
                {
                    saveNode->next = baseListNode->next;
                    p->system->realloc(baseListNode, 0);
                    baseListNode = saveNode;
                }
                else
                {
                    saveNode->next = NULL;
                    p->system->realloc(baseListNode, 0);
                    return;
                }
            }
            saveNode = baseListNode;
            baseListNode = baseListNode->next;
        } // while (baseListNode != NULL)
    }  //if (baseListNode != NULL)
} //updateSpriteLists