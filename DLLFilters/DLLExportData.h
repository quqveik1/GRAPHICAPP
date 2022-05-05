#pragma once
#include "..\CFilter.h"
#include "..\DrawBibliothek.h"

struct DLLFunc
{
    CFilter* (*func)() = NULL;
    int guid = 0;
};


struct DLLExportData
{
    int size;
    DLLFunc* funcs = NULL;
    int currlen = 0;
    int lastGuid = 0;

    DLLExportData(int _size) :
        size(_size),
        funcs(new DLLFunc[_size])
    {
    }

    virtual void addFunc(CFilter* (*func)());
    virtual CFilter* addToManager(Manager* manager);
};
