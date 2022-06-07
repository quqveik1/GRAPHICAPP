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

    isEditing = !tool->edit(this, outDC);



}


bool ToolLay::isInToolZone(ProgrammeDate* data, Vector mp, int mbCondition)
{
    if (toolZone.inRect(mp))
    {
        HDC outDC = lay->lay.outputLay;
        getTool()->setMBCondition(mbCondition);
        //isEditing = !tool->edit(this, outDC);
        return true;
    }
    return false;
}