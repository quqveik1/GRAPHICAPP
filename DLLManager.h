#pragma once
#include "wtypes.h"
#include "DrawBibliothek.h"
#include "DLLFilters\DLLExportData.h"

struct DLLManager 
{
    const char *pathToDLLList = NULL;
    const int size = 1;
    int currLen = 0;
    HMODULE libs[10] = {};
    AbstractAppData* appData = NULL;
    CFilter* dllWindows[10] = {};
    int currLoadWindowNum = 0;

    DLLManager (const char *_pathToDLLList = NULL, AbstractAppData* _appData = NULL) :
        pathToDLLList (_pathToDLLList),
        appData (_appData)
    {
    }

    bool loadLibs ();
    void addToManager(Manager *manager);

};
