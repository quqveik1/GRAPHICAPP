#pragma once
#include "MainTools.h"

ToolLay::~ToolLay()
{
    delete toolsData;
}

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
    return getTool()->use(data, this, getToolsData());
}

HDC ToolLay::drawTool(HDC dc /*= NULL*/)
{
    if (getTool() && isStarted()) return getTool()->load(this, dc);
    return NULL;
}

void ToolLay::editTool(ProgrammeDate* data)
{
    if (!getTool()) return;
    assert(data);
    HDC outDC = lay->lay.outputLay;

    isEditing = !getTool()->edit(this, outDC);
}


bool ToolLay::isInToolZone(ProgrammeDate* data, Vector mp)
{
    if (toolZone.inRect(mp))
    {
        HDC outDC = lay->lay.outputLay;
        if (!getTool()) return false;
        //getTool()->setMBCondition(mbCondition);
        //isEditing = !tool->edit(this, outDC);
        return true;
    }
    return false;
}

void* ToolLay::getToolsData()
{
    return toolsData;
}

HDC ToolLay::getPermanentDC()
{
    if (lay) return lay->getPermanentDC();
    else     return NULL;
} 

HDC ToolLay::getOutputDC()
{
    if (lay) return lay->getOutputDC();
    else     return NULL;
}


void ToolLay::addTool(Tool* _tool)
{
    if (tool != _tool)
    {
        if (tool)
        {
            tool->destroy(this);
        }
    }
    tool = _tool;
    if (tool)
    {
        toolsData = new char[tool->ToolSaveLen]{};
    }
    return;
}
