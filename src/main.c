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

static int update(void* userdata);
void setupGame(void);

typedef enum {
  Play,
  Pause,
  GameOver
} GameState;

PlaydateAPI* p = NULL;

GameState gameState = Play;

SpriteBase* base = NULL;
SpritePlayer* player = NULL;

int ix = 0;

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

int updatePlayer(void* userdata)
{
	p->graphics->drawText("function pointer", strlen("function pointer"), kASCIIEncoding, ix++, 5);
	return 1;
}

void setupGame()
{
  SpriteBase *playerBase = p->system->realloc(NULL, sizeof(SpriteBase));
  playerBase->x = 50;
  playerBase->y = 50;
  playerBase->spriteUpdate = updatePlayer;

  base = playerBase;
}

int updatePlay(void* userdata)
{
	base->spriteUpdate(userdata);
	return 1;
}
static int update(void* userdata)
{
	p->graphics->clear(kColorWhite);
	
	if (gameState == Play)
	  updatePlay(userdata);

	return 1;
}

