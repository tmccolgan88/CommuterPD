//prototypes
void setPlaydateAPISE(PlaydateAPI* pd);
void addBaseEnemy(SpriteBase* enemy);
void createBaseEnemy(LCDBitmap* bmp);
void updateSpriteLists(void);

//structs
typedef struct BaseListNode {
    SpriteBase* enemy;
    struct BaseListNode* next;
} BaseListNode;