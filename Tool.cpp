#pragma once
#include "Tool.h"



bool Tool::firstUse(ProgrammeDate* data, void* output, Vector currentPos)
{
    assert(data && output);
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = currentPos;
        return true;
    }
    return false;
}

void Tool::finishUse()
{
    workedLastTime = false;
}

HDC Tool::getOutDC()
{
        if (isFinished(toolLay)) return toolLay->lay->getPermanentDC();
        else                     return toolLay->lay->getOutputDC();
}


bool Tool::isFinished(ToolLay* data) 
{ 
    return ((ToolData*)data->getToolsData())->isFinished;
};

int Tool::destroy(ToolLay* toollay)
{
    toolLay = toollay;
    if (toolLay)
    {
        void* data = toolLay->getToolsData();
        if (data)
        {
            delete[] data;
            return 0;
        }
    }

    return (int)"ERROR";
}

HDC Tool::getDC()
{
    return iconDC;
}


const char* Tool::getName()
{
    return name;
}



