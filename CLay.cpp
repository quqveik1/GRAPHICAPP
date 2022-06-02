#include "CLay.h"

void CLay::createLay(Vector _size /* = DCVECTORSIZE*/)
{
    assert(_size > 0);
    lay.createLay(_size);
}


void CLay::addTool(ToolLay* tool)
{
    assert(lastToolNum < ONELAYTOOLSLIMIT - 1);

    lastToolNum++;
    toolLays[lastToolNum] = tool;
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


ToolLay* CLay::getActiveLay()
{
    return toolLays[lastToolNum];
}

HDC CLay::getDCForToolLoad()
{
    return lay.outputLay;
}

void CLay::redraw()
{
    lay.clean();
    lay.clean(lay.outputLay);

    for (int toollay = 0; toollay <= lastToolNum; toollay++)
    {
        toolLays[toollay]->tool->load(toolLays[toollay]);
    }
}

HDC CLay::getPermanentDC()
{
    return lay.lay;
}
