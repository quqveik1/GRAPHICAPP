#pragma once
#include "..\DrawBibliothek.h"
#include "PointSave.cpp"
#include "..\TransferToolStructure.h"
#include "ToolExportData.cpp"
#include "..\ProgrammeDate.h"
#include "..\Tool.cpp"
#include "..\..\BaseFunctions.cpp"

extern "C" __declspec (dllexport) DLLToolExportData * initDLL(AbstractAppData* data);


AbstractAppData* TheApp = NULL;

struct ColorSave : ToolData
{
    COLORREF color;
    ColorSave(COLORREF _color = DrawColor) :
        color(_color)
    {}

};


struct Line : Tool
{
    ToolSave saveTool = {};

    const int controlSquareLength = 4;
    Rect* controlSquare = new Rect[controlSquareLength]{};
    Vector controlSquareSize = { 10, 10 };
    Vector deltaForButtons = {};
    int activeControlSquareNum = -1;
    bool draggedLastTime = false;
    Vector lastTimeMP = {};

    Line(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }


    void controlMoving();
    void controlLeftButton();
    void controlRightButton();
    void setControlSquares();
    void countDeltaButtons();
    virtual void countToolZone();
    void drawControlButtons(HDC outDC);

    ToolSave* getToolData() { return (ToolSave*)toolLay->getToolsData(); };

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay *toollay, HDC dc = NULL);
    virtual bool edit(ToolLay* toollay, HDC dc = NULL);
};

struct Point : Tool
{
    Vector lastPos = {};
    PointSave* pointSave = NULL;

    Point(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }


    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay, HDC dc = NULL);
};

struct Vignette : Tool
{
    COLORREF selectedColor = NULL;

    Vignette(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay, HDC dc = NULL);
};

struct Gummi : Tool
{

    Gummi(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }
    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
};

struct RectangleTool : Line
{
    

    RectangleTool(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Line(_name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void countToolZone();

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay, HDC dc = NULL);
};

struct EllipseTool : Line
{
    EllipseTool(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Line(_name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay, HDC dc = NULL);
};