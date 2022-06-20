#pragma once
#include "DLLToolsManager.h"

void DLLToolsManager::addToManager(CToolManager* manager)
{
    if (systemSettings->debugMode) printf("����������� ������ ����������� � ������\n");
    if (systemSettings->debugMode) printf("currLen: %d\n", currLen);
    for (int i = 0; i < currLen; i++)
    {
        DLLToolExportData* (*initDll) (AbstractAppData * data) = (DLLToolExportData * (*) (AbstractAppData * data))GetProcAddress(libs[i], "initDLL");
        if (systemSettings->debugMode) printf("���������� ��������� � ������\n");
        if (!initDll) assert(!"������� �� �����������");
        DLLToolExportData* exportData = initDll(appData);

        for (int j = 0; j < exportData->currlen; j++)
        {
            assert(exportData->tools[j]);
            manager->addTool(exportData->tools[j]);
        }
    }
}
