#pragma once
#include "wtypes.h"
#include "DrawBibliothek.h"
#include "DLLFilters\DLLExportData.h"
#include "TransferStructure.h"

int findSymbol(const char* text, int size, char symbol);

struct DLLManager
{
    CSystemSettings* systemSettings = NULL;

    const char* pathToDLLList = NULL;
    const int size = 2;
    int currLen = 0;
    HMODULE libs[10] = {};
    AbstractAppData* appData = NULL;
    int currLoadWindowNum = 0;
    const char* fileExtension = NULL;

    DLLManager(AbstractAppData* _app, const char* _pathToDLLList = NULL) :
        pathToDLLList(_pathToDLLList),
        appData(_app),
        systemSettings (_app->systemSettings)
    {
    }

    bool loadLibs ();

};
