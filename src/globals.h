#include "pd_api.h"

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