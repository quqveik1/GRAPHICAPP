#pragma once
#include "DLLManager.h"
#include "ToolExportData.h"


struct DLLToolsManager : DLLManager
{
    DLLToolsManager(const char* _pathToDLLList = NULL, AbstractAppData* _appData = NULL) :
        DLLManager (_pathToDLLList, _appData)
    {
        fileExtension = "tool";
    }
    void addToManager(CToolManager* manager);
};