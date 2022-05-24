﻿// dllmain.cpp : Defines the entry point for the DLL application.
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
    DLLToolExportData* dllData = new DLLToolExportData(4);

    dllData->addTool(new Line    ((const char*)(1), sizeof(ToolSave),                                       LoadManager.loadImage("Line.bmp"), data));
    dllData->addTool(new Point   ((const char*)(2), sizeof(PointSave) + sizeof(ToolSave) * POINTSAVELENGTH, LoadManager.loadImage("Pen.bmp"), data));
    dllData->addTool(new Vignette((const char*)(3), sizeof(ColorSave),                                      LoadManager.loadImage("Vignette.bmp"), data));
    dllData->addTool(new Gummi   ((const char*)(4), sizeof(ToolSave),                                       LoadManager.loadImage("Gummi.bmp"), data));

    return dllData;


}




bool Vignette::use(ProgrammeDate* data, Lay* lay, void* output)
{
    Vector pos = data->mousePos + data->canvasCoordinats;
    firstUse(data, output, pos);
    *(app->currColor) = txGetPixel(pos.x, pos.y, lay->lay);

    ColorSave colorsave(*(app->currColor));
    *(ColorSave*)output = colorsave;

    finishUse();

    return true;
}

void Vignette::load(void* input, HDC finalDC)
{
    assert(input && finalDC);
    ColorSave* toolDate = (ColorSave*)input;
    *(app->currColor) = toolDate->color;
}

bool Gummi::use(ProgrammeDate* data, Lay* lay, void* output)
{
    assert(data && lay && output && lay->outputLay);
    Vector pos = data->mousePos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }

    app->setColor(data->backGroundColor, lay->lay, data->size.x);
    txEllipse(pos.x - data->size.x, pos.y - data->size.y, pos.x + data->size.x, pos.y + data->size.y, lay->lay);



    //printf ("1");

    if (clicked != 1)
    {
        workedLastTime = false;

        ToolSave saveTool(startPos + data->canvasCoordinats, { (double)data->gummiThickness, (double)data->gummiThickness }, data->backGroundColor, data->size.x, (const char*)2);

        *(ToolSave*)output = saveTool;

        return true;
    }

    return false;
}

bool RectangleTool::use(ProgrammeDate* data, Lay* lay, void* output)
{
    assert(data && lay && output && lay->outputLay);
    Vector pos = data->mousePos;
    if (!workedLastTime)
    {
        workedLastTime = true;
        startPos = pos;
    }

    app->setColor(data->color, lay->outputLay, data->size.x);
    lay->rectangle(startPos.x, startPos.y, pos.x, pos.y);

    if (clicked == 2)
    {
        app->setColor(data->color, lay->lay, data->size.x);
        lay->rectangle(startPos.x, startPos.y, pos.x, pos.y);

        ToolSave saveTool(startPos + data->canvasCoordinats, pos - startPos, data->color, data->size.x, (const char*)2);
        (*(ToolSave*)output) = saveTool;
        workedLastTime = false;
        return true;
    }

    return false;
}

void RectangleTool::load(void* input, HDC finalDC)
{
    assert(input && finalDC);
    ToolSave* rectDate = (ToolSave*)input;
    app->setColor(rectDate->color, finalDC, rectDate->thickness);


    txRectangle(rectDate->pos.x, rectDate->pos.y, rectDate->pos.x + rectDate->size.x, rectDate->pos.y + rectDate->size.y, finalDC);
}

bool Point::use(ProgrammeDate* data, Lay* lay, void* output)
{
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

    lastPos = pos;

    return false;
}

void Point::load(void* input, HDC finalDC)
{
    assert(input && finalDC);
    PointSave* pointsDate = (PointSave*)input;
    if (pointsDate->currentLength > 0) app->setColor(pointsDate->points[1].color, finalDC, pointsDate->points[1].thickness);

    for (int i = 0; i < pointsDate->currentLength; i++)
    {
        txEllipse(pointsDate->points[i].pos.x - pointsDate->points[i].size.x, pointsDate->points[i].pos.y - pointsDate->points[i].size.y, pointsDate->points[i].pos.x + pointsDate->points[i].size.x, pointsDate->points[i].pos.y + pointsDate->points[i].size.y, finalDC);
    }

}


bool Line::use(ProgrammeDate* data, Lay* lay, void* output)
{
    assert(data && lay && output && lay->outputLay);
    Vector pos = data->mousePos;
    if (!workedLastTime || clicked == 1)
    {
        startPos = pos;
        //printf("StartPos %d||", (int)startPos.x);
        workedLastTime = true;
    }
    app->setColor(data->color, lay->outputLay, data->size.x);
    txLine(startPos.x, startPos.y, pos.x, pos.y, lay->outputLay);

    if (clicked == 2)
    {
        app->setColor(data->color, lay->lay, data->size.x);

        txTransparentBlt (lay->lay, 0, 0, lay->laySize.x, lay->laySize.y, lay->outputLay, 0, 0, TRANSPARENTCOLOR);

        ToolSave saveTool(startPos + data->canvasCoordinats, pos - startPos, data->color, data->size.x, (const char*)1);
        *(ToolSave*)output = saveTool;

        finishUse();
        return true;

    }

    return false;
}

void Line::load(void* input, HDC finalDC)
{
    assert(input && finalDC);
    ToolSave* toolDate = (ToolSave*)input;
    app->setColor(toolDate->color, finalDC, toolDate->thickness);
    txLine(toolDate->pos.x, toolDate->pos.y, toolDate->pos.x + toolDate->size.x, toolDate->pos.y + toolDate->size.y, finalDC);

}




