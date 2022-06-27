#pragma once
#include "ThreeUpWindows.h"


void CloseButton::draw()
{
    $s
        //app->setColor (color);
        //app->rectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
        app->bitBlt(finalDC, 0, 0, 0, 0, dc);
}


void MinimizeWindow::onClick(Vector mp)
{
    assert(app);
    assert(app->systemSettings);
    assert(app->systemSettings->MAINWINDOW);
    if (!isClickedLastTime()) ShowWindow(app->systemSettings->MAINWINDOW, SW_SHOWMINIMIZED);
}

void CloseButton::onClick(Vector mp)
{
    assert(app);
    app->IsRunning = false;
}



void ResizeButton::onClick(Vector mp)
{
    PowerPoint* fullapp = (PowerPoint*)app;

    if (!isClickedLastTime())
    {
        Vector saveSize = app->systemSettings->SizeOfScreen;
        if (app->isFullScreen())
        {
            if (app->systemSettings->FullSizeOfScreen == app->systemSettings->lastTimeSizeOfScreen || app->systemSettings->lastTimeSizeOfScreen == app->systemSettings->lastTimeSizeOfScreen.getNullVector())
            {
                Vector newSize = app->systemSettings->FullSizeOfScreen / 1.5;
                fullapp->changeWindow(newSize);
            }
            else
            {
                fullapp->changeWindow(app->systemSettings->lastTimeSizeOfScreen);
            }

        }
        else
        {
            fullapp->changeWindow(app->systemSettings->FullSizeOfScreen);
        }
        app->setResized();
        app->systemSettings->lastTimeSizeOfScreen = saveSize;
    }
}

void ResizeButton::draw()
{
    setMbLastTime();
    if (app->isFullScreen()) dc = nowIsFullScreen;
    else                     dc = nowIsNotFullScreen;


    standartDraw(this);
}

