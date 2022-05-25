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
#include "globals.h"
#include "tools/tools.h"
#include "structs/spritestructs.h"
#include "sprites/spriteengine.h"
#include "levels/levelengine.h"

static int update(void* userdata);
void setupGame(void);

typedef enum {
  Play,
  Pause,
  GameOver
} GameState;

//PlaydateAPI* p = NULL;

static GameState gameState = Play;

SpritePlayer* commuter = NULL;
SpriteBase* tempBase = NULL;

LCDBitmap* commuterBMP;
LCDBitmap* baseEnemyBMP;

int lastTime = 0;
int deltaTime = 0;

int addEnemyTime = 0;
int addEnemyTimer = 3500;

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
	char line[20];
	loadAssets();
	//setPlaydateAPISE(p);
	//setPlaydateAPILE(p);
	loadLevel(1);
	createBackground();
	createPlayer(commuterBMP);
	createBaseEnemy(baseEnemyBMP);

	distanceTraveled = 0;
	deltaTime = p->system->getCurrentTimeMilliseconds() - deltaTime;

	SDFile* file = p->file->open("level1.txt", kFileReadData);
    //p->system->logToConsole("file error : %s", p->file->geterr());
    p->file->read(file, line, 20);
    //p->system->logToConsole("file error : %s", p->file->geterr());
    p->system->logToConsole("main : %s", line);
    p->file->close(file);

}

int updatePlay(void* userdata)
{
	int saveTime = p->system->getCurrentTimeMilliseconds();
	deltaTime = saveTime - lastTime;
	lastTime = saveTime;

	addEnemyTime += deltaTime;
	if (addEnemyTime > addEnemyTimer)
	{
		addEnemyTime = 0;
		createBaseEnemy(baseEnemyBMP);
	}
	updatePlayer();
	updateSpriteLists(deltaTime);
	p->sprite->drawSprites();

	//p->system->logToConsole("%d", distanceTraveled);
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

