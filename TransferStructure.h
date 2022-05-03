#pragma once
#include "TXLib.h"
#include "DrawBibliothek.h"

struct TransferData
{
    HWND MAINWINDOW = NULL;
    HDC MainWindowDC = NULL;
};


struct CFilter : Manager
{
    CFilter (Rect _rect,  int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}) :
        Manager (_rect, _length, _advancedMode, _dc, _handle)
    {
    }
    
    virtual void apply () = 0; 
};

struct AbstractAppData
{
    virtual void setColor (COLORREF color, HDC dc) = 0;
    virtual void rectangle (Rect rect, HDC dc) = 0;
    virtual void drawOnScreen (HDC dc) = 0;

    Manager *canvasManager = NULL;


};

struct DLLExportData
{ 
    Window* (*createContrastWindow) (Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager) = NULL;    
    CFilter* (*createKontrastFilter) (Rect rect, Vector firstDomain, Vector secondDomain) = 0;
};









/*
struct DLLData
{
     Window* (*createContrastMenu) (TransferData data, Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager) = NULL;
}; */                                                                                                                                                                                                                