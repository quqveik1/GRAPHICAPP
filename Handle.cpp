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


void Handle::draw()
{
    standartManagerDraw(this);

    if (app->wasResized())
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

    if (wasCommonHandlePlaceClicked)
    {
        Vector superAbsMP = getAbsMousePos() + app->systemSettings->ScreenPos;
        Vector delta = superAbsMP - lastTimeMousePos;
        if (app->wasResized())
        {
            delta = { 0, 0 };
        }

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

    setMbLastTime();
}

void Handle::onClick(Vector mp)
{
    int resultOfClicking = standartManagerOnClick(this, mp);

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

