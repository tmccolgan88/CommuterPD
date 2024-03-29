#include "pd_api.h"

//structs
typedef struct BaseListNode {
    SpriteBase* enemy;
    struct BaseListNode* next; 
} BaseListNode;

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
void addBaseEnemyJam(EnemyTypes enemyType, int y, int count);
void addEnemyToList(BaseListNode* node);
void createBaseEnemyWithY(LCDBitmap* bmp, int y);
void createBigRigWithY(LCDBitmap* bmp, int y);