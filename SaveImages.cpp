#pragma once
#include "SaveImages.h"


void SaveImages::draw()
{
    needToShow = false;

    //условие выхода если холст не создан
    Canvas* activeCanvas = canvasManager->getActiveCanvas();

    if (!activeCanvas)
    {
        txMessageBox("Нечего сохранять, холст не создан", "Ошибка");
        return;
    }

    const char* pathToSave = getCustomFilePathForSaving("Место сохранения картинки", activeCanvas->getName(), "Image (*.bmp)\0*.bmp\0Image (*.png)\0*.png\0Image (*.jpg)\0*.jpg\0");
    char fullPath[MAX_PATH] = {};
    strcpy(fullPath, pathToSave);

    if (fullPath[0] == NULL)
    {
        return;
    }

    HDC saveDC = NULL;

    saveDC = activeCanvas->getImageForSaving();
    

    int wasErrorInSavings = 0;
    if (saveDC)
    {
        if (saveImage)
        {
            wasErrorInSavings = saveImage(saveDC, fullPath);
        }
    }


    //int result = app->saveImage(saveDC, fullPath);
    app->deleteDC(saveDC);


    if (wasErrorInSavings) txMessageBox("Ничего не сохранилось", "Ошибка");
}


