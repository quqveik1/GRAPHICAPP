#include "MainTools.h"
void ToolLay::needRedraw()
{
    lay->needRedraw();
}

Tool* ToolLay::getTool()
{
    return tool;
}

bool ToolLay::useTool(ProgrammeDate* data)
{
    assert(tool);
    assert(data);
    return tool->use(data, this, toolsData);
}

void ToolLay::drawTool(HDC dc /*= NULL*/)
{
    assert(tool);
    tool->load(this, dc);
}

void ToolLay::editTool(ProgrammeDate* data)
{
    assert(data);
    HDC outDC = lay->lay.outputLay;

    isEditing = !tool->edit(data, this, outDC);



}


bool ToolLay::isInToolZone(ProgrammeDate* data, Vector mp, int mbCondition)
{
    toolZone.countFinishPos();
    if (toolZone.inRect(mp))
    {
        HDC outDC = lay->lay.outputLay;
        tool->setMBCondition(mbCondition);
        isEditing = !tool->edit(data, this, outDC);
        return isEditing;
    }
    return false;
}