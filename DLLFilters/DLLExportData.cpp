#pragma once
#include "DLLExportData.h"

void DLLExportData::addFilter(CFilter* filter)
{
    if (currlen >= size)
    {
        printf("Мест для новых функция нет");
        return;
    }
    filters[currlen] = filter;
    currlen++;
    
   
}


CFilter* DLLExportData::addToManager(Manager* manager)
{
    for (int i = 0; i < currlen; i++)
    {
        manager->addWindow(filters[i]);
    }
    return 0;
}