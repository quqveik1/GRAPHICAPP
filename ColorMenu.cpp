#pragma once
#include "ColorMenu.h"


int ColorHistory::getByteSize()
{                      
    return sizeof(*this);
}


void ColorMenu::loadHistory()
{
    FILE* saveFile = fopen(pathToSaveHistory, "r+b");
    if (!saveFile)
    {
        printf("История цветов не загрузилась(");
        return;
    }

    int byteSize = colorHistory.getByteSize();
    fread(&colorHistory, sizeof(char), byteSize, saveFile);
    fclose(saveFile);
}

void ColorMenu::draw()
{
    assert(app);
    assert(app->systemSettings);

    standartManagerDraw(this);

    if (app->systemSettings->debugMode >= 3)printf("colorHistory.currentPos: %d\n", colorHistory.currentPos);
    
    if (confirmedColor)
    {
        confirmColor();
    }

    app->systemSettings->DrawColor = RGB(redComponent, greenComponent, blueComponent);

    app->setColor(app->systemSettings->DrawColor, finalDC);
    app->rectangle(20, 85, 45, 110, finalDC);
    
    setColorRects();
    for (int i = 0; i < colorHistory.currHistoryLen; i++)
    {
        app->setColor(colorHistory.colorHistory[i], finalDC);
        app->rectangle(colorHistory.colorRect[i], finalDC);
    }
    setMbLastTime();
}


void ColorMenu::onClick(Vector mp)
{
    setActiveWindow(this);
    standartManagerOnClick(this, mp);


    int clickedI = -1;
    setColorRects();
    for (int i = 0; i < colorHistory.currHistoryLen; i++)
    {
        if (colorHistory.colorRect[i].inRect(mp) && !isClickedLastTime()) clickedI = i;
    }

    if (clickedI < 0) return;

    COLORREF copyColor = colorHistory.colorHistory[clickedI];

    moveHistory(clickedI);
    colorHistory.colorHistory[colorHistory.currentPos - 1] = copyColor;
    app->systemSettings->DrawColor = copyColor;
    setColorComponents();
}

void ColorMenu::setColorComponents()
{
    redComponent = app->getColorComponent(app->systemSettings->DrawColor, TX_RED);
    greenComponent = app->getColorComponent(app->systemSettings->DrawColor, TX_GREEN);
    blueComponent = app->getColorComponent(app->systemSettings->DrawColor, TX_BLUE);
}



void ColorMenu::moveHistory(int clickedNumOfColorRect)
{
    if (clickedNumOfColorRect == colorHistory.currentPos - 1 || (colorHistory.currentPos == 0 && clickedNumOfColorRect == colorHistory.HistoryLength - 1))  return;

    int startNumSecondCircle = clickedNumOfColorRect;

    if (clickedNumOfColorRect > colorHistory.currentPos - 1)
    {
        for (int i = clickedNumOfColorRect + 1; i < colorHistory.currHistoryLen; i++)
        {
            colorHistory.colorHistory[i - 1] = colorHistory.colorHistory[i];
        }
        colorHistory.colorHistory[colorHistory.currHistoryLen - 1] = colorHistory.colorHistory[0];
        startNumSecondCircle = 0;
    }

    for (int i = startNumSecondCircle + 1; i <= colorHistory.currentPos - 1; i++)
    {
        colorHistory.colorHistory[i - 1] = colorHistory.colorHistory[i];
    }
}




void ColorMenu::confirmColor()
{
    if (colorHistory.currHistoryLen < colorHistory.HistoryLength) colorHistory.currHistoryLen++;
    colorHistory.colorHistory[colorHistory.currentPos] = app->systemSettings->DrawColor;

    if (colorHistory.currentPos < colorHistory.HistoryLength - 1) colorHistory.currentPos++;
    else                colorHistory.currentPos = 0;

    confirmedColor = false;
}

void ColorMenu::saveMenu()
{
    FILE* saveFile = fopen(pathToSaveHistory, "w+b");
    if (!saveFile)
    {
        printf("История цветов не сохранилась");
        return;
    }

    int byteSize = colorHistory.getByteSize();
    fwrite(&colorHistory, sizeof(char), byteSize, saveFile);
    fclose(saveFile);
}


void ColorMenu::setColorRects()
{
    for (int i = colorHistory.currentPos - 1; i >= 0; i--)
    {
        int orderI = colorHistory.currentPos - 1 - i;
        Rect colorsRect = { .pos = { (double)45 + 30 * orderI, (double)175}, .finishPos = { (double)70 + 30 * (orderI), (double)200} };
        colorHistory.colorRect[i] = colorsRect;
    }

    for (int i = colorHistory.currHistoryLen - 1; i >= colorHistory.currentPos; i--)
    {
        int orderI = colorHistory.currentPos + (colorHistory.currHistoryLen - 1 - i);
        Rect colorsRect = { .pos = { (double)45 + 30 * orderI, (double)175}, .finishPos = { (double)70 + 30 * (orderI), (double)200} };
        colorHistory.colorRect[i] = colorsRect;
    }
}