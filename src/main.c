//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "structs/spritestructs.h"
#include "sprites/spriteengine.h"

static int update(void* userdata);
void setupGame(void);

typedef enum {
  Play,
  Pause,
  GameOver
} GameState;

PlaydateAPI* p = NULL;

GameState gameState = Play;

SpritePlayer* commuter = NULL;
SpriteBase* tempBase = NULL;

LCDBitmap* baseEnemyBMP;

int ix = 0;
int idx = 0;

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		p = pd;
		pd->system->setUpdateCallback(update, pd);
		setupGame();
	}
	
	return 0;
}

LCDBitmap *loadImageAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDBitmap* img = p->graphics->loadBitmap(path, &outErr);
	if (outErr != NULL) 
	{
		p->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
	}
	return img;
}

int updatePlayer(void *s)
{
  SpritePlayer* playerPtr = ((SpriteBase*) s);
  PDButtons current;
  p->system->getButtonState(&current, NULL, NULL);

  if (kButtonUp & current)
  {
    p->sprite->moveBy(playerPtr->sb->sprite, 0, -1);
  }
  if (kButtonDown & current)
  {
	  p->sprite->moveBy(playerPtr->sb->sprite, 0, 1);
  }
  
  return 1;
}

/*void updateBaseEnemy(void* s)
{
	SpriteBase* ptr = ((SpriteBase*) s);

	ptr->x += ptr->dx;
	p->sprite->moveBy(ptr->sprite, ptr->dx, 0);
}

void createBaseEnemy()
{
	SpriteBase* baseEnemy = realloc(NULL, sizeof(SpriteBase));
	LCDSprite* baseSprite = p->sprite->newSprite();
	p->sprite->setImage(baseSprite, baseEnemyBMP, kBitmapUnflipped);	
	
	int w,h;
	p->graphics->getBitmapData(baseEnemyBMP, &w, &h, NULL, NULL, NULL);
	PDRect cr = PDRectMake(0, 0, w, h);
	p->sprite->setCollideRect(baseSprite, cr);	
	p->sprite->moveTo(baseSprite, 360, 50);
	p->sprite->addSprite(baseSprite);

	baseEnemy->sprite = baseSprite;
	baseEnemy->dx = -3;
	baseEnemy->spriteUpdate = updateBaseEnemy;
	tempBase = baseEnemy;

}*/

void createPlayer()
{
	SpriteBase* base = p->system->realloc(NULL, sizeof(SpriteBase));
	SpritePlayer* spritePlayer = p->system->realloc(NULL, sizeof(SpritePlayer));

	LCDSprite* tempSprite = p->sprite->newSprite();
	LCDBitmap* bmp = loadImageAtPath("images/commuter");

	p->sprite->setImage(tempSprite, bmp, kBitmapUnflipped);
	
	int w,h;
	p->graphics->getBitmapData(bmp, &w, &h, NULL, NULL, NULL);
	PDRect cr = PDRectMake(0, 0, w, h);
	p->sprite->setCollideRect(tempSprite, cr);

	p->sprite->moveTo(tempSprite, 50, 50);
	p->sprite->addSprite(tempSprite);

	base->sprite = tempSprite;
    base->spriteUpdate = updatePlayer;
	
	spritePlayer->sb = base;
	spritePlayer->health = 3;
 
	commuter = spritePlayer;
}

void loadAssets()
{
	baseEnemyBMP = loadImageAtPath("images/commuter"); //TODO base enemy sprite
}

void setupGame()
{
	loadAssets();
	setPlaydateAPISE(p);
	createPlayer();
	createBaseEnemy(baseEnemyBMP);
}

int updatePlay(void* userdata)
{
	commuter->sb->spriteUpdate(commuter);
	updateSpriteLists();
	p->sprite->drawSprites();
	return 1;
}

static int update(void* userdata)
{
	p->graphics->clear(kColorWhite);

	if (gameState == Play)
	  updatePlay(userdata);

	return 1;
}

