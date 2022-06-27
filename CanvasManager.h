#pragma once
#include "Canvas.cpp"
#include "ProgressBar.h"
#include "GlobalOptions.h"

struct CanvasManager : Manager
{
    HDC closeCanvasButton;
    Canvas* activeCanvas = NULL;
    bool addNewCanvas = false;
    Vector newCanvasWindowSize = { 1000, 600 };
    CLoadManager* loadManager;


    CanvasManager (AbstractAppData* _app, Rect _rect) :
        Manager(_app, _rect, 10, true, NULL, {}, TX_BLACK),
        loadManager (_app->loadManager)
    {
        compressImage(app, closeCanvasButton, { systemSettings->MENUBUTTONSWIDTH,  systemSettings->HANDLEHEIGHT }, loadManager->loadImage("CloseButton4.bmp"), { 50, 26 }, __LINE__);
    }

    virtual Canvas* getActiveCanvas();
    bool addCanvas();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual void deleteButton() override;
};