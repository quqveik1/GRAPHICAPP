#pragma once

#include "ProgrammeDate.h"
#include "Lay.h"
#include "TransferStructure.h"
#include "CLay.h"
#include "MainTools.h"
struct Tool;
struct ToolLay;


struct ToolData
{
    int byteLength = 0;

    virtual int getByteSize();
};

int ToolData::getByteSize()
{

    byteLength = sizeof(*this);
    return byteLength;
}



struct ToolSave : ToolData
{
    Vector pos = {};
    Vector size = {};
    COLORREF color = NULL;
    int thickness = NULL;
    const char* name;

    ToolSave(Vector _pos, Vector _size, COLORREF _color, int _thickness, const char* _name) :
        pos(_pos),
        size(_size),
        color(_color),
        thickness(_thickness),
        name(_name)
    {}

    ToolSave() :
        name(NULL)
    {}

};

struct Tool
{
    HDC dc; //�� �����������

    const char* name;

    Vector startPos = {};
    bool workedLastTime = false;
    const int ToolSaveLen;

    int clicked = 0;

    AbstractAppData* app;




    Tool(const char* _name, const int _ToolSaveLen, HDC _dc = NULL, AbstractAppData* _app = NULL) :
        name(_name),
        dc(_dc),
        ToolSaveLen(_ToolSaveLen),
        app(_app)
    {}


    bool firstUse(ProgrammeDate* data, void* output, Vector currentPos);
    void finishUse();

    virtual HDC getDC();
    virtual const char* getName();

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay);
};



