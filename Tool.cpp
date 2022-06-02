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


bool Tool::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    assert(data && lay && output);
    Vector pos = data->mousePos;
    firstUse(data, output, pos);
    (app)->setColor(data->color, lay->lay->getDCForToolLoad(), data->size.x);
    txEllipse(pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, lay->lay->getDCForToolLoad());



    //printf ("1");

    if (clicked != 1 || pos != startPos)
    {
        (app)->setColor(data->color, lay->lay->getPermanentDC (), data->size.x);
        txEllipse(pos.x - data->size.x + data->canvasCoordinats.x, pos.y - data->size.y + data->canvasCoordinats.y, pos.x + data->size.x + data->canvasCoordinats.x, pos.y + data->size.y + data->canvasCoordinats.y, lay->lay->getPermanentDC());

        workedLastTime = false;

        ToolSave saveTool(pos + data->canvasCoordinats, data->size, data->color, data->size.x, (const char*)2);

        *(ToolSave*)output = saveTool;
        return true;
    }

    return false;
}

void Tool::load(ToolLay* toollay)
{
    //assert(input && finalDC);
    //ToolSave* toolDate = (ToolSave*)input;
   // (app)->setColor(toolDate->color, finalDC, toolDate->thickness);
    //txEllipse(toolDate->pos.x - toolDate->size.x, toolDate->pos.y - toolDate->size.y, toolDate->pos.x + toolDate->size.x, toolDate->pos.y + toolDate->size.y, finalDC);
}



HDC Tool::getDC()
{
    return dc;
}


const char* Tool::getName()
{
    return name;
}



