#pragma once
#include "DLLManager.h"
#include<io.h>


bool DLLManager::loadLibs ()
{
    bool result = true;
    FILE *libsList = fopen (pathToDLLList, "r");
    assert (libsList);

    for (int i = 0; ; i++)
    {
        char path[100] = {};

        if (!fscanf (libsList, "%s ", path)) break;
        if (path[0] == 0) break;
        int numOfStarSymbol = findSymbol(path, 100, '*');
        if (numOfStarSymbol)
        {
            numOfStarSymbol--;
            path[numOfStarSymbol] = NULL;
            char pattern[100] = {};
            sprintf(pattern, "%s*.filter", path);

            struct _finddata_t fileinfo;
            int placeData = 0;
            

            for (int j = 0; ; j++)
            {
                int returnableValue = 0;
                if (j == 0)
                {
                    placeData = _findfirst(pattern, &fileinfo);
                    returnableValue = placeData;
                }
                if (j != 0)
                {
                    returnableValue = _findnext(placeData, &fileinfo);
                }
                if (returnableValue < 0) break;

                char fullPath[100] = {};

                assert(fileinfo.name);

                sprintf(fullPath, "%s%s", path, fileinfo.name);

                libs[i + j] = LoadLibrary(fullPath);
                result *= (int)libs[i + j];
                if (result) currLen++;
            }
        }
        else
        {
            libs[i] = LoadLibrary(path);
            result *= (int)libs[i];
            if (result) currLen++;
        }
    }
    return result;
}

int findSymbol(const char* text, int size, char symbol)
{
    for (int i = 0; i < size; i++)
    {
        if (text[i] == symbol) return i+1;
    }
    return 0;
}



void DLLManager::addToManager(Manager* manager)
{
    for (int i = 0; i < size; i++)
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
