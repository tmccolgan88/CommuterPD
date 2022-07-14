#include "pd_api.h"
#include "../globals.h"
#include "tools.h"

LCDBitmap *loadImageAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDBitmap* img = p->graphics->loadBitmap(path, &outErr);
	if (outErr != NULL) 
	{
		p->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
	}
	return img;
}