#pragma once
#include "SaveImages.h"


void SaveImages::draw()
{
    needToShow = false;

    //условие выхода если холст не создан
    if (!canvasManager->getActiveCanvas())
    {
        txMessageBox("Нечего сохранять, холст не создан", "Ошибка");
        return;
    }

    const char* pathToSave = getCustomFilePathForSaving("Место сохранения картинки", "Image (*.bmp)", "bmp");
    char fullPath[MAX_PATH] = {};
    strcpy(fullPath, pathToSave);

    if (fullPath[0] == NULL)
    {
        return;
    }

    HDC saveDC = NULL;

    saveDC = canvasManager->getActiveCanvas()->getImageForSaving();


    int result = app->saveImage(saveDC, fullPath);
    app->deleteDC(saveDC);


    if (!result) txMessageBox("Ничего не сохранилось", "Ошибка");
}


