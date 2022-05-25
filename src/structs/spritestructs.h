#include "pd_api.h"

//////////////////////////////////////////
//  Sprite defintions
//////////////////////////////////////////

typedef enum {
  Player,
  Enemy
} SpriteTypes;

typedef enum {
    Coupe,
    Sedan,
    Van,
    Bus,
    Truck
} EnemyTypes;

typedef struct SpriteBase {
  LCDBitmap* bmp;
  LCDSprite* sprite;
  SpriteTypes type;
  int x,y;
  int dx,dy;
  int (*spriteUpdate) (void *);
} SpriteBase;

typedef struct SpritePlayer {
  SpriteBase *sb;
  int distanceTraveledSP;
  int health;
} SpritePlayer;

//////////////////////////////////////////
//  Level defintions
//////////////////////////////////////////

typedef struct LevelEnemyData {
    int launchDistance;
    int launchX;
    int launchY;
    EnemyTypes type;

} LevelEnemyData;

typedef struct LevelMapData {
    int numMaps;
    LCDBitmap **maps;
} LevelMapData;

typedef struct LevelData {
    int length;
    int enemeyIndex;
    int numEnemies;

    LevelEnemyData enemies[100];
} LevelData;