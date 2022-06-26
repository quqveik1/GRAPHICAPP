#pragma once
#include "Lay.h"


void Lay::createLay(CSystemSettings* _systemSettings, Vector _laySize/* = {}*/)
{
    //qassert (manager, info);
    assert(_systemSettings);
    systemSettings = _systemSettings;

    laySize = _laySize;
    Vector nullVector = { 0, 0 };

    if (laySize == nullVector) laySize = systemSettings->DCVECTORSIZE;
    lay = txCreateDIBSection(laySize.x, laySize.y, &layBuf);
    clean();
    outputLay = txCreateDIBSection(laySize.x, laySize.y, &outputBuf);
    clean(outputLay);
}


int Lay::getDownUpCoordinats(int x, int y)
{
    return (int)(x + (laySize.y - y) * laySize.x);
}


void Lay::line(int x0, int y0, int x1, int y1, RGBQUAD* buf/*=NULL*/, COLORREF drawColor /*=DrawColor*/)
{
    if (x0 < 0) x0 = 1;
    if (x1 < 0) x1 = 1;
    if (y0 < 0) y0 = 1;
    if (y1 < 0) y1 = 1;


    if (buf == NULL) buf = layBuf;
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = abs(dy) * 2;
    int error2 = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            RGBQUAD* color = &buf[getDownUpCoordinats(y, x)];
            color->rgbRed = txExtractColor(drawColor, TX_RED);
            color->rgbGreen = txExtractColor(drawColor, TX_GREEN);
            color->rgbBlue = txExtractColor(drawColor, TX_BLUE);
            color->rgbReserved = 255;
        }
        else
        {
            RGBQUAD* color = &buf[getDownUpCoordinats(x, y)];
            color->rgbRed = txExtractColor(drawColor, TX_RED);
            color->rgbGreen = txExtractColor(drawColor, TX_GREEN);
            color->rgbBlue = txExtractColor(drawColor, TX_BLUE);
            color->rgbReserved = 255;
        }
        error2 += derror2;

        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);

            error2 -= dx * 2;
        }
    }
}

void Lay::clean(HDC dc/* = NULL*/)
{
    if (!dc) dc = lay;
    txSetAllColors(systemSettings->TRANSPARENTCOLOR, dc);
    txRectangle(0, 0, laySize.x, laySize.y, dc);
}


void Lay::rectangle(int x0, int y0, int x1, int y1)
{
    if (x0 > x1) swap(x0, x1);
    if (y0 > y1) swap(y0, y1);
    COLORREF currColor = txGetColor(lay);

    for (int x = x0; x <= x1; x++)
    {
        //line (x, y0, x, y1);

        for (int y = y0; y <= y1; y++)
        {
            RGBQUAD* pixel = &layBuf[getDownUpCoordinats(x, y)];
            pixel->rgbRed = txExtractColor(currColor, TX_RED);
            pixel->rgbGreen = txExtractColor(currColor, TX_GREEN);
            pixel->rgbBlue = txExtractColor(currColor, TX_BLUE);
            pixel->rgbReserved = 255;
        }
    }
    // txSetAllColors (TX_RED, lay);
     //txRectangle (x0, y0, x1, y1, lay);
     //printBlt (lay);
}


void Lay::circle(int x0, int y0, int r, COLORREF color)
{

    for (double t = 0; t <= 1; t += 0.001)
    {
        double x = x0 - r + t * 2 * (r);
        int y1 = sqrt(r * r - (x - x0) * (x - x0)) + y0;
        int y2 = -sqrt(r * r - (x - x0) * (x - x0)) + y0;

        //printf ("x: %lf y = {%lf, %lf}\n", x, y1, y2);
        //txSetAllColors (TX_RED, lay);
        //txSetColor (TX_RED, 2);
        //txLine (x, y1, x, y2, lay);
        //if (x >= 0 && y1 >= 0 && y2 >= 0)
        line(x, y1, x, y2, NULL, color);
    }
}