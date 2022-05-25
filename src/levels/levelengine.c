/*
    levelengine.c
    Author : Tim McColgan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pd_api.h"
#include "../globals.h"
#include "../structs/spritestructs.h"

//PlaydateAPI* ple = NULL;
LevelData* level = NULL;

int loadLevel(int levelNum)
{
    char temp[5];

    LevelData* tempLevelData = p->system->realloc(NULL, sizeof(LevelData));
    SDFile* file = p->file->open("level1.txt", kFileReadData);
    //p->system->logToConsole("file error : %s", p->file->geterr());
    p->system->logToConsole("level engine strlen %d", strlen(temp));
    p->file->read(file, temp, strlen(temp));
    //p->system->logToConsole("file error : %s", p->file->geterr());
    p->system->logToConsole("level engine : %s", temp);
    p->file->close(file);



    return 1;
}