#pragma once
#include "OpenManager.h"

void OpenManager::onClick(Vector mp)
{
    setActiveWindow(this);
    if (!isClickedLastTime())
    {
        assert(openingManager);
        if (getOpeningManager()->needToShow) getOpeningManager()->hide();
        else                                 getOpeningManager()->show();
        getOpeningManager()->draw();
    }
    setMbLastTime();
}

void OpenManager::draw()
{
    standartDraw(this);

    showControl();
    
    app->setColor(app->systemSettings->TextColor, finalDC, app->systemSettings->MainFont);
    setMbLastTime();
}
