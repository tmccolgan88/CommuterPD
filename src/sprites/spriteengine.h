#include "pd_api.h"

//prototypes
void _spriteEngineInitialize(void);
int isColliding(PDRect*, PDRect*);
int updateBackground(void);
void drawBackground(void);
void createBackground(void);
void setLaneWidth(int width);
void setTopOfRoad(int top);
void setBottomOfRoad(int bottom);
void createCommuter(void);
int updatePlayer(void);
void updateSpriteLists(int deltaTime);
void addBaseEnemy(EnemyTypes enemyType, int y);
//void createBaseEnemy(LCDBitmap* bmp);
void createBaseEnemyWithY(LCDBitmap* bmp, int y);
void createBigRigWithY(LCDBitmap* bmp, int y);

//structs
typedef struct BaseListNode {
    SpriteBase* enemy;
    struct BaseListNode* next; 
} BaseListNode;