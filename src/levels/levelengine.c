/*
    levelengine.c
    Author : Tim McColgan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pd_api.h"
#include "../globals.h"
#include "../tools/tools.h"
#include "../structs/spritestructs.h"
#include "../sprites/spriteengine.h"

//PlaydateAPI* ple = NULL;
LevelData* level = NULL;
int currentEnemeyIndex = 0;

LCDBitmap* bmp = NULL;

void decodeError(json_decoder* decoder, const char* error, int linenum)
{
	p->system->logToConsole("decode error line %i: %s", linenum, error);
}

const char* typeToName(json_value_type type)
{
	switch ( type )
	{
		case kJSONNull: return "null";
		case kJSONTrue: return "true";
		case kJSONFalse: return "false";
		case kJSONInteger: return "integer";
		case kJSONFloat: return "float";
		case kJSONString: return "string";
		case kJSONArray: return "array";
		case kJSONTable: return "table";
		default: return "???";
	}
}

void willDecodeSublist(json_decoder* decoder, const char* name, json_value_type type)
{
	//p->system->logToConsole("%s willDecodeSublist %s %s", decoder->path, typeToName(type), name);
}

int shouldDecodeTableValueForKey(json_decoder* decoder, const char* key)
{
	//p->system->logToConsole("%s shouldDecodeTableValueForKey %s", decoder->path, key);
	return 1;
}

void didDecodeTableValue(json_decoder* decoder, const char* key, json_value value)
{
	//level data
    if (strcmp(key, "distance") == 0)
    {
        level->length = json_intValue(value);
        return;
    }
    if (strcmp(key, "enemyIndex") == 0)
    {
        level->enemeyIndex = json_intValue(value);
        return;
    }
    if (strcmp(key, "numEnemies") == 0)
    {
        level->numEnemies = json_intValue(value);
    }
    if (strcmp(key, "laneWidth") == 0)
    {
        setLaneWidth(json_intValue(value));
    }

    //level enemey data
    if (strcmp(key, "launchDistance") == 0)
    {
        level->enemies[currentEnemeyIndex].launchDistance = json_intValue(value);
    }
    if (strcmp(key, "launchX") == 0)
    {
        level->enemies[currentEnemeyIndex].launchX = json_intValue(value);
    }
    if (strcmp(key, "launchY") == 0)
    {
        level->enemies[currentEnemeyIndex].launchY = json_intValue(value);
    }
    if (strcmp(key, "type") == 0)
    {
        EnemyTypes tempType;
        if (strcmp(json_stringValue(value), "base") == 0)
            tempType = Coupe;

        //this is the last field in the enemy object, so increment the index
        level->enemies[currentEnemeyIndex++].type = tempType;
    }
}

int shouldDecodeArrayValueAtIndex(json_decoder* decoder, int pos)
{
	//p->system->logToConsole("%s shouldDecodeArrayValueAtIndex %i", decoder->path, pos);
	return 1;
}

void didDecodeArrayValue(json_decoder* decoder, int pos, json_value value)
{
	//p->system->logToConsole("%s didDecodeArrayValue %i %s", decoder->path, pos, typeToName(value.type));
}

void* didDecodeSublist(json_decoder* decoder, const char* name, json_value_type type)
{
	//p->system->logToConsole("%s didDecodeSublist %s %s", decoder->path, typeToName(type), name);
	return NULL;
}

json_decoder decoder =
{
	.decodeError = decodeError,
	.willDecodeSublist = willDecodeSublist,
	.shouldDecodeTableValueForKey = shouldDecodeTableValueForKey,
	.didDecodeTableValue = didDecodeTableValue,
	.shouldDecodeArrayValueAtIndex = shouldDecodeArrayValueAtIndex,
	.didDecodeArrayValue = didDecodeArrayValue,
	.didDecodeSublist = didDecodeSublist
};

int loadLevel(int levelNum)
{
    bmp = loadImageAtPath("images/commuter");
    level = p->system->realloc(NULL, sizeof(LevelData));
    
    SDFile* file = p->file->open("level1.json", kFileRead);
    
	json_value val;
	p->json->decode(&decoder, (json_reader){ .read = p->file->read, .userdata = file }, &val);
    p->file->close(file);



    return 1;
}

int updateLevel()
{
    if (level->enemeyIndex < level->numEnemies)
        if (distanceTraveled > level->enemies[level->enemeyIndex].launchDistance)
        {
            createBaseEnemyWithY(bmp, level->enemies[level->enemeyIndex].launchY);
            level->enemeyIndex++;
        }
}