#pragma once
#include "wtypes.h"
#include "DrawBibliothek.h"
#include "DLLFilters\DLLExportData.h"
#include "DLLManager.cpp"


struct DLLFiltersManager : DLLManager 
{
    CFilter* dllWindows[10] = {};

    DLLFiltersManager(const char *_pathToDLLList = NULL, AbstractAppData* _appData = NULL) :
        DLLManager (_pathToDLLList, _appData)
    { 
        fileExtension = "filter";
    }

    void addToManager(Manager* manager);

};
