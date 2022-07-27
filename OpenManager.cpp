#pragma once
#include "OpenManager.h"

void OpenManager::onClick(Vector mp)
{
    setActiveWindow(this);
    if (!isClickedLastTime())
    {
        
        if (mode == 0)
        {
            assert(openingManager);
            if (getOpeningManager()->needToShow)
            {
                getOpeningManager()->hide();
            }
            else
            {
                getOpeningManager()->show();
            }
            getOpeningManager()->draw();
        }

        if (mode == 1)
        {
            if (*opening == 1) *opening = 0;
            if (*opening == 0) *opening = 1;
        }
    }
    setMbLastTime();
}

void OpenManager::draw()
{
    app->windowsLibApi->standartWindowDraw(this);

    showControl();
    
    app->setColor(app->systemSettings->TextColor, finalDC, app->systemSettings->MainFont);
    setMbLastTime();
}
