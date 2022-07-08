#pragma once
#include "CanvasManager.h"
#include "Canvas.cpp"



Canvas* CanvasManager::getActiveCanvas()
{
    return activeCanvas;
}


void CanvasManager::draw()
{
    rect.finishPos = app->systemSettings->SizeOfScreen;

    app->setColor(app->systemSettings->BackgroundColor, finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);

   


    app->standartManagerDraw(this);
}

int CanvasManager::setDrawingMode(int num)
{
    Canvas* canvas = getActiveCanvas();

    if (num > 0)
    {
        bool needToChangeDrawingMode = false;
        if (canvas)
        {
            if (!canvas->getEditingMode())
            {
                needToChangeDrawingMode = true;
            }
        }
        else
        {
            needToChangeDrawingMode = true;
        }

        if (needToChangeDrawingMode)
        {
            app->systemSettings->DrawingMode = num;
            return !"Sucsess";
        }
    }
    return 1;
}

bool CanvasManager::addCanvas()
{
    return addWindow(activeCanvas = new Canvas(app, { .pos = {100, 50}, .finishPos = {newCanvasWindowSize.x + 100, newCanvasWindowSize.y + 50} }, loadManager, closeCanvasButton));
}

void CanvasManager::onClick(Vector mp)
{
    int clickedCellNum = app->standartManagerOnClick(this, mp);

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
