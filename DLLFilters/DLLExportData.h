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
    CFilter* filters[10] = {};
    int currlen = 0;
    int lastGuid = 0;

    DLLExportData(int _size) :
        size(_size)
    {
    }

    void addFilter(CFilter* filter);
    
    virtual CFilter* addToManager(Manager* manager);
};
