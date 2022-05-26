#pragma once



struct Lay
{
    HDC lay = {};
    Vector layCoordinats = {};
    Vector laySize = {};
    RGBQUAD* layBuf = {};
    HDC tempLay = {};
    RGBQUAD* tempBuf = {};
    HDC outputLay = NULL;
    RGBQUAD* outputBuf = {};
    bool isClicked = false;

    void createLay(Vector size = { DCMAXSIZE, DCMAXSIZE });
    int getDownUpCoordinats(int x, int y);
    virtual void line(int x0, int y0, int x1, int y1, RGBQUAD* buf = NULL, COLORREF drawColor = DrawColor);
    virtual void circle(int x, int y, int r, COLORREF color = TX_BLACK);
    virtual void rectangle(int x0, int y0, int x1, int y1);
    virtual void clean(HDC dc = NULL);
};