#pragma once
#include "DLLFiltersManager.h"
#include<io.h>








void DLLFiltersManager::addToManager(Manager* manager)
{

    if (debugMode) printf("Инструменты начали добавляться в прожку\n");
    if (debugMode) printf("currLen: %d\n", currLen);
    for (int i = 0; i < currLen; i++)
    {
        DLLExportData* (*initDll) (AbstractAppData * data) = (DLLExportData * (*) (AbstractAppData * data))GetProcAddress(libs[i], "initDLL");
        if (debugMode) printf("Инструмент добавился в прожку\n");
        if (!initDll) assert(!"функция не загрузилась");
        DLLExportData* exportData = initDll(appData);

        for (int j = 0; j < exportData->currlen; j++)
        {
            assert(exportData->filters[j]);
            dllWindows[currLoadWindowNum] = exportData->filters[j];
            manager->addWindow(exportData->filters[j]);
            currLoadWindowNum++;
        }
    }

    /*
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
    }  */
}
