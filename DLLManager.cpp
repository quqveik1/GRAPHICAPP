#pragma once
#include "DLLManager.h"

int findSymbol(const char* text, int size, char symbol)
{
    for (int i = 0; i < size; i++)
    {
        if (text[i] == symbol) return i + 1;
    }
    return 0;
}


bool DLLManager::loadLibs()
{
    bool result = true;
    FILE* libsList = fopen(pathToDLLList, "r");
    if (!libsList) printf("%s Не найден\n", pathToDLLList);
    assert(libsList);

    for (int i = 0; ; i++)
    {
        char path[100] = {};

        if (!fscanf(libsList, "%s ", path)) break;
        if (path[0] == 0) break;
        int numOfStarSymbol = findSymbol(path, 100, '*');
        if (numOfStarSymbol)
        {
            numOfStarSymbol--;
            path[numOfStarSymbol] = NULL;
            char pattern[100] = {};
            sprintf(pattern, "%s*.%s", path, fileExtension);

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

                if (systemSettings->debugMode) printf("Путь к библиотеке: %s\n", fullPath);
            }
        }
        else
        {
            libs[i] = LoadLibrary(path);
            result *= (int)libs[i];
            if (result) currLen++;

            if (systemSettings->debugMode) printf("Путь к библиотеке: %s\n", path);
        }
    }
    if (systemSettings->debugMode) printf("Инструменты(DLL) загрузились в exe\n");

    return result;
}