#pragma once
#include "CanvasManager.h"
#include "Canvas.cpp"



Canvas* CanvasManager::getActiveCanvas()
{
    if (activeCanvasNum < 0) return NULL;
    return canvases[activeCanvasNum];
}

void CanvasManager::drawTabs()
{
    app->setColor(app->systemSettings->MenuColor, finalDC);
    app->rectangle({}, { getSize().x, oneTabSize.y }, finalDC);

    app->setColor(TX_BLACK, finalDC, 1);
    app->line({0, 1}, { getSize().x, 1 }, finalDC);
    app->line({0, oneTabSize.y}, { getSize().x, oneTabSize.y }, finalDC);

    for (int i = 0; i < canvasesLength; i++)
    {
        COLORREF cadreColor = TX_BLACK;
        if (i == activeCanvasNum) cadreColor = TX_WHITE;
        app->drawCadre(tabs[i], finalDC, cadreColor, 2);
        app->setColor(app->systemSettings->TextColor, finalDC);
        app->drawText(tabs[i], canvases[i]->getName(), finalDC);
    }
}

void CanvasManager::setTabsRect()
{
    for (int i = 0; i < canvasesLength; i++)
    {
        tabs[i] = { .pos = {oneTabSize.x * i, 0}, .finishPos = {oneTabSize.x * (i + 1), oneTabSize.y } };
    }
}




void CanvasManager::draw()
{
    rect.finishPos = app->systemSettings->SizeOfScreen;

    app->setColor(RGB(100, 100, 100), finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);

    drawTabs();

    if (activeCanvasNum >= 0)getActiveCanvas()->print(finalDC);
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

Vector CanvasManager::getCentrizedPos(Vector localSize, Vector globalSize)
{
    return (globalSize - localSize) * 0.5;
}

bool CanvasManager::addCanvas(const char* name, Vector dcSize)
{

    activeCanvas = new Canvas(app, { .pos = getCentrizedPos(dcSize, getSize()), .finishPos = getCentrizedPos(dcSize, getSize()) + dcSize }, name, closeCanvasButton);
    canvases[canvasesLength] = activeCanvas;
    activeCanvasNum = canvasesLength;
    canvasesLength++;
    setTabsRect();
    return addWindow(activeCanvas);
}



void CanvasManager::onClick(Vector mp)
{
    if (tabsOnClick() >= 0) return;

    if (getActiveCanvas()) if (getActiveCanvas()->rect.inRect(getMousePos()))getActiveCanvas()->onClick(mp);

    //int clickedCellNum = app->windowsLibApi->standartManagerOnClick(this, mp);
}

int CanvasManager::tabsOnClick()
{
    Vector mp = getMousePos();
    for (int i = 0; i < canvasesLength; i++)
    {
        if (tabs[i].inRect(mp))
        {
            activeCanvasNum = i;
            return activeCanvasNum;
        }
    }
    return -1;
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

void CanvasManager::screenChanged()
{
    rect.finishPos = app->systemSettings->SizeOfScreen;
    Vector centrizedPos = {};
    Vector tempSize = {};
    for (int i = 0; i < canvasesLength; i++)
    {
        if (canvases[i])
        {
            tempSize = canvases[i]->rect.getSize();
            centrizedPos = getCentrizedPos(tempSize, getSize());
            canvases[i]->MoveWindowTo(centrizedPos);

        }
    }
}
