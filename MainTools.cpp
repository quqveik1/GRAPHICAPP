#pragma once
#include "MainTools.h"
void ToolLay::needRedraw()
{
    lay->needRedraw();
}

Tool* ToolLay::getTool()
{
    return tool;
}

bool ToolLay::isFinished()
{
    if (!getTool()) return false;
    return getTool()->isFinished(this);
}

bool ToolLay::isStarted()
{
    if (!getTool()) return false;
    return getTool()->isStarted(this);
}

bool ToolLay::useTool(ProgrammeDate* data)
{
    if (!getTool()) return false;
    assert(data);
    return getTool()->use(data, this, toolsData);
}

void ToolLay::drawTool(HDC dc /*= NULL*/)
{
    if (getTool() && isStarted())getTool()->load(this, dc);
}

void ToolLay::editTool(ProgrammeDate* data)
{
    if (!getTool()) return;
    assert(data);
    HDC outDC = lay->lay.outputLay;

    isEditing = !getTool()->edit(this, outDC);
}

int ToolLay::setMBCondition(int condition)
{
    if (!getTool()) return 0;

    getTool()->setMBCondition(condition);
    return 1;
}


bool ToolLay::isInToolZone(ProgrammeDate* data, Vector mp, int mbCondition)
{
    if (toolZone.inRect(mp))
    {
        HDC outDC = lay->lay.outputLay;
        if (!getTool()) return false;
        getTool()->setMBCondition(mbCondition);
        //isEditing = !tool->edit(this, outDC);
        return true;
    }
    return false;
}