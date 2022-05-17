//prototypes
void setPlaydateAPISE(PlaydateAPI* pd);
int isColliding(PDRect*, PDRect*);
int updateBackground(void);
void drawBackground(void);
void createBackground(void);
int updateCommuter(void*);
int updatePlayer(void);
void createPlayer(LCDBitmap*);
void addBaseEnemy(SpriteBase* enemy);
void createBaseEnemy(LCDBitmap* bmp);
void updateSpriteLists(void);

//structs
typedef struct BaseListNode {
    SpriteBase* enemy;
    struct BaseListNode* next;
} BaseListNode;