#pragma once
#include "Canvas.h"
#include "ProgressBar.h"
#include "GlobalOptions.h"

struct CanvasManager : Manager
{
    HDC closeCanvasButton;
    Canvas* activeCanvas = NULL;
    bool addNewCanvas = false;
    Vector newCanvasWindowSize = { 1000, 600 };
    CLoadManager* loadManager;


    CanvasManager (AbstractAppData* _app) :
        Manager(_app, { .pos = {}, .finishPos = _app->systemSettings->FullSizeOfScreen }, 10, true, NULL, {}, TX_BLACK),
        loadManager (_app->loadManager)
    {
        app->compressImage(closeCanvasButton, { systemSettings->MENUBUTTONSWIDTH,  systemSettings->HANDLEHEIGHT }, loadManager->loadImage("CloseButton4.bmp"), { 50, 26 });
    }

    virtual Canvas* getActiveCanvas();
    bool addCanvas();

    virtual int setDrawingMode(int num);

    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual void deleteButton() override;
};