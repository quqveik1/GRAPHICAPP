#pragma once
#include "SaveImages.h"


void SaveImages::draw()
{
    needToShow = false;

    //������� ������ ���� ����� �� ������
    if (!canvasManager->getActiveCanvas())
    {
        txMessageBox("������ ���������, ����� �� ������", "������");
        return;
    }

    const char* pathToSave = getCustomFilePathForSaving("����� ���������� ��������", "Image (*.bmp)", "bmp");
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


    if (!result) txMessageBox("������ �� �����������", "������");
}


