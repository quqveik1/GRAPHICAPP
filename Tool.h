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
    HDC dc = NULL; //ее изображение

    const char* name = NULL;

    Vector startPos = {};
    bool workedLastTime = false;
    const int ToolSaveLen = 0;

    int clicked = 0;

    AbstractAppData* app = NULL;

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
    virtual void setMBCondition(int mbCond);

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output);
    virtual void load(ToolLay* toollay, HDC dc = NULL);
    virtual bool edit(ProgrammeDate* data, ToolLay* toollay, HDC dc = NULL) { return 0; };
};



