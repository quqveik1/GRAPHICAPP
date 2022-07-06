#pragma once
#include "CanvasManager.h"



Canvas* CanvasManager::getActiveCanvas()
{
    return activeCanvas;
}


void CanvasManager::draw()
{
    rect.finishPos = app->systemSettings->SizeOfScreen;

    app->setColor(app->systemSettings->BackgroundColor, finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);

   


    standartManagerDraw(this);
}

bool CanvasManager::addCanvas()
{
    return addWindow(activeCanvas = new Canvas(app, { .pos = {100, 50}, .finishPos = {newCanvasWindowSize.x + 100, newCanvasWindowSize.y + 50} }, loadManager, closeCanvasButton));
}

void CanvasManager::onClick(Vector mp)
{
    int clickedCellNum = standartManagerOnClick(this, mp);

    if (clickedCellNum >= 0)
    {
        activeCanvas = (Canvas*)pointers[clickedCellNum];
        replaceWindow(clickedCellNum);
    }
}

void CanvasManager::deleteButton()
{
    //newCanvas.deleteButton();
    app->smartDeleteDC(closeCanvasButton);

    for (int i = 0; i < length; i++)
    {
        if (pointers[i]) pointers[i]->deleteButton();
    }
}
