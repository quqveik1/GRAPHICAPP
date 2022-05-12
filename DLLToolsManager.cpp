#pragma once
#include "DLLToolsManager.h"

void DLLToolsManager::addToManager(CToolManager* manager)
{
    for (int i = 0; i < currLen; i++)
    {
        DLLToolExportData* (*initDll) (AbstractAppData * data) = (DLLToolExportData * (*) (AbstractAppData * data))GetProcAddress(libs[i], "initDLL");
        if (!initDll) assert(!"функция не загрузилась");
        DLLToolExportData* exportData = initDll(appData);

        for (int j = 0; j < exportData->currlen; j++)
        {
            assert(exportData->tools[j]);
            manager->addTool(exportData->tools[j]);
        }
    }
}
