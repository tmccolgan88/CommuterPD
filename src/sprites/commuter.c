/*
  sprites/commuter.c

  The player character.  All drawing and control logic contained in this program
*/

#include "../globals.h"
#include "commuter.h"
#include "../tools/tools.h"
#include "../structs/spritestructs.h"
#include "spriteengine.h"
#include "../particles/particles.h"

//internal prototypes
int loadBitmapsC(void);

LCDBitmap** bmpsc;
SpritePlayer* player;

//meta fields
int commuterHeight = 0;
int commuterWidth = 0;
int commWidthOffset = 0;

//player state fields
int playerHealth = 3;
int playerSpeed = 20;
int canChangeSpeed = 1;
int canJump = 1;
int speedTime = 0;
int speedTimer = 1000;
int isDamaged = 0;
int blinking = 0;
int blinkTime = 0;
int blinkTimer = 150;
int damageTime = 0;
int damageTimer = 1350;

//level related fields
int laneSize = 0;
int topOfRoad = 0;
int bottomOfRoad = 0;

/*
*  Load the bitmaps used by the commuter
*
*  @param - void
*  @return - int (status code)
*/
int loadBitmapsC()
{
  bmpsc = p->system->realloc(NULL, sizeof(LCDBitmap *) * 3);
  bmpsc[0] = loadImageAtPath("images/commuter");
  bmpsc[1] = loadImageAtPath("images/blink");
  bmpsc[2] = loadImageAtPath("images/teleport_particle");
} //loadBitmaps


/////////////////////////////
// Start Getters & Setters
////////////////////////////

/*
*  Return the collision rect of the player
*
*  @return PDRect* - a pointer to the collision rect 
*/
PDRect getCommuterRect()
{
  return p->sprite->getBounds(player->sb->sprite);
} //getPDRect()

/*
*  Return the distance traveled / 100
*
*  @return int - disctance traveled / 100 (score)
*/
int getDistanceTraveled()
{
    if (player == NULL)
      return -1;

    return player->distanceTraveledSP / 100;
} //getDistanceTraveled

/*
*  Return the current player speed
*
*  @return - int the players speed
*/
int getSpeed()
{
    if (player == NULL)
        return -1;

    return playerSpeed;
}

/*
*  Set the size of the lane provided by the level json
*
*  @param _laneSize - the width of the lane
*  @return - void
*/
void setLaneWidth(int _laneSize)
{
    laneSize = _laneSize;
} //setLaneSize

void setTopOfRoad(int _topOfRoad)
{
    topOfRoad = _topOfRoad;
} //setTopOfRoad

void setBottomOfRoad(int _bottomOfRoad)
{
    bottomOfRoad = _bottomOfRoad;
} //setBottomOfRoad

void setDamaged (int _isDamaged)
{
    if (!isDamaged && _isDamaged)
    {
        isDamaged = _isDamaged;
        blinking = 1;
        p->sprite->setImage(player->sb->sprite, bmpsc[1], kBitmapUnflipped);
    }
} //setDamaged

/////////////////////////////
// End Getters & Setters
////////////////////////////


/*
  update the player character

  @param delTime - Time passed since last frame
  @return - int (status code)
*/
int updateCommuter(int delTime)
{
    SpritePlayer* playerPtr = player;
	PDButtons current, pushed, released;
    float crankChange = 0;
	int deltaX = 0;
	int deltaY = 0;

	p->system->getButtonState(&current, &pushed, &released);
    crankChange = p->system->getCrankChange();

    if (speedTime <= speedTimer)
      speedTime += delTime;

    if (speedTime > speedTimer)
    {
        canChangeSpeed = 1;
        speedTime = 0;
    }

    //change speed logic
    if (crankChange && canChangeSpeed)
    {
        if (crankChange > 0)
        {
            if (playerSpeed < 20)
                playerSpeed += 2;
        }
        else
        {
            if (playerSpeed > 4)
                playerSpeed -= 2;
        }
        canChangeSpeed = 0;
    }	

    //linear movement logic
	if (kButtonUp & current)
	{
		deltaY = -3;
	}
	if (kButtonDown & current)
	{	
		deltaY = 3;	  
	}
	if (kButtonLeft & current)
	{ 
		deltaX = -3;
	}
	if (kButtonRight & current)
	{	
		deltaX = 3;	  
	}

    //lane warping logic
    if ((kButtonA & pushed) && canJump)
    {
        deltaY = -laneSize;
        addTeleportParticleBurst(bmpsc[2], playerPtr->sb->x, playerPtr->sb->y);
        p->sprite->setImage(playerPtr->sb->sprite, bmpsc[1], kBitmapUnflipped);
        canJump = 0;
    }
    if ((kButtonB & pushed) && canJump)
    {
        deltaY = laneSize;
        addTeleportParticleBurst(bmpsc[2], playerPtr->sb->x, playerPtr->sb->y);
        p->sprite->setImage(playerPtr->sb->sprite, bmpsc[1], kBitmapUnflipped);
        canJump = 0;
    }

    if ((kButtonA & released) && !canJump)
    {
        canJump = 1;
        p->sprite->setImage(playerPtr->sb->sprite, bmpsc[0], kBitmapUnflipped);
    }
    if ((kButtonB & released) && !canJump)
    {
        canJump = 1;
        p->sprite->setImage(playerPtr->sb->sprite, bmpsc[0], kBitmapUnflipped);
    } 

    //damage state logic
    if  (isDamaged)
    {
        canJump = 0;
        deltaX = -1;
        deltaY = 0;
        blinkTime += delTime;
        if (blinkTime >= blinkTimer)
        {
            blinkTime = 0;
            if (blinking)
            {
                p->sprite->setImage(playerPtr->sb->sprite, bmpsc[0], kBitmapUnflipped);
                blinking = 0;
            } 
            else
            {
                p->sprite->setImage(playerPtr->sb->sprite, bmpsc[1], kBitmapUnflipped);
                blinking = 1;
            }
        }

        damageTime += delTime;
        if (damageTime >= damageTimer)
        {
            p->sprite->setImage(playerPtr->sb->sprite, bmpsc[0], kBitmapUnflipped);
            canJump = 1;
            isDamaged = 0;
            damageTime = 0;
        }
    }

    //make sure the player is in the screen bounds
    float x,y;
    p->sprite->getPosition(playerPtr->sb->sprite, &x, &y);

    x += (float) deltaX;
    y += (float) deltaY;

    if (x < commWidthOffset)
        x = commWidthOffset;
    if (x > 400 - commWidthOffset)
        x = 400 - commWidthOffset;
    if (y < topOfRoad)
        y = topOfRoad;
    if (y > bottomOfRoad - commuterHeight)
        y = bottomOfRoad - commuterHeight;

    p->sprite->moveTo(playerPtr->sb->sprite, x, y);
    playerPtr->sb->x = x;
    playerPtr->sb->y = y;
    playerPtr->distanceTraveledSP += playerSpeed;
    distanceTraveled += playerSpeed;
   
	return 1;
} //updatePlayer

void commuterDamage()
{
  playerHealth--;
  if (playerHealth == -1)
  {
      ;
    //gameOver
    //gameState = GameOver;
  }

  //isDamaged = 1;

}

/*
*  Create the player character, initialize all related sprite data
*
*  @param - void
*  @return - void
*/
void createPlayer()
{   
    loadBitmapsC();

	SpriteBase* base = p->system->realloc(NULL, sizeof(SpriteBase));
	SpritePlayer* spritePlayer = p->system->realloc(NULL, sizeof(SpritePlayer));

	LCDSprite* playerSprite = p->sprite->newSprite();

	p->sprite->setImage(playerSprite, bmpsc[0], kBitmapUnflipped);
	
	p->graphics->getBitmapData(bmpsc[0], &commuterWidth, &commuterHeight, NULL, NULL, NULL);
    commWidthOffset = commuterWidth / 2;
    commuterHeight = commuterHeight / 2;

	PDRect cr = PDRectMake(0, 0, commuterWidth, commuterHeight);
	p->sprite->setCollideRect(playerSprite, cr);

	p->sprite->moveTo(playerSprite, 50, 50);
	p->sprite->addSprite(playerSprite);

	base->sprite = playerSprite;
	
	spritePlayer->sb = base;
    spritePlayer->distanceTraveledSP = 0;
	spritePlayer->health = 3;
 
	player = spritePlayer;
}