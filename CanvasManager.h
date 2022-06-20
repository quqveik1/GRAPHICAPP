#pragma once
#include "Canvas.h"
#include "ProgressBar.h"
#include "GlobalOptions.h"

struct CanvasManager : Manager
{
    HDC closeCanvasButton;
    Canvas* activeCanvas = NULL;
    ProgressBar* bar;
    bool addNewCanvas = false;
    Vector newCanvasWindowSize = { 1000, 800 };
    CLoadManager* loadManager;


    CanvasManager (CSystemSettings* _systemSettings, Rect _rect, HDC _NewCanvasDC, ProgressBar* _bar, CLoadManager* _loadManager) :
        Manager(_systemSettings, _rect, 10, true, NULL, {}, TX_BLACK),
        bar(_bar),
        loadManager (_loadManager)
    {
        compressImage(closeCanvasButton, { systemSettings->MENUBUTTONSWIDTH,  systemSettings->HANDLEHEIGHT }, loadManager->loadImage("CloseButton4.bmp"), { 50, 26 }, __LINE__);
    }

    virtual Canvas* getActiveCanvas();
    bool addCanvas();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual void deleteButton() override;
};