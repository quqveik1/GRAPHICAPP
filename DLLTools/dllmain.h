#pragma once
#include "..\DrawBibliothek.h"
#include "PointSave.cpp"
#include "..\TransferToolStructure.h"
#include "ToolExportData.cpp"
#include "..\ProgrammeDate.h"
#include "..\Tool.cpp"
#include "..\BaseFunctions.cpp"
#include "..\LoadManager.h"
#include "..\DllSettings.h"

extern "C" __declspec (dllexport) DLLToolExportData* initDLL(AbstractAppData* data);


AbstractAppData* TheApp = NULL;
�DllSettings DllSettings;


struct ColorSave : ToolData
{
    COLORREF color;
    ColorSave(COLORREF _color) :
        color(_color)
    {}

};
                                                                                                                                    

struct Tool4Squares : Tool
{
    ToolSave *saveTool = NULL;

    const int controlSquareLength = 4;
    Rect* controlSquare = new Rect[controlSquareLength]{};
    Vector controlSquareSize = { 10, 10 };
    Vector deltaForButtons = {};
    int activeControlSquareNum = -1;
    bool draggedLastTime = false;
    Vector lastTimeMP = {};

    Tool4Squares(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }


    void controlMoving();
    void controlLeftButton();
    void controlRightButton();
    void setControlSquares();
    void countDeltaButtons();
    void drawControlButtons(HDC outDC);

    virtual void countToolZone();
    virtual void outputFunc(HDC outdc) = NULL;
    virtual bool isFinished(ToolLay* data);
    virtual bool isStarted(ToolLay* data);


    ToolSave* getToolData() { return (ToolSave*)toolLay->getToolsData(); };

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output) override;
    virtual HDC load(ToolLay* toollay, HDC dc = NULL) override;
    virtual bool edit(ToolLay* toollay, HDC dc = NULL) override;
};


struct Line : Tool4Squares
{
    ToolSave saveTool = {};

    const int controlSquareLength = 4;
    Rect* controlSquare = new Rect[controlSquareLength]{};
    Vector controlSquareSize = { 10, 10 };
    Vector deltaForButtons = {};
    int activeControlSquareNum = -1;
    bool draggedLastTime = false;
    Vector lastTimeMP = {};

    Line(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool4Squares(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }


    virtual void outputFunc(HDC outdc);

    ToolSave* getToolData() { return (ToolSave*)toolLay->getToolsData(); };

};


struct Point : Tool
{
    Vector lastPos = {};
    PointSave* pointSave = NULL;
    

    Point(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void initPointSave();

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output) override;
    virtual HDC load(ToolLay* toollay, HDC dc = NULL) override;
};

struct Vignette : Tool
{
    COLORREF selectedColor = NULL;
    ColorSave* colorSave = NULL;

    Vignette(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    } 

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual HDC load(ToolLay* toollay, HDC dc = NULL) { return NULL; };
};

struct Gummi : Point
{

    Gummi(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Point(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void initPointSave();
};

struct RectangleTool : Tool4Squares
{
    

    RectangleTool(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool4Squares(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void countToolZone();
    virtual void outputFunc(HDC outdc);
};

struct EllipseTool : Tool4Squares
{
    EllipseTool(�DllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool4Squares(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void outputFunc(HDC outdc);
};