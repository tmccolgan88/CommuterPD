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
#include "sprites/backgroundgenerator.h"
#include "sprites/commuter.h"
#include "sprites/enemygenerator.h"
#include "ui/ui.h"
#include "levels/levelengine.h"
#include "particles/particles.h"

static int update(void* userdata);
void loadAssets(void);
void setupGame(void);

typedef enum {
  Play,
  Pause,
  GameOver
} GameState;

static GameState gameState = Play;

SpriteBase* tempBase = NULL;

int lastTime = 0;
int deltaTime = 0;

#ifdef _WINDLL
__declspec(dllexport)
#endif

/*
*  This function is needed to hi jack the lua runtime and
*    write the entire game in c.
*
*	@param *pd - the playdate API
*	@param event - the runtime event
*	@param arg - some arg or some shit
*/
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		// Note: If you set an update callback in the kEventInit handler, 
		//       the system assumes the game is pure C and doesn't run any Lua code in the game
		p = pd;
		pd->system->setUpdateCallback(update, pd);
		setupGame();
	}
	
	return 0;
}

/**/
void setupGame()
{
	_spriteEngineInitialize();
	removeAllParticles();
	loadLevel(1);
 
	distanceTraveled = 0;
	deltaTime = p->system->getCurrentTimeMilliseconds() - deltaTime;
}

/*
*  Update loop for the Play state
*
*  @param userdata - some bullshit
*/
int updatePlay(void* userdata)
{
	int saveTime = p->system->getCurrentTimeMilliseconds();
	deltaTime = saveTime - lastTime;
	lastTime = saveTime;

	updateLevel(); 
	updateSpriteLists(deltaTime);
	p->sprite->drawSprites();
    drawParticles();
	drawHUD();
	return 1;
} //updatePlay

/*
*  Main update loop. Clears the screen and calls update
*    for the current state of the game
*
*  @param userData - playdate data 
*/
static int update(void* userdata)
{
	p->graphics->clear(kColorWhite);

	if (gameState == Play)
	  updatePlay(userdata);

	p->system->drawFPS(5, 5);

	return 1;
} //update

