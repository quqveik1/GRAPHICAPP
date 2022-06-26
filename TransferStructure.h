#pragma once
#include "TXLib.cpp"
#include "DrawBibliothek.h"
#include "LoadManager.h"


/*
struct TransferData
{
    HWND MAINWINDOW = NULL;
    HDC MainWindowDC = NULL;
};
*/

struct AbstractAppData
{
    CSystemSettings* systemSettings = NULL;

    virtual void setColor (COLORREF color, HDC dc, int thickness = 1) = 0;

    virtual void rectangle (Rect rect, HDC dc) = 0;
    virtual void rectangle(int x1, int y1, int x2, int y2, HDC dc) = 0;
    virtual void rectangle(Vector pos1, Vector pos2, HDC dc) = 0;

    virtual void drawOnScreen (HDC dc, bool useAlpha = false) = 0;
    virtual void cleanTransparentDC() = 0;
    virtual bool getAsyncKeyState(int symbol) = 0;
    virtual void deleteTransparency(RGBQUAD* buf, unsigned int totalSize) = 0;

    virtual void line(Rect rect, HDC dc) = 0;
    virtual void line(int x1, int y1, int x2, int y2, HDC dc) = 0;
    virtual void line(Vector pos1, Vector pos2, HDC dc) = 0;

    virtual void ellipse(Vector centrPos, Vector halfSize, HDC dc) = 0;
    virtual void ellipse(int x1, int y1, int x2, int y2, HDC dc) = 0;

    virtual void transparentBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2) = 0;
    virtual void bitBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2) = 0;

    virtual HDC createDIBSection(Vector size, RGBQUAD** pixels = NULL) = 0;
    virtual HDC createDIBSection(int sizex, int sizey, RGBQUAD** pixels = NULL) = 0;

    virtual void drawText(double x0, double y0, double x1, double y1, const char text[], HDC dc,
        unsigned format = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS) = 0;

    virtual void setAlign(unsigned align, HDC dc) = 0;
    virtual void selectFont(const char* text, int sizey, HDC& dc, int sizex = -1) = 0;

    virtual void deleteDC(HDC dc) = 0;
    virtual int saveImage(HDC dc, const char* path) = 0;

    virtual void drawCadre(Rect rect, HDC dc) = 0;

    virtual void changeWindow(Vector size = {}, Vector pos = {}) = 0;

    virtual bool wasResized() = 0;
    virtual bool isFullScreen() = 0;
    virtual void setResized(bool state = true) = 0;

    void* canvasManager = NULL;
    COLORREF* currColor = NULL;
    CLoadManager* loadManager = NULL;
    struct DLLManager* toolManager = NULL;
    bool isResized = false;
}; 