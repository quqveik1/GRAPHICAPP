#include "CopyDC.h"
#include "..\CanvasManager.h"


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

    Canvas* activeCanvas = NULL;
    Vector canvasLaySize = {};
    if (app->canvasManager) activeCanvas = app->canvasManager->getActiveCanvas();
    if (activeCanvas) canvasLaySize = activeCanvas->getLaySize();

    if (canvasLaySize == canvasLaySize.getNullVector())
    {
        gassert(!"Размер слоя в классе CopyDC не узнался");
    }
    else
    {
        if (saveCopyDC->stretchedDCFullSize != canvasLaySize)
        {
            saveCopyDC->stretchedDCFullSize = canvasLaySize;
            if (saveCopyDC->stretchedDC)app->deleteDC(saveCopyDC->stretchedDC);
            saveCopyDC->stretchedDC = app->createDIBSection(saveCopyDC->stretchedDCFullSize, &saveCopyDC->stretchedBuf);
        }
    }

     /*
    if (isEqual(absSize.x, 0))
    {
        absSize.x = 1;
    }
    if (isEqual(absSize.y, 0))
    {
        absSize.y = 1;
    }
    
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
    */
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


    if (isFinished(toolLay) && !isEqual(saveCopyDC->size.x, 0) && !isEqual(saveCopyDC->size.y, 0))
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

            if (!isEqual (absSize.x, 0) && !isEqual(absSize.y, 0))
            {
                app->stretchBlt(saveCopyDC->stretchedDC, {}, absSize, saveCopyDC->dc, {}, saveCopyDC->startSize);
            }

            if (saveCopyDC->size.x < 0)
            {
                app->verticalReflect(saveCopyDC->stretchedDC, saveCopyDC->stretchedBuf, absSize, saveCopyDC->stretchedDCFullSize);
            }
            if (saveCopyDC->size.y < 0)
            {
                app->horizontalReflect(saveCopyDC->stretchedDC, saveCopyDC->stretchedBuf, absSize, saveCopyDC->stretchedDCFullSize);
            }
            if (absSize > 0)
            {
                app->transparentBlt(outDC, truePos, absSize, saveCopyDC->stretchedDC);
            }

        }
        saveCopyDC->stretchedDCSize = saveCopyDC->size;
    }


    return outDC;
}