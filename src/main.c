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
#include "tools/tools.h"
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

static GameState gameState = Play;

SpritePlayer* commuter = NULL;
SpriteBase* tempBase = NULL;

LCDBitmap* commuterBMP;
LCDBitmap* baseEnemyBMP;

int lastTime = 0;
int deltaTime = 0;

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

void loadAssets()
{
	commuterBMP = loadImageAtPath("images/commuter", p);
	baseEnemyBMP = loadImageAtPath("images/commuter", p); //TODO base enemy sprite
}

void setupGame()
{
	loadAssets();
	setPlaydateAPISE(p);
	createBackground();
	createPlayer(commuterBMP);
	createBaseEnemy(baseEnemyBMP);

	deltaTime = p->system->getCurrentTimeMilliseconds() - deltaTime;

}

int updatePlay(void* userdata)
{
	int saveTime = p->system->getCurrentTimeMilliseconds();
	deltaTime = saveTime - lastTime;
	lastTime = saveTime;

	updatePlayer();
	updateSpriteLists(deltaTime);
	p->sprite->drawSprites();

	return 1;
}

static int update(void* userdata)
{
	p->graphics->clear(kColorWhite);

	if (gameState == Play)
	  updatePlay(userdata);

	p->system->drawFPS(5, 5);

	return 1;
}

