#pragma once

#include "Q_Rect.h"


struct AbstractAppData
{
    const char* appVersion = NULL;

    struct CSystemSettings* systemSettings = NULL;
    struct CToolManager* toolManager = NULL;
    struct CLoadManager* loadManager = NULL;
    struct CFileSavings* fileSavings = NULL;
    struct CanvasManager* canvasManager = NULL;
    struct WindowsLibApi* windowsLibApi = NULL;

    COLORREF* currColor = NULL;
    bool isResized = false;
    bool IsRunning = true;
    bool isShowing = true;;
    HCURSOR defaultCursor = LoadCursor(NULL, IDC_ARROW);

    virtual void setColor(COLORREF color, HDC dc, int thickness = 1) = 0;
    virtual int getColorComponent(COLORREF color, COLORREF component) = 0;
    virtual void setDrawColor(COLORREF color) = 0;
    virtual COLORREF getPixel(Vector pos, HDC dc) = 0;

    //+Function to draw figures
    virtual void rectangle(Rect rect, HDC dc) = 0;
    virtual void rectangle(int x1, int y1, int x2, int y2, HDC dc) = 0;
    virtual void rectangle(Vector pos1, Vector pos2, HDC dc) = 0;
    virtual void line(Rect rect, HDC dc) = 0;
    virtual void line(int x1, int y1, int x2, int y2, HDC dc) = 0;
    virtual void line(Vector pos1, Vector pos2, HDC dc) = 0;

    virtual void ellipse(Vector centrPos, Vector halfSize, HDC dc) = 0;
    virtual void ellipse(int x1, int y1, int x2, int y2, HDC dc) = 0;
    //-Function to draw figures

    virtual void drawOnScreen(HDC dc, bool useAlpha = false) = 0;
    virtual void cleanTransparentDC() = 0;
    virtual bool getAsyncKeyState(int symbol) = 0;
    virtual void deleteTransparency(RGBQUAD* buf, unsigned int totalSize) = 0;




    //+Functions with dc
    virtual void horizontalReflect(HDC dc, RGBQUAD* buf, Vector size, Vector fullDCSize = {}) = 0;
    virtual void verticalReflect(HDC dc, RGBQUAD* buf, Vector size, Vector fullDCSize = {}) = 0;

    virtual int stretchBlt(HDC dest, int destPosx, int destPosy, int destSizex, int destSizey, HDC source, int sourcePosx, int sourcePosy, int sourceSizex, int sourceSizey) = 0;
    virtual int stretchBlt(HDC dest, Vector destPos, Vector destSize, HDC source, Vector sourcePos, Vector sourceSize) = 0;
    virtual void transparentBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2, int xSource = 0, int ySource = 0) = 0;
    virtual void transparentBlt(HDC dc1, Vector pos, Vector size, HDC dc2, Vector posSource = {}) = 0;
    virtual void bitBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2, int xSource = 0, int ySource = 0) = 0;
    virtual void bitBlt(HDC dc1, Vector pos, Vector size, HDC dc2, Vector posSource = {}) = 0;

    virtual void compressImage(HDC& newDC, Vector newSize, HDC oldDC, Vector oldSize) = 0;

    virtual HDC createDIBSection(Vector size, RGBQUAD** pixels = NULL) = 0;
    virtual HDC createDIBSection(int sizex, int sizey, RGBQUAD** pixels = NULL) = 0;
    virtual void deleteDC(HDC dc) = 0;
    virtual int smartDeleteDC(HDC dc) = 0;
    //-Functions with dc



    virtual void drawText(double x0, double y0, double x1, double y1, const char text[], HDC dc,
        unsigned format = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS) = 0;
    virtual void drawText(Rect rect, const char text[], HDC dc,
        unsigned format = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS) = 0;

    virtual void setAlign(unsigned align, HDC dc) = 0;
    virtual void selectFont(const char* text, int sizey, HDC& dc, int sizex = -1) = 0;

    virtual int saveImage(HDC dc, const char* path) = 0;

    virtual void drawCadre(Rect rect, HDC dc, COLORREF color, int thickness) = 0;
    virtual void drawCadre(Vector pos1, Vector pos2, HDC dc, COLORREF color, int thickness) = 0;
    virtual void drawCadre(int x1, int y1, int x2, int y2, HDC dc, COLORREF color, int thickness) = 0;

    virtual Vector getCentrizedPos(Vector localSize, Vector globalSize) = 0;

    virtual void changeWindow(Vector size = {}, Vector pos = {}) = 0;
    virtual void setCursor(HCURSOR cursor) = 0;
    virtual Vector getCursorPos() = 0;

    virtual void setResized(bool state = true) = 0;
    virtual bool wasResized() = 0;
    virtual bool isFullScreen() = 0;

    virtual Rect getUserRect() = 0;
};