#include "pd_api.h"

typedef enum {
  Player,
  Enemy
} SpriteTypes;

typedef struct SpriteBase {
  LCDBitmap* bmp;
  LCDSprite* sprite;
  SpriteTypes type;
  int x,y;
  int dx,dy;
  int (*spriteUpdate) (void*);
} SpriteBase;

typedef struct SpritePlayer {
  SpriteBase *sb;
  int health;
} SpritePlayer;