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
    Vector pos = data->mousePos;
    firstUse(data, output, pos);
    *(app->currColor) = txGetPixel(pos.x, pos.y, lay->lay->getPermanentDC());

    ColorSave colorsave(*(app->currColor));
    *(ColorSave*)output = colorsave;

    finishUse();
    
    return true;
}

void Vignette::load(ToolLay* toollay, HDC dc)
{


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
    assert(data && lay && output);
    lay->needRedraw();
    Vector pos = data->mousePos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
        lay->isToolFinished = false;
    }

    saveTool.pos = startPos;
    saveTool.size = pos - startPos;
    saveTool.color = data->color;
    saveTool.thickness = data->size.x;
    saveTool.name = (const char*)1;
    (*(ToolSave*)output) = saveTool;
    appData = data;
    toolLay = lay;

    if (clicked == 2)
    {
        workedLastTime = false;
        toolLay->isToolFinished = true;
        countToolZone();
        
        return true;
    }  

    return false;
}

void RectangleTool::load(ToolLay* toollay, HDC dc)
{
    assert(toollay);
    ToolSave* rectDate = (ToolSave*)toollay->getToolsData();

    toolLay = toollay;

    HDC outDC = dc;
    if (!dc)
    {
        if (toollay->isToolFinished) outDC = toollay->lay->getPermanentDC();
        else                         outDC = toollay->lay->getDCForToolLoad();
    }

    app->setColor(rectDate->color, outDC, rectDate->thickness);
    txRectangle(rectDate->pos.x, rectDate->pos.y, rectDate->pos.x + rectDate->size.x, rectDate->pos.y + rectDate->size.y, outDC);
}




void RectangleTool::countToolZone()
{
    ToolSave* rectDate = (ToolSave*)toolLay->getToolsData();
    toolLay->toolZone = { .pos = rectDate->pos, .finishPos = rectDate->pos + rectDate->size };
}


bool EllipseTool::use(ProgrammeDate* data, ToolLay* lay, void* output)
{      
    assert(data && lay && output);
    lay->needRedraw();
    Vector pos = data->mousePos;
    if (!workedLastTime || clicked == 1)
    {
        workedLastTime = true;
        startPos = pos;
        lay->isToolFinished = false;
    }

    Vector ellipsePos = (startPos + pos) / 2;
    Vector ellipseSize = (startPos - pos) / 2;

    saveTool.pos = startPos;
    saveTool.size = pos - startPos;
    saveTool.color = data->color;
    saveTool.thickness = data->size.x;
    saveTool.name = (const char*)1;
    (*(ToolSave*)output) = saveTool;
    appData = data;
    toolLay = lay;


    {
        workedLastTime = false;
        toolLay->isToolFinished = true;
        countToolZone();
        return true;
    }    

    return false;

}

void EllipseTool::load(ToolLay* toollay, HDC dc)
{
    toolLay = toollay;
    HDC outDC = dc;
    if (!dc)
    {
        if (toollay->isToolFinished) outDC = toollay->lay->getPermanentDC();
        else                         outDC = toollay->lay->getDCForToolLoad();
    }

    ToolSave* rectDate = (ToolSave*)toollay->getToolsData();

    Vector ellipsePos  = rectDate->pos + (rectDate->size) / 2;
    Vector ellipseSize = (rectDate->size) / 2;

    app->setColor(rectDate->color, outDC, rectDate->thickness);
    txEllipse(ellipsePos.x - ellipseSize.x, ellipsePos.y - ellipseSize.y, ellipsePos.x + ellipseSize.x, ellipsePos.y + ellipseSize.y, outDC);
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
        toolLay->isToolFinished = false;
    }
    saveTool.pos = startPos;
    saveTool.size = pos - startPos;
    saveTool.color = data->color;
    saveTool.thickness = data->size.x;
    saveTool.name = (const char*)1;
    
    
    

    *(ToolSave*)output = saveTool;


    if (clicked == 2)
    {
        finishUse();
        lay->isToolFinished = true;

        setControlSquares();
        countDeltaButtons();
        countToolZone();

        
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

bool Line::edit(ToolLay* toollay, HDC dc/* = NULL*/)
{
    assert(toollay);
    printf("Tool clicked: %d\n", clicked);
    printf("Toolzone pos: {%lf, %lf}\n", toolLay->toolZone.pos.x, toolLay->toolZone.pos.y);
    toolLay = toollay;
    ToolSave* toolDate = getToolData();
    countDeltaButtons();
    countToolZone();
    setControlSquares();
    controlMoving();


    app->setColor(TX_WHITE, dc, 1);
    drawCadre(toolLay->toolZone, dc);

    for (int i = 0; i < controlSquareLength; i++)
    {
        Rect drawRect = controlSquare[i] + toollay->toolZone.pos;
        app->rectangle(drawRect, dc);
    }

    return false;
}


void Line::countDeltaButtons()
{
    int isSizePositivX = (toolLay->toolZone.getSize().x > 0);
    int isSizePositivY = (toolLay->toolZone.getSize().y > 0);
    deltaForButtons = { (!isSizePositivX) * ((-controlSquareSize.x) * 2) + controlSquareSize.x, (!isSizePositivY) * ((-controlSquareSize.y) * 2) + controlSquareSize.y };
}

void Line::countToolZone()
{
    ToolSave* toolDate = (ToolSave*)toolLay->getToolsData();
    toolLay->toolZone = { .pos = toolDate->pos - deltaForButtons, .finishPos = toolDate->size + toolDate->pos + (deltaForButtons) };
}

void Line::drawControlButtons(HDC outDC)
{
    for (int i = 0; i < controlSquareLength; i++)
    {
        Rect drawRect = controlSquare[i] + toolLay->toolZone.pos;
        app->rectangle(drawRect, outDC);
    }
}


void Line::setControlSquares()
{
    Vector size = toolLay->toolZone.getSize();
    controlSquare[0] = { .pos = {0, 0}, .finishPos = deltaForButtons };
    controlSquare[1] = { .pos = {toolLay->toolZone.getSize().x - deltaForButtons.x, 0}, .finishPos = {toolLay->toolZone.getSize().x, deltaForButtons.y} };
    controlSquare[2] = { .pos = toolLay->toolZone.getSize() - deltaForButtons, .finishPos = toolLay->toolZone.getSize() };
    controlSquare[3] = { .pos = {0, toolLay->toolZone.getSize().y - deltaForButtons.y}, .finishPos = {deltaForButtons.x, toolLay->toolZone.getSize().y} };
}



void Line::controlMoving()
{
    controlLeftButton();
    controlRightButton();

    lastTimeMP = appData->mousePos;
}


void Line::controlLeftButton()
{
    ToolSave* toolDate = getToolData();

    if (clicked == 1 && activeControlSquareNum < 0)
    {
        Vector mp = appData->getMousePos() - toolLay->toolZone.pos;
        for (int i = 0; i < controlSquareLength; i++)
        {
            if (controlSquare[i].inRect(mp))
            {
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
            toolLay->toolZone.pos += deltaMP;
            toolLay->toolZone.finishPos -= deltaMP;
            toolDate->pos += deltaMP;
            toolDate->size -= deltaMP;
        }

        if (activeControlSquareNum == 1)
        {
            toolLay->toolZone.pos.y += deltaMP.y;
            toolDate->pos.y += deltaMP.y;
            toolDate->size.y -= deltaMP.y;
            toolLay->toolZone.finishPos.x += deltaMP.x;
            toolDate->size.x += deltaMP.x;
        }

        if (activeControlSquareNum == 2)
        {
            toolLay->toolZone.finishPos += deltaMP;
            toolDate->size += deltaMP;
        }

        if (activeControlSquareNum == 3)
        {
            toolLay->toolZone.pos.x += deltaMP.x;
            toolDate->pos.x += deltaMP.x;
            toolDate->size.x -= deltaMP.x;
            toolLay->toolZone.finishPos.y += deltaMP.y;
            toolDate->size.y += deltaMP.y;
        }

    }

    if (clicked != 1 && activeControlSquareNum >= 0)
    {
        activeControlSquareNum = -1;
        toolLay->needRedraw();
    }

}


void Line::controlRightButton()
{
    ToolSave* toolDate = getToolData();

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
        toolLay->needRedraw();
    }

}
