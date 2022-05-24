#pragma once
#include "TXLib.h"
#include "DrawBibliothek.h"

struct TransferData
{
    HWND MAINWINDOW = NULL;
    HDC MainWindowDC = NULL;
};


struct AbstractAppData
{
    virtual void setColor (COLORREF color, HDC dc, int thickness) = 0;
    virtual void rectangle (Rect rect, HDC dc) = 0;
    virtual void drawOnScreen (HDC dc, bool useAlpha = false) = 0;
    virtual void cleanTransparentDC() = 0;
    virtual bool getAsyncKeyState(int symbol) = 0;
    virtual void deleteTransparency(RGBQUAD* buf, unsigned int totalSize) = 0;
    void (*invert) (RGBQUAD* buf, unsigned int totalSize) = 0;

    struct Manager* canvasManager = NULL;
    COLORREF* currColor = NULL;

    Lay transparentLay;

};















/*
struct DLLData
{
     Window* (*createContrastMenu) (TransferData data, Rect rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue), Manager *canvasManager) = NULL;
}; */                                                                                                                                                                                                                