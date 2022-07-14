//prototypes
void setPlaydateAPISE(PlaydateAPI* pd);
int isColliding(PDRect*, PDRect*);
int updateBackground(void);
void drawBackground(void);
void createBackground(void);
void setLaneWidth(int width);
int updateCommuter(void*);
void createPlayer(LCDBitmap*);
int updatePlayer(void);
void updateSpriteLists(int deltaTime);
void addBaseEnemy(SpriteBase* enemy);
void createBaseEnemy(LCDBitmap* bmp);
void createBaseEnemyWithY(LCDBitmap* bmp, int y);

//structs
typedef struct BaseListNode {
    SpriteBase* enemy;
    struct BaseListNode* next;
} BaseListNode;