#pragma once
#include "OpenManager.h"

void OpenManager::onClick(Vector mp)
{
    if (!isClickedLastTime())
    {
        assert(openingManager);
        getOpeningManager()->advancedMode = !getOpeningManager()->advancedMode;
        getOpeningManager()->draw();
    }
    setMbLastTime();
}

void OpenManager::draw()
{
    standartDraw(this);
    app->setColor(app->systemSettings->TextColor, finalDC, app->systemSettings->MainFont);
    setMbLastTime();
}
