#pragma once
#include "wtypes.h"
#include "DrawBibliothek.h"
#include "DLLFilters\DLLExportData.h"
#include "TransferStructure.h"

int findSymbol(const char* text, int size, char symbol);

struct DLLManager
{
    CSystemSettings* systemSettings;

    const char* pathToDLLList = NULL;
    const int size = 2;
    int currLen = 0;
    HMODULE libs[10] = {};
    AbstractAppData* appData = NULL;
    int currLoadWindowNum = 0;
    const char* fileExtension = NULL;

    DLLManager(CSystemSettings* _systemSettings, const char* _pathToDLLList = NULL, AbstractAppData* _appData = NULL) :
        pathToDLLList(_pathToDLLList),
        appData(_appData),
        systemSettings(_systemSettings)
    {
    }

    bool loadLibs ();

};
