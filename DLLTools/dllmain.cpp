// dllmain.cpp : Defines the entry point for the DLL application.
//DLLTools
#include "dllmain.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



DLLToolExportData* initDLL(AbstractAppData* data)
{
    TheApp = data;
    DLLToolExportData* dllData = new DLLToolExportData(6);

    dllData->addTool(new Line            ("Линия", sizeof(ToolSave),                                       LoadManager.loadImage("Line.bmp"), data));
    dllData->addTool(new Point           ((const char*)(2), sizeof(PointSave) + sizeof(ToolSave) * POINTSAVELENGTH, LoadManager.loadImage("Pen.bmp"), data));
    dllData->addTool(new Vignette        ((const char*)(3), sizeof(ColorSave),                                      LoadManager.loadImage("Vignette.bmp"), data));
    dllData->addTool(new Gummi           ((const char*)(4), sizeof(ToolSave),                                       LoadManager.loadImage("Gummi.bmp"), data));
    dllData->addTool(new RectangleTool   ((const char*)(5), sizeof(ToolSave),                                       LoadManager.loadImage("Rectangle.bmp"), data));
    dllData->addTool(new EllipseTool     ((const char*)(6), sizeof(ToolSave),                                       LoadManager.loadImage("Ellipse.bmp"), data));

    return dllData;
}






bool Vignette::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    /*
    Vector pos = data->mousePos + data->canvasCoordinats;
    firstUse(data, output, pos);
    *(app->currColor) = txGetPixel(pos.x, pos.y, lay->lay);

    ColorSave colorsave(*(app->currColor));
    *(ColorSave*)output = colorsave;

    finishUse();
    */
    return true;
}

void Vignette::load(ToolLay* toollay, HDC dc)
{
    //assert(input && toollay);
    //ColorSave* toolDate = (ColorSave*)input;
    //*(app->currColor) = toolDate->color;
}

bool Gummi::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    /*
    assert(data && lay && output && lay->outputLay);
    Vector pos = data->mousePos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }

    app->setColor(data->backGroundColor, lay->lay, data->size.x);
    txEllipse(pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, lay->lay); 

    if (clicked != 1)
    {
        workedLastTime = false;

        ToolSave saveTool(startPos + data->canvasCoordinats, { (double)data->gummiThickness, (double)data->gummiThickness }, data->backGroundColor, data->size.x, (const char*)2);

        *(ToolSave*)output = saveTool;

        return true;
    }  &*/

    return false;
}

bool RectangleTool::use(ProgrammeDate* data, ToolLay* lay, void* output)
{    
    /*
    assert(data && lay && output && lay->outputLay);
    Vector pos = data->mousePos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }

    app->setColor(data->color, lay->outputLay, data->size.x);
    txRectangle(startPos.x, startPos.y, pos.x, pos.y, lay->outputLay);

    if (clicked == 2)
    {
        txTransparentBlt(lay->lay, 0, 0, 0, 0, lay->outputLay, 0, 0, TRANSPARENTCOLOR);

        ToolSave saveTool(startPos + data->canvasCoordinats, pos - startPos, data->color, data->size.x, (const char*)2);
        (*(ToolSave*)output) = saveTool;
        workedLastTime = false;
        return true;
    }  */

    return false;
}

void RectangleTool::load(ToolLay* toollay, HDC dc)
{
    assert(toollay);
    //ToolSave* rectDate = (ToolSave*)input;
    //app->setColor(rectDate->color, finalDC, rectDate->thickness);


    //txRectangle(rectDate->pos.x, rectDate->pos.y, rectDate->pos.x + rectDate->size.x, rectDate->pos.y + rectDate->size.y, finalDC);
}

bool EllipseTool::use(ProgrammeDate* data, ToolLay* lay, void* output)
{      
    /*
    assert(data && lay && output);
    Vector pos = data->mousePos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }

    app->setColor(data->color, lay->outputLay, data->size.x);
    Vector ellipsePos = (startPos + pos) / 2;
    Vector ellipseSize = (startPos - pos) / 2;
    txEllipse(ellipsePos.x - ellipseSize.x, ellipsePos.y - ellipseSize.y, ellipsePos.x + ellipseSize.x, ellipsePos.y + ellipseSize.y, lay->outputLay);
    

    if (clicked == 2)
    {
        txTransparentBlt(lay->lay, 0, 0, 0, 0, lay->outputLay, 0, 0, TRANSPARENTCOLOR);

        ToolSave saveTool(startPos + data->canvasCoordinats, pos - startPos, data->color, data->size.x, (const char*)2);
        (*(ToolSave*)output) = saveTool;
        workedLastTime = false;
        return true;
    }    */

    return false;

}

bool Point::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    /*
    assert(data && lay && output && lay->outputLay);
    Vector pos = data->mousePos;
    if (firstUse(data, output, pos))
    {
        if (pointSave) delete(pointSave);
        pointSave = new PointSave(100);
    }
    app->setColor(data->color, lay->lay, data->size.x);
    txEllipse(pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, lay->lay);

    if (lastPos != pos)
    {
        ToolSave saveTool(pos + data->canvasCoordinats, data->size, data->color, data->size.x, (const char*)2);
        pointSave->addPoint(saveTool);
    }

    if (clicked != 1)
    {
        *((PointSave*)output) = *pointSave;

        finishUse();

        return true;
    }

    lastPos = pos; */

    return false;
}

void Point::load(ToolLay* toollay, HDC dc)
{
    //assert(input && toollay);
    //PointSave* pointsDate = (PointSave*)input;
    //if (pointsDate->currentLength > 0) app->setColor(pointsDate->points[1].color, finalDC, pointsDate->points[1].thickness);

   // for (int i = 0; i < pointsDate->currentLength; i++)
    {
        //txEllipse(pointsDate->points[i].pos.x - pointsDate->points[i].size.x, pointsDate->points[i].pos.y - pointsDate->points[i].size.y, pointsDate->points[i].pos.x + pointsDate->points[i].size.x, pointsDate->points[i].pos.y + pointsDate->points[i].size.y, finalDC);
    }

}


bool Line::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    assert(data && lay && output);
    appData = data;
    toolLay = lay;

    lay->needRedraw();

    Vector pos = data->mousePos;
    if (!workedLastTime || clicked == 1)
    {
        startPos = pos;
        //printf("StartPos %d||", (int)startPos.x);
        workedLastTime = true;
    }
    saveTool.pos = startPos;
    saveTool.size = pos - startPos;
    saveTool.color = data->color;
    saveTool.thickness = data->size.x;
    saveTool.name = (const char*)1;
    
    lay->toolZone = { .pos = startPos, .size = pos - startPos };
    *(ToolSave*)output = saveTool;


    if (clicked == 2)
    {
        finishUse();
        lay->isToolFinished = true;
        
        return true;
    }

    return false;
}

void Line::load(ToolLay* toollay, HDC dc /* = NULL*/)
{
    assert(toollay);

    toolLay = toollay;

    ToolSave* toolDate = (ToolSave*)toollay->toolsData;

    HDC outDC = dc;
    if (!dc)
    {
        if (toollay->isToolFinished) outDC = toollay->lay->getPermanentDC();
        else                         outDC = toollay->lay->getDCForToolLoad();
    }

    app->setColor(toolDate->color, outDC, toolDate->thickness);
    txLine(toollay->toolZone.pos.x, toollay->toolZone.pos.y, (toolDate->size.x * toollay->size.x) + toollay->toolZone.pos.x, (toolDate->size.y * toollay->size.y) + toollay->toolZone.pos.y, outDC);
    //app->drawOnScreen(toollay->lay->getDCForToolLoad());
    //while (app->getAsyncKeyState('P')) {};
}  

bool Line::edit(ProgrammeDate* data, ToolLay* toollay, HDC dc/* = NULL*/)
{
    assert(data, toollay);
    appData = data;
    toolLay = toollay;
    load(toollay, dc);
    toollay->toolZone.finishPos = toollay->toolZone.pos + toollay->toolZone.size;
    app->setColor(TX_WHITE, dc, 1);
    
    drawCadre(toollay->toolZone, dc);

    int middleVert  = (toollay->toolZone.pos.y + toollay->toolZone.finishPos.y) / 2;
    int middleHor = (toollay->toolZone.pos.x + toollay->toolZone.finishPos.x) / 2;

    txRectangle (toollay->toolZone.pos.x - 10,       middleVert - 10, toollay->toolZone.pos.x + 10, middleVert + 10, dc);
    txRectangle (middleHor - 10, toollay->toolZone.finishPos.y - 10, middleHor + 10, toollay->toolZone.finishPos.y + 10, dc);
    txRectangle (toollay->toolZone.finishPos.x - 10, middleVert - 10, toollay->toolZone.finishPos.x + 10, middleVert + 10, dc);
    txRectangle (middleHor - 10, toollay->toolZone.pos.y - 10,       middleHor + 10, toollay->toolZone.pos.y + 10, dc);

    controlMoving();


    return false;
}


void Line::controlMoving()
{
    if (clicked == 2)
    {
        draggedLastTime = true;
        toolLay->needRedraw();
    }

    if (draggedLastTime)
    {
        toolLay->toolZone.pos += appData->mousePos - lastTimeMP;

        printf("toolZone: {%lf, %lf}\n", toolLay->toolZone.pos.x, toolLay->toolZone.pos.y);
    }

    if (clicked != 2 && draggedLastTime)
    {
        draggedLastTime = false;
    }

    lastTimeMP = appData->mousePos;
}
