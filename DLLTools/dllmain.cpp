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
    
    int isSizePositivX = (lay->toolZone.size.x > 0);
    int isSizePositivY = (lay->toolZone.size.y > 0);
    deltaForButtons = { (!isSizePositivX) * ((-controlSquareSize.x) * 2) + controlSquareSize.x, (!isSizePositivY) * ((-controlSquareSize.y) * 2) + controlSquareSize.y };
    lay->toolZone = { .pos = startPos - deltaForButtons, .size = (pos - startPos) + (deltaForButtons * 2) };

    *(ToolSave*)output = saveTool;


    if (clicked == 2)
    {
        finishUse();
        lay->isToolFinished = true;



        /*
         _(3)_
        |     |
        (0)  (2)
        |_(1)_|
        */

       
        sortContolSquares();

        
        return true;
    }

    return false;
}

void Line::load(ToolLay* toollay, HDC dc /* = NULL*/)
{
    assert(toollay);

    toolLay = toollay;

    ToolSave* toolDate = (ToolSave*)toollay->getToolsData();

    HDC outDC = dc;
    if (!dc)
    {
        if (toollay->isToolFinished) outDC = toollay->lay->getPermanentDC();
        else                         outDC = toollay->lay->getDCForToolLoad();
    }

    app->setColor(toolDate->color, outDC, toolDate->thickness);
    txLine(toolDate->pos.x, toolDate->pos.y, (toolDate->size.x * toollay->size.x) + toolDate->pos.x, (toolDate->size.y * toollay->size.y) + toolDate->pos.y, outDC);
    //app->drawOnScreen(toollay->lay->getDCForToolLoad());
    //while (app->getAsyncKeyState('P')) {};
}  

bool Line::edit(ProgrammeDate* data, ToolLay* toollay, HDC dc/* = NULL*/)
{
    assert(data, toollay);
    appData = data;
    toolLay = toollay;
    load(toollay, dc);
    toollay->toolZone.countFinishPos();
    app->setColor(TX_WHITE, dc, 1);

    drawCadre(toollay->toolZone, dc);

    sortContolSquares();

    


    app->rectangle(controlSquare[0], dc);
    app->rectangle(controlSquare[1], dc);
    app->rectangle(controlSquare[2], dc);
    app->rectangle(controlSquare[3], dc);
    /*
    txRectangle(toollay->toolZone.pos.x,                                 middleVert - controlSquareSize.y, toollay->toolZone.pos.x + controlSquareSize.x * 2, middleVert + controlSquareSize.y, dc); //(1)
    txRectangle(middleHor - controlSquareSize.x, toollay->toolZone.finishPos.y - controlSquareSize.y * 2, middleHor + controlSquareSize.x, toollay->toolZone.finishPos.y,                       dc);  //(2)
    txRectangle(toollay->toolZone.finishPos.x - controlSquareSize.x * 2, middleVert - controlSquareSize.y, toollay->toolZone.finishPos.x,                     middleVert + controlSquareSize.y, dc); //(3)
    txRectangle(middleHor - controlSquareSize.x, toollay->toolZone.pos.y,                                 middleHor + controlSquareSize.x, toollay->toolZone.pos.y + controlSquareSize.y * 2,  dc); //(4)
    */

    controlMoving();


    return false;
}


void Line::sortContolSquares()
{

    /*
        _(3)_
       |\   |
      (0)\ (2)
       |  \ |
       |___\|
        (1)
       */

    toolLay->toolZone.countFinishPos();

    int middleVert = (toolLay->toolZone.pos.y + toolLay->toolZone.finishPos.y) / 2;
    int middleHor = (toolLay->toolZone.pos.x + toolLay->toolZone.finishPos.x) / 2;
    controlSquare[0] = { .pos = {toolLay->toolZone.pos.x,                                 middleVert - controlSquareSize.y}, .finishPos = {toolLay->toolZone.pos.x + deltaForButtons.x * 2, middleVert + controlSquareSize.y} };
    controlSquare[1] = { .pos = {middleHor - controlSquareSize.x, toolLay->toolZone.finishPos.y - deltaForButtons.y * 2},  .finishPos = {middleHor + controlSquareSize.x, toolLay->toolZone.finishPos.y                    } };
    controlSquare[2] = { .pos = {toolLay->toolZone.finishPos.x - deltaForButtons.x * 2, middleVert - controlSquareSize.y}, .finishPos = {toolLay->toolZone.finishPos.x,                     middleVert + controlSquareSize.y} };
    controlSquare[3] = { .pos = {middleHor - controlSquareSize.x, toolLay->toolZone.pos.y},                                  .finishPos = {middleHor + controlSquareSize.x, toolLay->toolZone.pos.y + deltaForButtons.y * 2} };


    if (toolLay->toolZone.pos.x > toolLay->toolZone.finishPos.x)
    {
        Rect copyRect = controlSquare[2];
        controlSquare[2] = controlSquare[0];
        controlSquare[0] = copyRect;
    }    
    
    if (toolLay->toolZone.pos.y > toolLay->toolZone.finishPos.y)
    {
        Rect copyRect = controlSquare[3];
        controlSquare[3] = controlSquare[1];
        controlSquare[1] = copyRect;
    }
}


void Line::controlMoving()
{

    ToolSave* toolDate = (ToolSave*)toolLay->getToolsData();
    if (clicked == 2)
    {
        draggedLastTime = true;
        toolLay->needRedraw();
    }

    if (draggedLastTime)
    {
        toolLay->toolZone.pos += appData->mousePos - lastTimeMP;
        
        toolDate->pos += appData->mousePos - lastTimeMP;

        printf("toolZone: {%lf, %lf}\n", toolLay->toolZone.pos.x, toolLay->toolZone.pos.y);
    }

    if (clicked != 2 && draggedLastTime)
    {
        draggedLastTime = false;
    }


    if (clicked == 1)
    {
        for (int i = 0; i < controlSquareLength; i++)
        {
            if (controlSquare[i].inRect(appData->getMousePos()))
            {
                isResizing = true;
                activeControlSquareNum = i;
            }
        }
    }

    if (activeControlSquareNum >= 0)
    {
        toolLay->needRedraw();
        Vector deltaMP = appData->mousePos - lastTimeMP;
        if (activeControlSquareNum == 0)
        {
            toolLay->toolZone.pos.x += deltaMP.x;
            toolLay->toolZone.size.x -= deltaMP.x;
            toolDate->pos.x += deltaMP.x;
            toolDate->size.x -= deltaMP.x;
        }

        if (activeControlSquareNum == 1)
        {
            toolLay->toolZone.size.y += deltaMP.y;
            toolDate->size.y += deltaMP.y;
        }

        if (activeControlSquareNum == 2)
        {
            toolLay->toolZone.size.x += deltaMP.x;
            toolDate->size.x += deltaMP.x;
        }

        if (activeControlSquareNum == 3)
        {
            toolLay->toolZone.pos.y += deltaMP.y;
            toolLay->toolZone.size.y -= deltaMP.y;
            toolDate->pos.y += deltaMP.y;
            toolDate->size.y -= deltaMP.y;
        }

    }

    if (clicked != 1 && activeControlSquareNum >= 0) activeControlSquareNum = -1;



    lastTimeMP = appData->mousePos;
}
