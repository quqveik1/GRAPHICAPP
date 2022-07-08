#pragma once

#include "ProgrammeDate.h"
#include "Lay.h"
#include "TransferStructure.h"
#include "CLay.h"
#include "MainTools.h"
#include "DllSettings.h"

struct Tool;
struct ToolLay;


struct ToolData
{
    int byteLength = 0;
    bool isFinished = false;
    bool isStarted = false;

    virtual int getByteSize();
};

int ToolData::getByteSize()
{
    byteLength = sizeof(*this);
    return byteLength;
}

struct ToolZoneSave : ToolData
{
    Vector pos = {};
    Vector size = {};
};



struct ToolSave : ToolZoneSave
{
    COLORREF color = NULL;
    int thickness = NULL;
    const char* name = NULL;

    ToolSave(Vector _pos, Vector _size, COLORREF _color, int _thickness, const char* _name) :
        color(_color),
        thickness(_thickness),
        name(_name)
    {
        pos = _pos;
        size = _size;
    }

    ToolSave() :
        name(NULL)
    {}

};

struct Tool
{
    HDC iconDC = NULL; //åå èçîáðàæåíèå

    const char* name = NULL;

    Vector startPos = {};
    bool workedLastTime = false;
    const int ToolSaveLen = 0;

    ToolData* toolData = NULL;
    ÑDllSettings* dllSettings;

    AbstractAppData* app = NULL;
    ToolLay* toolLay = NULL;
    ProgrammeDate* appData = NULL;

    Tool(ÑDllSettings* _dllSettings, const char* _name, const int _ToolSaveLen, HDC _iconDC = NULL, AbstractAppData* _app = NULL) :
        name(_name),
        iconDC(_iconDC),
        ToolSaveLen(_ToolSaveLen),
        app(_app),
        dllSettings (_dllSettings)
    {}


    bool firstUse(ProgrammeDate* data, void* output, Vector currentPos);
    void finishUse();
    HDC getOutDC();

    virtual HDC getDC();
    virtual const char* getName();
    virtual bool isFinished(ToolLay* data);
    virtual bool isStarted(ToolLay* data) { return ((ToolData*)data->getToolsData())->isStarted; };

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void *output);
    virtual HDC load(ToolLay* toollay, HDC dc = NULL);
    virtual bool edit(ToolLay* toollay, HDC dc = NULL) { return 1; };
};



