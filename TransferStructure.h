#pragma once
#include "TXLib.h"
#include "DrawBibliothek.h"

struct TransferData
{
    HWND MAINWINDOW = NULL;
    HDC MainWindowDC = NULL;
};

struct DLLExportData
{ 
    Window* (*createContrastWindow) (Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager) = NULL;    
};


struct CFilter
{

    virtual void apply (RGBQUAD **mainScreen, Vector size); 
};


struct AbstractDLLFilter
{
    

    virtual void apply (HDC dc, Vector size) = 0;
};

struct AbstractAppData
{
    virtual void setColor (COLORREF color, HDC dc) = 0;
    virtual void rectangle (Rect rect, HDC dc) = 0;
    virtual void drawOnScreen (HDC dc) = 0;


};



/*
struct DLLData
{
     Window* (*createContrastMenu) (TransferData data, Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager) = NULL;
}; */                                                                                                                                                                                                                