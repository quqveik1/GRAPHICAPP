#pragma once
#include "GlobalOptions.h"



struct Lay
{
    CSystemSettings* systemSettings;

    HDC lay = NULL;
    Vector layCoordinats = {};
    Vector laySize = {};
    RGBQUAD* layBuf = {};
    HDC outputLay = NULL;
    RGBQUAD* outputBuf = {};
    bool isClicked = false;

    void createLay(CSystemSettings* _systemSettings, Vector size = {});
    int getDownUpCoordinats(int x, int y);
    virtual void line(int x0, int y0, int x1, int y1, RGBQUAD* buf = NULL, COLORREF drawColor = NULL);
    virtual void circle(int x, int y, int r, COLORREF color = TX_BLACK);
    virtual void rectangle(int x0, int y0, int x1, int y1);
    virtual void clean(HDC dc = NULL);
};