#pragma once
#include "Canvas.h"

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
    Vector scaleButtonSize = {};
    int intScale = 0;
    int minScale = 0;
    int maxScale = 1000;
    struct InputButton2* scaleButton;


    CanvasManager(AbstractAppData* _app);

    virtual Canvas* getActiveCanvas();
    virtual bool addCanvas(const char* name, Vector dcSize);

    virtual int setDrawingMode(int num);

    Vector getCentrizedPos(Vector localSize, Vector globalSize);
    void drawTabs();
    void setTabsRect();
    int tabsOnClick();
    void controlActiveCanvas();


    virtual void screenChanged() override;
    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual void deleteButton() override;
};              