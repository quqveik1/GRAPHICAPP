#pragma once
#include "Canvas.h"

struct CanvasManager : Manager
{
    int activeCanvasNum = -1;
    bool addNewCanvas = false;
    Vector newCanvasWindowSize = { 1000, 600 };
    int canvasesLength = 0;
    Canvas* canvases[10] = {};
    Rect tabs[10] = {};
    Vector oneTabSize = {};
    
    int intScale = 0;
    int minScale = 0;
    int maxScale = 1000;

    Vector scaleButtonSize = {};
    struct InputButton2* scaleButton = NULL;

    Vector plusMinusButtonSize = {};
    Rect plusButtonRect = {};
    Rect minusButtonRect = {};
    HDC plusButtonDC = NULL;
    HDC minusButtonDC = NULL;
    int lastTimeButtonClicked = 0;
    int deltaBetween2Clicks = 300;


    CanvasManager(AbstractAppData* _app);

    virtual Canvas* getActiveCanvas();
    virtual bool addCanvas(const char* name, Vector dcSize);

    virtual int setDrawingMode(int num);

    Vector getCentrizedPos(Vector localSize, Vector globalSize);
    void drawTabs();
    void setTabsRect();
    int tabsOnClick();
    void controlActiveCanvas();
    void controlStretching();
    void controlPosition();
    void controlScaleButtons();
    void drawScaleButtons();


    virtual void screenChanged() override;
    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual void deleteButton() override;
};              