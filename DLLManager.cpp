#pragma once
#include "DLLManager.h"

bool DLLManager::loadLibs ()
{
    bool result = true;
    FILE *libsList = fopen (pathToDLLList, "r");
    assert (libsList);

    for (int i = 0; ; i++)
    {
        const char path[100] = {};

        if (!fscanf (libsList, "%s ", path)) break;
        if (path[0] == 0) break;

        libs[i] = LoadLibrary (path);
        result *= (int)libs[i];
        if (result) currLen++;
    }
    return result;
}



void DLLManager::addToManager(Manager* manager)
{
    for (int i = 0; i < size; i++)
    {
        DLLExportData* (*initDll) (AbstractAppData* data) = (DLLExportData * (*) (AbstractAppData * data))GetProcAddress(libs[i], "initDLL");
        if (!initDll) assert("функция не загрузилась");
        DLLExportData* exportData = initDll(appData);

        for (int i = 0; i < exportData->currlen; i++)
        {
            dllWindows[currLoadWindowNum] = exportData->funcs[i].func();
            manager->addWindow(dllWindows[currLoadWindowNum]);
            currLoadWindowNum++;
        }
    }
}
