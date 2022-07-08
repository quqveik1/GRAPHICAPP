// dllmain.cpp : Defines the entry point for the DLL application.
//DLLTools
#pragma once
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
    DLLToolExportData* dllData = new DLLToolExportData(7);

    dllData->addTool(new Line            (&DllSettings, "Линия", sizeof(ToolSave),           TheApp->loadManager->loadImage("Line.bmp"), data));
    dllData->addTool(new Point           (&DllSettings, "Точка", sizeof(PointSave),          TheApp->loadManager->loadImage("Pen.bmp"), data));
    dllData->addTool(new Vignette        (&DllSettings, "Виньетка", sizeof(ColorSave), TheApp->loadManager->loadImage("Vignette.bmp"), data));
    dllData->addTool(new Gummi           (&DllSettings, "Ластик", sizeof(ToolSave),  TheApp->loadManager->loadImage("Gummi.bmp"), data));
    dllData->addTool(new RectangleTool   (&DllSettings, "Прямоугольник", sizeof(ToolSave),  TheApp->loadManager->loadImage("Rectangle.bmp"), data));
    dllData->addTool(new EllipseTool     (&DllSettings, "Эллипс", sizeof(ToolSave),  TheApp->loadManager->loadImage("Ellipse.bmp"), data));
    dllData->addTool(new CopyDC          (&DllSettings, "Скопировать область",  TheApp->loadManager->loadImage("CopyDCButton.bmp"), data));


    return dllData;
}






bool Vignette::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    colorSave = (ColorSave*) lay->getToolsData();
    appData = data;
    Vector pos = data->mousePos;
    COLORREF newColor = app->getPixel(pos, lay->lay->getPermanentDC());

    colorSave->color = *(app->currColor);

    if (data->clickedMB  && !workedLastTime)
    {
        workedLastTime = true;
        colorSave->isStarted = true;
        app->setDrawColor(newColor);
        colorSave->isFinished = true;
        return colorSave->isFinished;
    }
    if (!data->clickedMB) workedLastTime = false;

    return false;
}

void RectangleTool::outputFunc(HDC outdc)
{
    ToolSave* rectDate = getToolData();

    Vector ellipsePos = rectDate->pos + (rectDate->size) / 2;
    Vector ellipseSize = (rectDate->size) / 2;
    app->setColor(rectDate->color, outdc, rectDate->thickness);
    app->rectangle({ .pos = rectDate->pos, .finishPos = rectDate->pos + rectDate->size }, outdc);
}




void RectangleTool::countToolZone()
{
    ToolSave* rectDate = (ToolSave*)toolLay->getToolsData();
    toolLay->toolZone = { .pos = rectDate->pos, .finishPos = rectDate->pos + rectDate->size };
}


void EllipseTool::outputFunc(HDC outdc)
{
    ToolSave* rectDate = getToolData();

    Vector ellipsePos = rectDate->pos + (rectDate->size) / 2;
    Vector ellipseSize = (rectDate->size) / 2;
    app->setColor(rectDate->color, outdc, rectDate->thickness);
    app->ellipse(ellipsePos, ellipseSize, outdc);
}


void Point::initPointSave()
{
    pointSave->size = appData->size;
    pointSave->color = appData->color;
    pointSave->dllSettings = dllSettings;
    pointSave->pointsPosition = new Vector[dllSettings->POINTSAVELENGTH]{};
}

void Gummi::initPointSave()
{
    Vector size = { (double)dllSettings->GummiThickness, (double)dllSettings->GummiThickness };
    pointSave->size = size;
    pointSave->color = appData->backGroundColor;
    pointSave->dllSettings = dllSettings;
    pointSave->pointsPosition = new Vector[dllSettings->POINTSAVELENGTH]{};
}

bool Point::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    assert(data && lay && output); 
    appData = data;
    toolLay = lay;
    toolData = (ToolData*)lay->getToolsData();
    pointSave = (PointSave*)lay->getToolsData();
    

    if (data->clickedMB == 1)
    {
        lay->needRedraw();

        if (!isStarted(lay)) initPointSave();

        pointSave->isStarted = true;
        if (lastPos != data->mousePos)
        {
            pointSave->addPoint(data->mousePos);
        }
        lastPos = data->mousePos;
    }

    if ((!data->clickedMB) && isStarted(lay))
    {
        pointSave->isFinished = true;
        toolLay->needRedraw();
    }


    return pointSave->isFinished;
}

HDC Point::load(ToolLay* toollay, HDC dc)
{
    assert(toollay);
    toolLay = toollay;

    pointSave = (PointSave*)toollay->getToolsData();
    HDC outDC = dc;
    if (!outDC) outDC = getOutDC();

    app->setColor(pointSave->color, outDC, 1);
    if (app->systemSettings->debugMode == 5) printf("pointSave->currentLength: %d", pointSave->currentLength);

    for (int i = 0; i < pointSave->currentLength; i++)
    {
        app->ellipse(pointSave->pointsPosition[i], pointSave->size, outDC);
    }

    return outDC;
}


void Line::outputFunc(HDC outdc)
{ 
    ToolSave* toolDate = getToolData();
    app->line({ .pos = toolDate->pos, .finishPos = toolDate->pos + toolDate->size }, outdc);
}


bool Tool4Squares::use(ProgrammeDate* data, ToolLay* lay, void* output)
{
    assert(data && lay && output);
    appData = data;
    toolLay = lay;
    saveTool = (ToolSave*)output;
    Vector pos = appData->getMousePos();
    if (app->systemSettings->debugMode == 5) printf("pos: {%lf, %lf}\n", pos.x, pos.y);

    if (data->clickedMB == 1)
    {
        saveTool->pos = pos;
        workedLastTime = true;
        saveTool->isStarted = true;
        saveTool->isFinished = false;
    }

    if (isStarted(toolLay))
    {
        if (appData->getMousePos() != saveTool->size + saveTool->pos) lay->needRedraw();
        saveTool->size = pos - saveTool->pos;
        saveTool->color = data->color;
        saveTool->thickness = app->systemSettings->Thickness;
        saveTool->name = (const char*)1;
    }


    if (data->clickedMB == 2 && isStarted(toolLay))
    {
        finishUse();

        setControlSquares();
        countDeltaForControlButtons();
        countToolZone();
        saveTool->isFinished = true;

        lay->needRedraw();

        
        return true;
    }

    return false;
}

HDC Tool4Squares::load(ToolLay* toollay, HDC dc /* = NULL*/)
{
    assert(toollay);

    toolLay = toollay;

    ToolSave* toolDate = (ToolSave*)toollay->getToolsData();

    HDC outDC = dc;
    if (!dc)
    {
        outDC = getOutDC();
    }

    

    app->setColor(toolDate->color, outDC, toolDate->thickness);
    outputFunc(outDC);

    return outDC;
}  

