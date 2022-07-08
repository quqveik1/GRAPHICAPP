#pragma once
#include "PointSave.cpp"
#include "..\TransferToolStructure.h"
#include "ToolExportData.cpp"
#include "..\ProgrammeDate.h"
#include "..\Tool.cpp"
#include "..\BaseFunctions.cpp"
#include "..\LoadManager.h"
#include "..\DllSettings.h"
#include "CopyDC.cpp"
#include "CadreResizingTool.cpp"

extern "C" __declspec (dllexport) DLLToolExportData* initDLL(AbstractAppData* data);


AbstractAppData* TheApp = NULL;
ÑDllSettings DllSettings;


struct ColorSave : ToolData
{
    COLORREF color;
    ColorSave(COLORREF _color) :
        color(_color)
    {}

};
                                                                                                                                    

struct Tool4Squares : CadreResizingTool
{
    ToolSave *saveTool = NULL;

    

    Tool4Squares(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        CadreResizingTool(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void outputFunc(HDC outdc) = NULL;


    virtual ToolSave* getToolData() { return (ToolSave*)toolLay->getToolsData(); };

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output) override;
    virtual HDC load(ToolLay* toollay, HDC dc = NULL) override;
};


struct Line : Tool4Squares
{

    const int controlSquareLength = 4;
    Rect* controlSquare = new Rect[controlSquareLength]{};
    Vector controlSquareSize = { 10, 10 };
    Vector deltaForButtons = {};
    int activeControlSquareNum = -1;
    bool draggedLastTime = false;
    Vector lastTimeMP = {};

    Line(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
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
    

    Point(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
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

    Vignette(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    } 

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual HDC load(ToolLay* toollay, HDC dc = NULL) { return NULL; };
};

struct Gummi : Point
{

    Gummi(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Point(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void initPointSave();
};

struct RectangleTool : Tool4Squares
{
    

    RectangleTool(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool4Squares(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void countToolZone();
    virtual void outputFunc(HDC outdc);
};

struct EllipseTool : Tool4Squares
{
    EllipseTool(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool4Squares(_dllSettings, _name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual void outputFunc(HDC outdc);
};