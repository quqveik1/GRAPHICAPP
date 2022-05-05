#pragma once
#include "DLLExportData.h"

void DLLExportData::addFunc(CFilter* (*func)())
{
    if (currlen >= size)
    {
        printf("Мест для новых функция нет");
        return;
    }
    funcs[currlen].func = func;
    funcs[currlen].guid = ++lastGuid;
    currlen++;
    
   
}


CFilter* DLLExportData::addToManager(Manager* manager)
{
    for (int i = 0; i < currlen; i++)
    {
        manager->addWindow(funcs[i].func());
    }
    return 0;
}