#pragma once
#include "DrawBibliothek.h"

struct FrameZone
{
    Rect* rect = {};
    HCURSOR cursor = {};
};


struct MainManager : Manager
{
    Vector mousePos = {};
    int clicked = 0;
    Window* activeWindow = 0;
    Vector screenSizeLastTime = {};
    HCURSOR cursorHorizontal = NULL;
    HCURSOR cursorVertical = NULL;
    HCURSOR cursorSNDiagonal = NULL;
    HCURSOR cursorNSDiagonal = NULL;

    int activeZone = 0;
    bool wasZoneClicked = false;
    Rect upRect = {};//zone = 1
    Rect rightRect = {};//zone = 2
    Rect bottomRect = {};//zone = 3
    Rect leftRect = {};//zone = 4

    Rect upLeft = {}; //zone = 5
    Rect upRight = {}; //zone = 6
    Rect bottomRight = {}; //zone = 7
    Rect bottomLeft = {}; //zone = 8
    int zonesNum = 8;
    FrameZone zones[8] = {};
    POINT mpLastTime = {};



    MainManager(AbstractAppData* _app, Rect _rect, int _length) :
        Manager(_app, _rect, _length)
    {
        cursorHorizontal = LoadCursor(NULL, IDC_SIZEWE);
        cursorVertical = LoadCursor(NULL, IDC_SIZENS);
        cursorSNDiagonal = LoadCursor(NULL, IDC_SIZENESW);
        cursorNSDiagonal = LoadCursor(NULL, IDC_SIZENWSE);
        zones[0].rect = &upRect;
        zones[0].cursor = cursorVertical;
        zones[1].rect = &rightRect;
        zones[1].cursor = cursorHorizontal;
        zones[2].rect = &bottomRect;
        zones[2].cursor = cursorVertical;
        zones[3].rect = &leftRect;
        zones[3].cursor = cursorHorizontal;
        zones[4].rect = &upLeft;
        zones[4].cursor = cursorNSDiagonal;
        zones[5].rect = &upRight;
        zones[5].cursor = cursorSNDiagonal;
        zones[6].rect = &bottomRight;
        zones[6].cursor = cursorNSDiagonal;
        zones[7].rect = &bottomLeft;
        zones[7].cursor = cursorSNDiagonal;
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    void controlWorkSpace();
    int findZone();
    void setFrameZones();
    void controlScreenResizing();
    void setFrameCursor();

    virtual void screenChanged() override;

    virtual int getMBCondition() override { return clicked; };

    virtual Vector getMousePos() override { return mousePos; }
    
    virtual Window* getActiveWindow() override { return activeWindow; };

    virtual void setActiveWindow(Window* window) override { activeWindow = window; };
};