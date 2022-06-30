#pragma once
#include "ColorMenu.h"

void ColorMenu::draw()
{
    standartManagerDraw(this);

    for (int i = 0; i < getCurLen(); i++)
    {
        if (((ColorComponentChanger*)pointers[i])->confirmColor)
        {
            confirmColor();
            ((ColorComponentChanger*)pointers[i])->confirmColor = false;
        }
    }

    app->systemSettings->DrawColor = RGB(redComponent, greenComponent, blueComponent);

    app->setColor(app->systemSettings->DrawColor, finalDC);
    app->rectangle(20, 85, 45, 110, finalDC);
    
    setColorRects();
    for (int i = 0; i < currHistoryLen; i++)
    {
        app->setColor(colorHistory[i], finalDC);
        app->rectangle(colorRect[i], finalDC);
    }
    setMbLastTime();
}


void ColorMenu::onClick(Vector mp)
{
    setActiveWindow(this);
    standartManagerOnClick(this, mp);


    int clickedI = -1;
    setColorRects();
    for (int i = 0; i < currHistoryLen; i++)
    {
        if (colorRect[i].inRect(mp) && !isClickedLastTime()) clickedI = i;
    }

    if (clickedI < 0) return;

    COLORREF copyColor = colorHistory[clickedI];

    moveHistory(clickedI);
    colorHistory[currentPos - 1] = copyColor;
}



void ColorMenu::moveHistory(int clickedNumOfColorRect)
{
    if (clickedNumOfColorRect == currentPos - 1 || (currentPos == 0 && clickedNumOfColorRect == HistoryLength - 1))  return;

    int startNumSecondCircle = clickedNumOfColorRect;

    if (clickedNumOfColorRect > currentPos - 1)
    {
        for (int i = clickedNumOfColorRect + 1; i < currHistoryLen; i++)
        {
            colorHistory[i - 1] = colorHistory[i];
        }
        colorHistory[currHistoryLen - 1] = colorHistory[0];
        startNumSecondCircle = 0;
    }

    for (int i = startNumSecondCircle + 1; i <= currentPos - 1; i++)
    {
        colorHistory[i - 1] = colorHistory[i];                                                   
    }
}




void ColorMenu::confirmColor()
{
    if (currHistoryLen < HistoryLength) currHistoryLen++;
    colorHistory[currentPos] = app->systemSettings->DrawColor;

    if (currentPos < HistoryLength - 1) currentPos++;
    else                currentPos = 0;
}


void ColorMenu::setColorRects()
{
    for (int i = currentPos - 1; i >= 0; i--)
    {
        int orderI = currentPos - 1 - i;
        Rect colorsRect = { .pos = { (double)45 + 30 * orderI, (double)175}, .finishPos = { (double)70 + 30 * (orderI), (double)200} };
        colorRect[i] = colorsRect;
    }

    for (int i = currHistoryLen - 1; i >= currentPos; i--)
    {
        int orderI = currentPos + (currHistoryLen - 1 - i);
        Rect colorsRect = { .pos = { (double)45 + 30 * orderI, (double)175}, .finishPos = { (double)70 + 30 * (orderI), (double)200} };
        colorRect[i] = colorsRect;
    }
}