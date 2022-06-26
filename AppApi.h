#pragma once
#include "TransferStructure.h"

struct PowerPoint : AbstractAppData
{
    virtual void setColor(COLORREF color, HDC dc, int thickness = 1);

    virtual void rectangle(Rect rect, HDC dc);
    virtual void rectangle(int x1, int y1, int x2, int y2, HDC dc);
    virtual void rectangle(Vector pos1, Vector pos2, HDC dc);

    virtual void drawOnScreen(HDC dc, bool useAlpha = false);
    virtual void cleanTransparentDC();
    virtual bool getAsyncKeyState(int symbol);
    virtual void deleteTransparency(RGBQUAD* buf, unsigned int totalSize);

    virtual void line(Rect rect, HDC dc);
    virtual void line(int x1, int y1, int x2, int y2, HDC dc);
    virtual void line(Vector pos1, Vector pos2, HDC dc);

    virtual void ellipse(Vector centrPos, Vector halfSize, HDC dc);
    virtual void ellipse(int x1, int y1, int x2, int y2, HDC dc);

    virtual void transparentBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2);
    virtual void bitBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2);

    virtual HDC createDIBSection(Vector size, RGBQUAD** pixels = NULL);
    virtual HDC createDIBSection(int sizex, int sizey, RGBQUAD** pixels = NULL);

    virtual void drawText(double x0, double y0, double x1, double y1, const char text[], HDC dc,
        unsigned format = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS);

    virtual void selectFont(const char* text, int sizey, HDC& dc, int sizex = -1);

    virtual void setAlign(unsigned align, HDC dc);
    virtual void deleteDC(HDC dc);
    virtual int saveImage(HDC dc, const char* path);

    virtual void drawCadre(Rect rect, HDC dc);

    virtual void changeWindow(Vector size = {}, Vector pos = {});

    virtual bool wasResized() { return isResized; };
    virtual void setResized(bool state = true);

    virtual bool isFullScreen();

};
