#pragma once
#include "CLay.h"

void CLay::createLay(CSystemSettings* _systemSetings, Vector _size /* = DCVECTORSIZE*/)
{
    assert(_systemSetings);
    systemSetings = _systemSetings;

    toolLays = new ToolLay* [systemSetings->ONELAYTOOLSLIMIT];
    for (int i = 0; i < systemSetings->ONELAYTOOLSLIMIT; i++)
    {
        toolLays[i] = new ToolLay;
    }

    if (_size == _size.getNullVector()) _size = systemSetings->DCVECTORSIZE;

    lay.createLay(systemSetings, _size);
}


void CLay::addToolLay(ToolLay* tool)
{
    assert(toolLength < systemSetings->ONELAYTOOLSLIMIT);

    toolLength++;
    activeToolNum = toolLength - 1;
    toolLays[activeToolNum] = tool;
    tool->lay = this;
}

void CLay::needRedraw()
{
    needToRedraw = true;
} 

void CLay::editTool(ProgrammeDate* data)
{
    if (getActiveToolLay() && getActiveToolLay()->getTool()) getActiveToolLay()->editTool(data);
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
    if (activeToolNum < 0) return NULL;
    return toolLays[activeToolNum];
}

int CLay::getActiveToolLayNum()
{
    return activeToolNum;
}

void CLay::setActiveToolLayNum(int num)
{
    activeToolNum = num;
    needRedraw();
}

int CLay::getCurrentSize()
{
    return toolLength;
}

HDC CLay::getOutputDC()
{
    return lay.outputLay;
}


RGBQUAD* CLay::getOutputBuf()
{
    return lay.outputBuf;
}

ToolLay** CLay::getToolLays()
{
    return &(toolLays[0]);
}

Lay* CLay::getLay()
{
    return &lay;
}

Vector CLay::getLaySize()
{
    return lay.laySize;
}

void CLay::redraw()
{
    lay.clean();
    lay.clean(lay.outputLay);

    for (int toollay = 0; toollay < toolLength; toollay++)
    {
        toolLays[toollay]->drawTool();
    }
}

HDC CLay::getPermanentDC()
{
    return lay.lay;
}

RGBQUAD* CLay::getPermanentBuf()
{
    return lay.layBuf;
}
