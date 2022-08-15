#include "pd_api.h"

//////////////////////////////////////////
//  Sprite defintions
//////////////////////////////////////////

typedef enum {
  Player,
  Enemy,
  Base
} SpriteTypes;

//idk if jam is the best solution
typedef enum {
    Coupe,
    CoupeJam,
    Sedan,
    SedanJam,
    Van,
    VanJam,
    Bus,
    BusJam,
    Truck,
    TruckJam,
    BigRig,
    BigRigJam
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
  int points;
  int health;
  int flashes;
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