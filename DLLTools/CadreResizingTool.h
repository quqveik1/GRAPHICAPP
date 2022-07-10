#pragma once
#include "..\Tool.cpp"

struct CadreResizingTool : Tool
{

    const int controlSquareLength = 4;
    Rect* controlSquare = new Rect[controlSquareLength]{};

    int activeControlSquareNum = -1;
    bool draggedLastTime = false;
    Vector lastTimeMP = {};
    Vector controlSquareSize = { 10, 10 };
    Vector deltaForControlButtons = {};
    COLORREF cadreColor = TX_BLACK;

    CadreResizingTool(—DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    void controlMoving();
    void controlLeftButton();
    void controlRightButton();
    void setControlSquares();
    void drawControlButtons(HDC outDC);

    virtual  void countDeltaForControlButtons();
    virtual void countToolZone();

    bool edit(ToolLay* toollay, HDC dc = NULL) override;
};