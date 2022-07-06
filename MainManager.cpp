#pragma once
#include "MainManager.h"

void MainManager::controlWorkSpace()
{
    if (screenSizeLastTime != app->systemSettings->SizeOfScreen)
    {
        screenChanged();
    }
    screenSizeLastTime = app->systemSettings->SizeOfScreen;
}


void MainManager::screenChanged()
{
    setFrameZones();
    for (int i = 0; i < getCurLen(); i++)
    {
        if (pointers[i]) pointers[i]->screenChanged();
    }
}

void MainManager::setFrameZones()
{

    upRect = { .pos = {app->systemSettings->FrameThickness, 0}, .finishPos = {app->systemSettings->SizeOfScreen.x - app->systemSettings->FrameThickness, app->systemSettings->FrameThickness} };
    rightRect = { .pos = {app->systemSettings->SizeOfScreen.x - app->systemSettings->FrameThickness, app->systemSettings->FrameThickness}, .finishPos = {app->systemSettings->SizeOfScreen.x, app->systemSettings->SizeOfScreen.y - app->systemSettings->FrameThickness} };
    bottomRect = { .pos = {app->systemSettings->FrameThickness, app->systemSettings->SizeOfScreen.y - app->systemSettings->FrameThickness}, .finishPos = {app->systemSettings->SizeOfScreen.x - app->systemSettings->FrameThickness, app->systemSettings->SizeOfScreen.y} };
    leftRect = { .pos = {0, app->systemSettings->FrameThickness}, .finishPos = {app->systemSettings->FrameThickness, app->systemSettings->SizeOfScreen.y - app->systemSettings->FrameThickness} };

    upLeft = { .pos = {}, .finishPos = {app->systemSettings->FrameThickness, app->systemSettings->FrameThickness} };
    upRight = { .pos = {app->systemSettings->SizeOfScreen.x - app->systemSettings->FrameThickness, 0}, .finishPos = {app->systemSettings->SizeOfScreen.x, app->systemSettings->FrameThickness} };
    bottomRight = { .pos = {app->systemSettings->SizeOfScreen.x - app->systemSettings->FrameThickness, app->systemSettings->SizeOfScreen.y - app->systemSettings->FrameThickness}, .finishPos = {app->systemSettings->SizeOfScreen.x, app->systemSettings->SizeOfScreen.y} };
    bottomLeft = { .pos = {0, app->systemSettings->SizeOfScreen.y - app->systemSettings->FrameThickness}, .finishPos = {app->systemSettings->FrameThickness, app->systemSettings->SizeOfScreen.y} };

}

int MainManager::findZone()
{
    Vector mp = getMousePos();  
    if (activeZone) return activeZone;
    for (int i = 0; i < zonesNum; i++)
    {
        if (zones[i].rect->inRect(mp) || i == activeZone - 1)
        {
            return i + 1;
        }
    }
    return 0;
}

void MainManager::setFrameCursor()
{
    if (activeZone > 0) app->setCursor(zones[activeZone - 1].cursor);
}


void MainManager::controlScreenResizing()
{
    if (!getMBCondition() && activeZone) activeZone = 0;
    Vector superAbsMP = app->getCursorPos();
    Vector delta = superAbsMP - mousePosLastTime;
    if (wasZoneClicked)
    {

        if (activeZone == 1)
        {
            app->systemSettings->SizeOfScreen.y -= delta.y;
            app->systemSettings->ScreenPos.y += delta.y;
        }
        if (activeZone == 2)
        {
            app->systemSettings->SizeOfScreen.x += delta.x;
        }
        if (activeZone == 3)
        {
            app->systemSettings->SizeOfScreen.y += delta.y;
        }
        if (activeZone == 4)
        {
            app->systemSettings->SizeOfScreen.x -= delta.x;
            app->systemSettings->ScreenPos.x += delta.x;
        }


        if (activeZone == 5)
        {
            app->systemSettings->SizeOfScreen -= delta;
            app->systemSettings->ScreenPos += delta;
        }
        if (activeZone == 6)
        {
            app->systemSettings->ScreenPos.y += delta.y;
            delta.y = -delta.y;
            app->systemSettings->SizeOfScreen += delta;
        }
        if (activeZone == 7)
        {
            app->systemSettings->SizeOfScreen += delta;
        }
        if (activeZone == 8)
        {
            app->systemSettings->ScreenPos.x += delta.x;
            delta.x = -delta.x;
            app->systemSettings->SizeOfScreen += delta;
        }

        app->changeWindow(app->systemSettings->SizeOfScreen, app->systemSettings->ScreenPos);
    }

    mousePosLastTime = app->getCursorPos();
}


void MainManager::draw()
{
    
    controlWorkSpace();
    int mb = getMBCondition();
    if (!app->isFullScreen())
    {
        if (mb && wasZoneClicked || !mb && !wasZoneClicked)
        {
            controlScreenResizing();
            activeZone = findZone();
            setFrameCursor();
        }
    }
    if (wasZoneClicked && getMBCondition() != 1)
    {
        wasZoneClicked = false;
    }


    standartManagerDraw(this);

    printf("Active Window: %p\n", isActiveWindowBelow());

    if (!app->isFullScreen())
    {
        app->drawCadre({ .pos = {0, -app->systemSettings->FrameThickness}, .finishPos = app->systemSettings->SizeOfScreen }, finalDC, color, app->systemSettings->FrameThickness);
    }

    setMbLastTime();
}


void MainManager::onClick(Vector mp)
{
    if (!app->isFullScreen()) activeZone = findZone();
    if (activeZone && mbLastTime != getMBCondition())
    {
        wasZoneClicked = true;
        return;
    }
    if (wasZoneClicked) return;

    standartManagerOnClick(this, mp);
}
