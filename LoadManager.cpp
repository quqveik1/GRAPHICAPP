#pragma once
#include "LoadManager.h"


HDC CLoadManager::loadImage(const char* path, Vector size /* = {} */)
{
    bool newImage = true;
    int suitableDCNum = -1;

    char fullPath[MAX_PATH] = {};
    sprintf(fullPath, "Files\\%s", path);

    for (int i = 0; i < currentImagesAmount; i++)
    {
        if (!strcmp(fullPath, images[i].path))
        {
            newImage = false;
            suitableDCNum = i;
            break;
        }
    }

    if (!newImage)
    {
        if (debugMode == 4) printf("Fullpath: %s; Result: %lf\n", fullPath, images[suitableDCNum].dc);
        if (debugMode == 4) printBlt(images[suitableDCNum].dc);
        return images[suitableDCNum].dc;
    }


    images[currentImagesAmount].dc = txLoadImage(fullPath);
    if (debugMode == 4) printf("Fullpath: %s; Result: %lf\n", fullPath, images[currentImagesAmount].dc);
    if (debugMode == 4) printBlt(images[currentImagesAmount].dc);
    if (images[currentImagesAmount].dc == NULL) assert(!fullPath);
    strcpy (images[currentImagesAmount].path, fullPath);
    images[currentImagesAmount].size = size;
    currentImagesAmount++;

    return images[currentImagesAmount - 1].dc;


}
