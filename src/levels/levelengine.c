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
	p->system->logToConsole("%s willDecodeSublist %s %s", decoder->path, typeToName(type), name);
}

int shouldDecodeTableValueForKey(json_decoder* decoder, const char* key)
{
	p->system->logToConsole("%s shouldDecodeTableValueForKey %s", decoder->path, key);
	return 1;
}

void didDecodeTableValue(json_decoder* decoder, const char* key, json_value value)
{
	p->system->logToConsole("%s didDecodeTableValue %s %s", decoder->path, key, typeToName(value.type));
    p->system->logToConsole("\n\n\n %s", json_stringValue(value));
}

int shouldDecodeArrayValueAtIndex(json_decoder* decoder, int pos)
{
	p->system->logToConsole("%s shouldDecodeArrayValueAtIndex %i", decoder->path, pos);
	return 1;
}

void didDecodeArrayValue(json_decoder* decoder, int pos, json_value value)
{
	p->system->logToConsole("%s didDecodeArrayValue %i %s", decoder->path, pos, typeToName(value.type));
}

void* didDecodeSublist(json_decoder* decoder, const char* name, json_value_type type)
{
	p->system->logToConsole("%s didDecodeSublist %s %s", decoder->path, typeToName(type), name);
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
    char temp[5];

    LevelData* tempLevelData = p->system->realloc(NULL, sizeof(LevelData));
    
    SDFile* file = p->file->open("level1.json", kFileRead);
    //p->system->logToConsole("file error : %s", p->file->geterr());
    //p->system->logToConsole("level engine strlen %d", strlen(temp));
    /*p->file->read(file, temp, strlen(temp));
    //p->system->logToConsole("file error : %s", p->file->geterr());
    p->system->logToConsole("level engine : %s", temp);*/



	json_value val;
	p->json->decode(&decoder, (json_reader){ .read = p->file->read, .userdata = file }, &val);
    p->file->close(file);



    return 1;
}