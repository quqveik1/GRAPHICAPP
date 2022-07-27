#pragma once 
#include "ImportImage.h"
#include "CanvasManager.h"

void ImportImage::draw()
{

    if (needToShow)
    {
        //условие выхода если холст не создан
        assert(app->canvasManager);
        Canvas* activeCanvas = app->canvasManager->getActiveCanvas();

        if (!activeCanvas)
        {
            app->messageBox("Некуда открывать, холст не создан", "Ошибка", MB_OK);
            needToShow = false;
            return;
        }

        const char* pathToSave = app->getSaveFileName("Место хранения изображения", "Image (*.bmp)\0*.bmp\0Image (*.png)\0*.png\0Image (*.jpg)\0*.jpg\0");;//getCustomFilePathForSaving("Место сохранения картинки", activeCanvas->getName(), "Image (*.bmp)\0*.bmp\0Image (*.png)\0*.png\0Image (*.jpg)\0*.jpg\0");
        char fullPath[MAX_PATH] = {};
        if (pathToSave)strcpy(fullPath, pathToSave);

        if (fullPath[0])
        {
            if (importImage)
            {
                Vector newDCSize = {};
                HDC dc = importImage(fullPath, newDCSize, app);
                if (dc) activeCanvas->importImage(dc);
            }
        }
    }
    needToShow = false;
}