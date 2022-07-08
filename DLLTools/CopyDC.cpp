#include "CopyDC.h"


void CopyDC::countToolZone()
{
    SaveCopyDC* saveCopyDC = (SaveCopyDC*)toolData;
    toolLay->toolZone.pos = saveCopyDC->pos;
    toolLay->toolZone.finishPos = saveCopyDC->pos + saveCopyDC->size;
}


void CopyDC::sizeSignControl(Vector* pos, Vector* size)
{
    SaveCopyDC* saveCopyDC = (SaveCopyDC*)toolData;
    if (!(saveCopyDC->size > 0))
    {
        if (isSmaller(size->x, 0))
        {
            pos->x += size->x;
            size->x = fabs(size->x);
        }

        if (isSmaller(size->y, 0))
        {
            pos->y += size->y;
            size->y = fabs(size->y);
        }
    }
}


void CopyDC::controlStretchedDCFullSize(Vector absSize)
{
    SaveCopyDC* saveCopyDC = (SaveCopyDC*)toolData;

    if (app->systemSettings->debugMode >= 2) printf("CopyDC::absSize: {%lf, %lf}\n", absSize.x, absSize.y);
    if (app->systemSettings->debugMode >= 2) printf("CopyDC::saveCopyDC->stretchedDCFullSize: {%lf, %lf}\n", saveCopyDC->stretchedDCFullSize.x, saveCopyDC->stretchedDCFullSize.y);

    if (isSmaller (saveCopyDC->stretchedDCFullSize.x, absSize.x) || isSmaller(saveCopyDC->stretchedDCFullSize.y, absSize.y))
    {
        saveCopyDC->stretchedDCFullSize = absSize * 2;
        app->deleteDC(saveCopyDC->stretchedDC);
        saveCopyDC->stretchedDC = app->createDIBSection(saveCopyDC->stretchedDCFullSize, &saveCopyDC->stretchedBuf);
    }

    if (isBigger(saveCopyDC->stretchedDCFullSize.x * 0.4, absSize.x) || isBigger(saveCopyDC->stretchedDCFullSize.y * 0.4, absSize.y))
    {
        saveCopyDC->stretchedDCFullSize = absSize * 2;
        app->deleteDC(saveCopyDC->stretchedDC);
        saveCopyDC->stretchedDC = app->createDIBSection(saveCopyDC->stretchedDCFullSize, &saveCopyDC->stretchedBuf);
    }
}


bool CopyDC::use (ProgrammeDate* data, ToolLay* lay, void* output)
{
    appData = data;
    toolLay = lay;
    assert(appData && lay);
    toolData = (ToolData*)toolLay->getToolsData();
    SaveCopyDC* saveCopyDC = (SaveCopyDC*)toolData;
    assert(toolData);

    if (appData->clickedMB == 1)
    {
        saveCopyDC->isStarted = true;
        saveCopyDC->pos = appData->getMousePos();
        saveCopyDC->selectedZone = false;
        
    }
    
    if (isStarted(toolLay))
    {
        if (saveCopyDC->size + saveCopyDC->pos != appData->getMousePos()) toolLay->needRedraw();
        if (!saveCopyDC->selectedZone) saveCopyDC->size = appData->getMousePos() - saveCopyDC->pos;

        if (appData->clickedMB == 2 && !saveCopyDC->selectedZone)
        {
            app->smartDeleteDC(saveCopyDC->dc);
            saveCopyDC->dc = app->createDIBSection({ fabs(saveCopyDC->size.x), fabs(saveCopyDC->size.y) });
            saveCopyDC->selectedZone = true;
        }

        if (app->getAsyncKeyState(VK_CONTROL) && app->getAsyncKeyState('C') && saveCopyDC->selectedZone)
        {
            sizeSignControl(&saveCopyDC->pos, &saveCopyDC->size);
            app->bitBlt(saveCopyDC->dc, { 0, 0 }, saveCopyDC->size, toolLay->getPermanentDC(), saveCopyDC->pos);
            saveCopyDC->zoneCopied = true;
        }

        if (app->getAsyncKeyState(VK_CONTROL) && app->getAsyncKeyState('V') && saveCopyDC->zoneCopied)
        {
            saveCopyDC->isFinished = true;
            saveCopyDC->startSize = saveCopyDC->size;
            toolLay->needRedraw();
            countToolZone();
            return true;
        }

    }
    return 0;
}


HDC CopyDC::load(ToolLay* toollay, HDC dc/* = NULL*/)
{
    toolLay = toollay;
    toolData = (ToolData*)toolLay->getToolsData();
    SaveCopyDC* saveCopyDC = (SaveCopyDC*)toolData;

    HDC outDC = dc;
    if (!dc)
    {
        outDC = getOutDC();
    }

    if (isStarted(toolLay) && !isFinished(toolLay))
    {
        app->drawCadre(saveCopyDC->pos, saveCopyDC->pos + saveCopyDC->size, outDC, frameColor, 2);
    }

    if (isFinished(toolLay))
    {
        if (saveCopyDC->size > 0)
        {
            controlStretchedDCFullSize(saveCopyDC->size);
            if (saveCopyDC->stretchedDCSize != saveCopyDC->size)
            {
                app->stretchBlt(saveCopyDC->stretchedDC, {}, saveCopyDC->size, saveCopyDC->dc, {}, saveCopyDC->startSize);
                
            }
            app->transparentBlt(outDC, saveCopyDC->pos, saveCopyDC->size, saveCopyDC->stretchedDC);
        }
        else
        {
            Vector truePos = saveCopyDC->pos;
            Vector absSize = saveCopyDC->size;

            sizeSignControl(&truePos, &absSize);

            controlStretchedDCFullSize(absSize);

            app->stretchBlt(saveCopyDC->stretchedDC, {}, absSize, saveCopyDC->dc, {}, saveCopyDC->startSize);

            if (saveCopyDC->size.x < 0)
            {
                app->verticalReflect(saveCopyDC->stretchedDC, saveCopyDC->stretchedBuf, absSize, saveCopyDC->stretchedDCFullSize);
            }
            if (saveCopyDC->size.y < 0)
            {
                app->horizontalReflect(saveCopyDC->stretchedDC, saveCopyDC->stretchedBuf, absSize, saveCopyDC->stretchedDCFullSize);
            }
            app->transparentBlt(outDC, truePos, absSize, saveCopyDC->stretchedDC);

        }
        saveCopyDC->stretchedDCSize = saveCopyDC->size;
    }


    return outDC;
}