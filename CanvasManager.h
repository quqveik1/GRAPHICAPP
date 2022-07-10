#pragma once
#include "Canvas.h"
#include "ProgressBar.h"

struct CanvasManager : Manager
{
    HDC closeCanvasButton;
    int activeCanvasNum = -1;
    bool addNewCanvas = false;
    Vector newCanvasWindowSize = { 1000, 600 };
    int canvasesLength = 0;
    Canvas* canvases[10] = {};
    Rect tabs[10] = {};
    Vector oneTabSize = {};


    CanvasManager (AbstractAppData* _app) :
        Manager(_app, { .pos = {0, _app->systemSettings->HANDLEHEIGHT}, .finishPos = _app->systemSettings->FullSizeOfScreen }, 10, true, NULL, {}, TX_BLACK),
        oneTabSize({app->systemSettings->BUTTONWIDTH * 4, app->systemSettings->HANDLEHEIGHT})
    {
        gassert(loadManager);
        app->compressImage(closeCanvasButton, { systemSettings->MENUBUTTONSWIDTH,  systemSettings->HANDLEHEIGHT }, loadManager->loadImage("CloseButton4.bmp"), { 50, 26 });
    }

    virtual Canvas* getActiveCanvas();
    virtual bool addCanvas(const char* name, Vector dcSize);

    virtual int setDrawingMode(int num);

    Vector getCentrizedPos(Vector localSize, Vector globalSize);
    void drawTabs();
    void setTabsRect();
    int tabsOnClick();


    virtual void screenChanged() override;
    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual void deleteButton() override;
};