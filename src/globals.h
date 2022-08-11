#include "pd_api.h"

#define ENEMY_X_START 410
#define JAM_BUFFER 35

typedef enum {
  Play,
  Pause,
  GameOver
} GameState;

GameState gameState;

PlaydateAPI* p;
int* distanceTraveled;


//LCDBitmap enemyAssets[5];
//LCDBitmap particleAssets[5];