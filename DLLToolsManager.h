#pragma once
#include "DLLManager.h"
#include "ToolExportData.h"


struct DLLToolsManager : DLLManager
{
    DLLToolsManager(CSystemSettings* _systemSetings, const char* _pathToDLLList = NULL, AbstractAppData* _appData = NULL) :
        DLLManager (_systemSetings, _pathToDLLList, _appData)
    {
        fileExtension = "tool";
    }
    void addToManager(CToolManager* manager);
};