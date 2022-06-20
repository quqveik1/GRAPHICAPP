#pragma once

#include "GlobalOptions.h"

struct Image
{
    char path[MAX_PATH] = {};
	HDC dc = NULL;
    Vector size = {};
};

struct CLoadManager
{
    CSystemSettings* systemSettings;

	int currentImagesAmount = 0;

	Image images[1000];

    CLoadManager(CSystemSettings* _systemSettings) :
        systemSettings (_systemSettings)
    {}



    virtual HDC loadImage(const char* path, Vector size = {});
};
