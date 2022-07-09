#pragma once
#include "CLay.h"

void CLay::createLay(AbstractAppData* _app, Vector _size /* = {}*/)
{
    assert(_app);
    app = _app;

    toolLays = new ToolLay* [app->systemSettings->ONELAYTOOLSLIMIT];
    for (int i = 0; i < app->systemSettings->ONELAYTOOLSLIMIT; i++)
    {
        toolLays[i] = new ToolLay;
    }

    if (_size == _size.getNullVector()) _size = app->systemSettings->DCVECTORSIZE;

    lay.createLay(app, _size);
}


void CLay::addToolLay(ToolLay* tool)
{
    assert(toolLength < app->systemSettings->ONELAYTOOLSLIMIT);

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
    if (activeToolNum < 0 || !toolLays) return NULL;
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
        HDC outDC = toolLays[toollay]->drawTool();
        if (outDC != lay.outputLay && outDC) app->transparentBlt(lay.outputLay, 0, 0, 0, 0, outDC);
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
