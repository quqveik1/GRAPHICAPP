#pragma once
#include "ImportDC.h"


bool ImportDC::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    appData = data;
    toolLay = lay;
    assert(appData && lay);
    toolData = (ToolData*)toolLay->getToolsData();
    SaveCopyDC* saveCopyDC = (SaveCopyDC*)toolData;
    assert(toolData);

    if (app->getAsyncKeyState(VK_CONTROL) && app->getAsyncKeyState('V') && !wasClickedLastTime)
    {
        saveCopyDC->dc = app->getBufferDC(&saveCopyDC->buf);

        saveCopyDC->isStarted = true;
        saveCopyDC->isFinished = true;
        saveCopyDC->pos = appData->getMousePos();
        saveCopyDC->size = app->getHDCSize(saveCopyDC->dc);
        saveCopyDC->startSize = saveCopyDC->size;
        saveCopyDC->selectedZone = false;
        toolLay->needRedraw();
        wasClickedLastTime = true;
        countToolZone();
        return 1;
    }
    if (!(app->getAsyncKeyState(VK_CONTROL) && app->getAsyncKeyState('V')))
    {
        wasClickedLastTime = false;
    }

    return 0;
}