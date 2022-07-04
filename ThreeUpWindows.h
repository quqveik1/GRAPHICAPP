#pragma once
#include "DrawBibliothek.h"

struct CloseButton : Window
{

    CloseButton(AbstractAppData* _app, HDC _dc) :
        Window(_app, {}, NULL, _dc)
    {}

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};

struct ResizeButton : Window
{
    Vector sizeLastTime;
    HDC nowIsFullScreen;
    HDC nowIsNotFullScreen;


    ResizeButton(AbstractAppData* _app) :
        Window(_app),
        sizeLastTime(app->systemSettings->FullSizeOfScreen),
        nowIsFullScreen(app->loadManager->loadImage("FullScreenButton.bmp")),
        nowIsNotFullScreen(app->loadManager->loadImage("ResizeScreenButton.bmp"))
    {
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};



struct MinimizeWindow : Window
{
    MinimizeWindow(AbstractAppData* _app) :
        Window(_app, {}, NULL, _app->loadManager->loadImage("MinimizeWindowButton.bmp"))
    {}


    virtual void onClick(Vector mp) override;
};


