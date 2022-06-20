#pragma once
#include "wtypes.h"
#include "DrawBibliothek.h"
#include "DLLFilters\DLLExportData.h"
#include "DLLManager.cpp"


struct DLLFiltersManager : DLLManager 
{
    CFilter* dllWindows[10] = {};

    DLLFiltersManager(CSystemSettings* _systemSettings, const char *_pathToDLLList = NULL, AbstractAppData* _appData = NULL) :
        DLLManager (_systemSettings, _pathToDLLList, _appData)
    { 
        fileExtension = "filter";
    }

    void addToManager(Manager* manager);

};
