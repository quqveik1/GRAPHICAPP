#pragma once
#include "CanvasManager.h"
#include "Canvas.cpp"
#include "InputButton.cpp"

CanvasManager::CanvasManager(AbstractAppData* _app) :
    Manager(_app, { .pos = {0, _app->systemSettings->HANDLEHEIGHT}, .finishPos = _app->systemSettings->FullSizeOfScreen }, 10, true, NULL, {}, TX_BLACK),
    oneTabSize({ app->systemSettings->BUTTONWIDTH * 4, app->systemSettings->HANDLEHEIGHT }),
    scaleButtonSize({75, 25}),
    scaleButton(new InputButton (_app, { .pos = {}, .finishPos = scaleButtonSize }, &intScale, &minScale, &maxScale, 1, color))
{
    gassert(loadManager);
    addWindow(scaleButton);
    app->compressImage(closeCanvasButton, { systemSettings->MENUBUTTONSWIDTH,  systemSettings->HANDLEHEIGHT }, loadManager->loadImage("CloseButton4.bmp"), { 50, 26 });
}
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
    if (!getActiveCanvas())
    {
        app->setColor(app->systemSettings->TextColor, finalDC);
        app->drawText({ .pos = {}, .finishPos = {getSize().x, oneTabSize.y} }, "Íàæìèòå Ñtrl+N, ÷òîáû ñîçäàòü õîëñò", finalDC);
    }

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

    app->setColor(app->systemSettings->BackgroundColor, finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);

    if (getActiveCanvas())
    {
        Rect userRect = app->getUserRect();
        userRect = userRect - rect.pos;

        Vector scaleButtonPos = { userRect.pos.x, userRect.finishPos.y - scaleButton->getSize().y };

        intScale = getActiveCanvas()->getScale() * 100;
        scaleButton->MoveWindowTo(scaleButtonPos);
        scaleButton->draw();
        controlActiveCanvas();
        
        app->bitBlt(finalDC, scaleButton->rect.pos, scaleButton->getSize(), scaleButton->finalDC);
    }

    drawTabs();
}

void CanvasManager::controlActiveCanvas()
{
    getActiveCanvas()->getScale() = (double)intScale / 100.0;

    getActiveCanvas()->draw();
    if (app->getAsyncKeyState(VK_RIGHT))
    {
        getActiveCanvas()->deltaPos.x -= 10;
    } 
    if (app->getAsyncKeyState(VK_LEFT))
    {
        getActiveCanvas()->deltaPos.x += 10;
    }  
    if (app->getAsyncKeyState(VK_UP))
    {
        getActiveCanvas()->deltaPos.y += 10;
    }  
    if (app->getAsyncKeyState(VK_DOWN))
    {
        getActiveCanvas()->deltaPos.y -= 10;
    }
    Vector ñanvasPos = app->getCentrizedPos(getActiveCanvas()->getSize(), getSize());
    ñanvasPos += getActiveCanvas()->deltaPos;
    getActiveCanvas()->MoveWindowTo(ñanvasPos);

    app->bitBlt(finalDC, getActiveCanvas()->rect.pos, getActiveCanvas()->getSize(), getActiveCanvas()->finalDC);
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
    canvases[canvasesLength] = new Canvas(app, { .pos = getCentrizedPos(dcSize, getSize()), .finishPos = getCentrizedPos(dcSize, getSize()) + dcSize }, name, closeCanvasButton);
    activeCanvasNum = canvasesLength;
    canvasesLength++;
    setTabsRect();
    return addWindow(canvases[canvasesLength - 1]);
}



void CanvasManager::onClick(Vector mp)
{
    if (tabsOnClick() >= 0) return;

    if (getActiveCanvas())
    {
        mp = getMousePos();
        if (scaleButton->rect.inRect(mp))
        {
            scaleButton->onClick(mp);
            return;
        }

        if (getActiveCanvas()->rect.inRect(mp))getActiveCanvas()->onClick(mp);
    }
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
            centrizedPos = app->getCentrizedPos(tempSize, getSize());
            canvases[i]->MoveWindowTo(centrizedPos);
        }
    }
}
