#pragma once
#include "DLLFiltersManager.h"
#include<io.h>








void DLLFiltersManager::addToManager(Manager* manager)
{
    for (int i = 0; i < currLen; i++)
    {
        DLLExportData* (*initDll) (AbstractAppData* data) = (DLLExportData * (*) (AbstractAppData * data))GetProcAddress(libs[i], "initDLL");
        if (!initDll) assert(!"функция не загрузилась");
        DLLExportData* exportData = initDll(appData);

        for (int i = 0; i < exportData->currlen; i++)
        {
            dllWindows[currLoadWindowNum] = exportData->funcs[i].func();
            manager->addWindow(dllWindows[currLoadWindowNum]);
            currLoadWindowNum++;
        }
    }
}
