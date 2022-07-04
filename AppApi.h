#pragma once
#include "TransferStructure.h"

struct PowerPoint : AbstractAppData
{
    HCURSOR activeCursor = NULL;
    int lastTimeCursorSetTime = NULL;



    virtual void setColor(COLORREF color, HDC dc, int thickness = 1) override;
    virtual int getColorComponent(COLORREF color, COLORREF component) override;
    virtual void setDrawColor(COLORREF color) override;
    virtual COLORREF getPixel(Vector pos, HDC dc) override;

    virtual void rectangle(Rect rect, HDC dc) override;
    virtual void rectangle(int x1, int y1, int x2, int y2, HDC dc) override;
    virtual void rectangle(Vector pos1, Vector pos2, HDC dc) override;

    virtual void drawOnScreen(HDC dc, bool useAlpha = false) override;
    virtual void cleanTransparentDC() override;
    virtual bool getAsyncKeyState(int symbol) override;
    virtual void deleteTransparency(RGBQUAD* buf, unsigned int totalSize) override;

    virtual void line(Rect rect, HDC dc) override;
    virtual void line(int x1, int y1, int x2, int y2, HDC dc) override;
    virtual void line(Vector pos1, Vector pos2, HDC dc) override;

    virtual void ellipse(Vector centrPos, Vector halfSize, HDC dc) override;
    virtual void ellipse(int x1, int y1, int x2, int y2, HDC dc) override;

    virtual void transparentBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2) override;
    virtual void bitBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2) override;

    virtual HDC createDIBSection(Vector size, RGBQUAD** pixels = NULL) override;
    virtual HDC createDIBSection(int sizex, int sizey, RGBQUAD** pixels = NULL) override;

    virtual void drawText(double x0, double y0, double x1, double y1, const char text[], HDC dc,
        unsigned format = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS)  override;

    virtual void selectFont(const char* text, int sizey, HDC& dc, int sizex = -1) override;
    virtual void setAlign(unsigned align, HDC dc) override;

    virtual void deleteDC(HDC dc) override;
    virtual int smartDeleteDC(HDC dc) override;
    virtual int saveImage(HDC dc, const char* path) override;

    virtual void drawCadre(Rect rect, HDC dc, COLORREF color, int thickness) override;

    virtual void changeWindow(Vector size = {}, Vector pos = {})  override;
    virtual void setCursor(HCURSOR cursor) override;

    virtual bool wasResized() override { return isResized; };
    virtual void setResized(bool state = true) override;

    virtual bool isFullScreen() override;

    virtual void controlApp();

};
