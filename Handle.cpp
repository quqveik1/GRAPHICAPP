#include "Handle.h"


bool Handle::addWindowToStart(Window* window)
{
    Rect windowRect = {};
    windowRect.pos.x = rect.pos.x + app->systemSettings->BUTTONWIDTH * (getCurLen() - numberOfAddToBackElements);
    windowRect.pos.y = rect.pos.y;

    windowRect.finishPos.x = rect.pos.x + app->systemSettings->BUTTONWIDTH * (getCurLen() - numberOfAddToBackElements + 1);
    windowRect.finishPos.y = rect.finishPos.y;

    window->resize(windowRect);

    return addWindow(window);
}

bool Handle::addWindowToBack(Window* window)
{
    Rect windowRect = {};
    windowRect.pos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * (getCurLen() + 1);
    windowRect.pos.y = rect.pos.y;

    windowRect.finishPos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * getCurLen();
    windowRect.finishPos.y = rect.finishPos.y;

    window->resize(windowRect);

    addToBackElemetsPos[numberOfAddToBackElements] = getCurLen();
    numberOfAddToBackElements++;

    return addWindow(window);
}

void Handle::screenChanged()
{
    Rect newRect = { .pos = rect.pos, .finishPos = {app->systemSettings->SizeOfScreen.x, rect.pos.y + getSize().y} };
    resize(newRect);

    for (int i = 0; i < numberOfAddToBackElements; i++)
    {
        Rect windowRect = {};
        windowRect.pos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * (i + 1);
        windowRect.pos.y = rect.pos.y;

        windowRect.finishPos.x = rect.finishPos.x - app->systemSettings->BUTTONWIDTH * i;
        windowRect.finishPos.y = rect.finishPos.y;
        pointers[i]->resize(windowRect);
    }
}


void Handle::draw()
{


    app->standartManagerDraw(this);

    rect.finishPos.x = app->systemSettings->SizeOfScreen.x;

    if (wasCommonHandlePlaceClicked)
    {
        Vector superAbsMP = app->getCursorPos();
        Vector delta = superAbsMP - lastTimeMousePos;
        if (wasInFullScreenLastTime != app->isFullScreen()) delta = {};
        
        if (app->systemSettings->debugMode >= 2) printf("delta: {%lf, %lf}\n", delta.x, delta.y);
        if (app->systemSettings->debugMode >= 2) printf("mp: {%lf, %lf}\n", superAbsMP.x, superAbsMP.y);

        if (delta != 0)
        {
            app->systemSettings->ScreenPos += delta;
            app->changeWindow({}, app->systemSettings->ScreenPos);
        }

        lastTimeMousePos = superAbsMP;
        if (wasCommonHandlePlaceClicked && getMBCondition() == 0)wasCommonHandlePlaceClicked = 0;
    }

    wasInFullScreenLastTime = app->isFullScreen();

    setMbLastTime();
}

void Handle::onClick(Vector mp)
{
    int resultOfClicking = app->standartManagerOnClick(this, mp);

    if (resultOfClicking == -1 && !wasCommonHandlePlaceClicked)
    {
        if (app->isFullScreen())
        {
            app->changeWindow(app->systemSettings->lastTimeSizeOfScreen);
            app->setResized();
        }
        lastTimeMousePos = getAbsMousePos() + app->systemSettings->ScreenPos;
        wasCommonHandlePlaceClicked = true;
    }
}

