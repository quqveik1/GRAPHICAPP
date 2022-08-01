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


DLLToolExportData* initDLL(AbstractAppData* appData)
{
    TheApp = appData;
    DLLToolExportData* dllData = new DLLToolExportData(8);

    dllData->addTool(new Line            (&DllSettings, "Линия", sizeof(ToolSave),           TheApp->loadManager->loadImage("Line.bmp"), appData));
    dllData->addTool(new Point           (&DllSettings, "Точка", sizeof(PointSave),          TheApp->loadManager->loadImage("Pen.bmp"), appData));
    dllData->addTool(new Vignette        (&DllSettings, "Виньетка", sizeof(ColorSave), TheApp->loadManager->loadImage("Vignette.bmp"), appData));
    dllData->addTool(new Gummi           (&DllSettings, "Ластик", sizeof(ToolSave),  TheApp->loadManager->loadImage("Gummi.bmp"), appData));
    dllData->addTool(new RectangleTool   (&DllSettings, "Прямоугольник", sizeof(ToolSave),  TheApp->loadManager->loadImage("Rectangle.bmp"), appData));
    dllData->addTool(new EllipseTool     (&DllSettings, "Эллипс", sizeof(ToolSave),  TheApp->loadManager->loadImage("Ellipse.bmp"), appData));
    dllData->addTool(new CopyDC          (&DllSettings, "Скопировать область",  TheApp->loadManager->loadImage("CopyDCButton.bmp"), appData));
    dllData->addTool(new ImportDC        (&DllSettings, "Вставить изображение из буффера",  TheApp->loadManager->loadImage("ImportDC.bmp"), appData));


    return dllData;
}






long Vignette::use(ToolLay* lay)
{
    colorSave = (ColorSave*) lay->getToolsData();
    appData = lay->getProgDate();
    Vector pos = appData->mousePos;
    COLORREF newColor = app->getPixel(pos, lay->lay->getPermanentDC());

    colorSave->color = *(app->currColor);

    if (appData->clickedMB  && !workedLastTime)
    {
        workedLastTime = true;
        colorSave->isStarted = true;
        app->setDrawColor(newColor);
        colorSave->isFinished = true;
        return colorSave->isFinished;
    }
    if (!appData->clickedMB) workedLastTime = false;

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


void Gummi::setPointSaveData()
{
    pointSave->saveData(app->systemSettings->BackgroundColor, { (double)app->systemSettings->Thickness, (double)app->systemSettings->Thickness });
}

void Point::setPointSaveData()
{
    pointSave->saveData(app->systemSettings->DrawColor, { (double)app->systemSettings->Thickness, (double)app->systemSettings->Thickness });
}

long Point::use(ToolLay* lay)
{
    assert(lay); 
    appData = lay->getProgDate();
    assert(appData);
    toolLay = lay;
    toolData = (ToolData*)lay->getToolsData();
    pointSave = (PointSave*)lay->getToolsData();
    

    if (appData->clickedMB == 1)
    {
        lay->needRedraw();
        
        setPointSaveData();
        pointSave->isStarted = true;
        if (lastPos != appData->mousePos)
        {
            pointSave->addPoint(appData->mousePos);
        }
        lastPos = appData->mousePos;
    }

    if ((!appData->clickedMB) && isStarted(lay))
    {
        pointSave->isFinished = true;
        toolLay->needRedraw();
    }


    return pointSave->isFinished;
}

HDC Point::load(ToolLay* toollay)
{
    assert(toollay);
    toolLay = toollay;

    pointSave = (PointSave*)toollay->getToolsData();
    HDC outDC = getOutDC();

    app->setColor(pointSave->color, outDC, pointSave->size.x);

    Polyline(outDC, pointSave->pointsPosition, pointSave->currentLength);

    /*
    for (int i = 0; i < pointSave->currentLength; i++)
    {
        app->setColor(pointSave->color, outDC, pointSave->size.x);
        if (i > 0)
        {
            app->line(pointSave->pointsPosition[i - 1], pointSave->pointsPosition[i], outDC);
        }
    }
    */

    return outDC;
}


long Point::handler(TOOLMESSAGE message, ToolLay* lay)
{
    if (message == T_CREATE)
    {
        PointSave* _pointSave = new PointSave(dllSettings);
        lay->getToolsData() = (char*)_pointSave;
        return 0;
    }

    if (message == T_DESTROY)
    {
        PointSave* _pointSave = (PointSave*)lay->getToolsData();
        delete _pointSave;
        return 0;
    }

    return defaultHandler(message, lay);
}


void Line::outputFunc(HDC outdc)
{ 
    ToolSave* toolDate = getToolData();
    app->line({ .pos = toolDate->pos, .finishPos = toolDate->pos + toolDate->size }, outdc);
}


long Tool4Squares::use(ToolLay* lay)
{
    assert(lay);
    appData = lay->getProgDate();
    toolLay = lay;
    saveTool = (ToolSave*)lay->getToolsData();
    Vector pos = appData->getMousePos();
    if (app->systemSettings->debugMode == 5) printf("pos: {%lf, %lf}\n", pos.x, pos.y);

    if (appData->clickedMB == 1)
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
        saveTool->color = appData->color;
        saveTool->thickness = app->systemSettings->Thickness;
        saveTool->name = (const char*)1;
    }


    if (appData->clickedMB == 2 && isStarted(toolLay))
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

HDC Tool4Squares::load(ToolLay* toollay)
{
    assert(toollay);

    toolLay = toollay;

    ToolSave* toolDate = (ToolSave*)toollay->getToolsData();

    HDC outDC = getOutDC();

    app->setColor(toolDate->color, outDC, toolDate->thickness);
    outputFunc(outDC);

    return outDC;
}  

