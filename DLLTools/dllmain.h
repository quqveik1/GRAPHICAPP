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

    ToolLay* toolLay = NULL;
    ProgrammeDate* appData = NULL;

    bool draggedLastTime = false;
    Vector lastTimeMP = {};

    Line(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }


    void controlMoving();

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay *toollay, HDC dc = NULL);
    virtual bool edit(ProgrammeDate* data, ToolLay* toollay, HDC dc = NULL);
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

struct RectangleTool : Tool
{
    RectangleTool(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay, HDC dc = NULL);
};

struct EllipseTool : Tool
{
    EllipseTool(const char* _name, const int _ToolSaveLen, HDC _dc, AbstractAppData* _data) :
        Tool(_name, _ToolSaveLen, _dc, _data)
    {
    }

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
};