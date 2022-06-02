#include "CLay.h"

void CLay::createLay(Vector _size /* = DCVECTORSIZE*/)
{
    assert(_size > 0);
    lay.createLay(_size);
}


void CLay::addTool(ToolLay* tool)
{
    assert(toolLength < ONELAYTOOLSLIMIT);

    toolLength++;
    activeToolNum = toolLength - 1;
    toolLays[activeToolNum] = tool;
    tool->lay = this;
}

void CLay::needRedraw()
{
    needToRedraw = true;
} 

void CLay::noMoreRedraw()
{
    needToRedraw = false;
}  

bool CLay::redrawStatus()
{
    if  (needToRedraw) return true;
    else               return false;
}


ToolLay* CLay::getActiveToolLay()
{
    return toolLays[activeToolNum];
}

int CLay::getActiveToolLayNum()
{
    return activeToolNum;
}

void CLay::setActiveToolLayNum(int num)
{
    activeToolNum = num;
}

int CLay::getCurrentSize()
{
    return toolLength;
}

HDC CLay::getDCForToolLoad()
{
    return lay.outputLay;
}

ToolLay** CLay::getToolLays()
{
    return &(toolLays[0]);
}

void CLay::redraw()
{
    lay.clean();
    lay.clean(lay.outputLay);

    for (int toollay = 0; toollay < toolLength; toollay++)
    {
        toolLays[toollay]->tool->load(toolLays[toollay]);
    }
}

HDC CLay::getPermanentDC()
{
    return lay.lay;
}
