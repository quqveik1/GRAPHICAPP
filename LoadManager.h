#pragma once

struct Image
{
    char path[MAX_PATH] = {};
	HDC dc = NULL;
    Vector size = {};
};

struct CLoadManager
{
	int currentImagesAmount = 0;

	Image images[1000];


    virtual HDC loadImage(const char* path, Vector size = {});
};
