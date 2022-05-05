#pragma once
#include "TXLib.h"
#include "DrawBibliothek.h"
#include "DLLFilters\DLLExportData.cpp"
#include "CFilter.h"

struct TransferData
{
    HWND MAINWINDOW = NULL;
    HDC MainWindowDC = NULL;
};




struct AbstractAppData
{
    virtual void setColor (COLORREF color, HDC dc) = 0;
    virtual void rectangle (Rect rect, HDC dc) = 0;
    virtual void drawOnScreen (HDC dc) = 0;

    Manager *canvasManager = NULL;


};















/*
struct DLLData
{
     Window* (*createContrastMenu) (TransferData data, Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager) = NULL;
}; */                                                                                                                                                                                                                