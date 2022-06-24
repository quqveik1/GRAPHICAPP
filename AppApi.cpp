#pragma once
#include "AppApi.h"

HDC PowerPoint::createDIBSection(Vector size, RGBQUAD** pixels/* = NULL*/)
{
    return txCreateDIBSection(size.x, size.y, pixels);
}

HDC PowerPoint::createDIBSection(int sizex, int sizey, RGBQUAD** pixels/* = NULL*/)
{
    return txCreateDIBSection(sizex, sizey, pixels);
}


void PowerPoint::rectangle(int x1, int y1, int x2, int y2, HDC dc)
{
    txRectangle(x1, y1, x2, y2, dc);
}


void PowerPoint::rectangle(Vector pos1, Vector pos2, HDC dc)
{
    txRectangle(pos1.x, pos1.y, pos2.x, pos2.y, dc);
}

void PowerPoint::rectangle(Rect rect, HDC dc)
{
    txRectangle(rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}

void PowerPoint::drawCadre(Rect rect, HDC dc)
{
    if (systemSettings->debugMode == 5) printf("Rect: {%lf, %lf}\n", rect.pos.x, rect.pos.y);
    line(rect.pos.x, rect.pos.y, rect.pos.x, rect.finishPos.y, dc);
    line(rect.pos.x, rect.finishPos.y, rect.finishPos.x, rect.finishPos.y, dc);
    line(rect.finishPos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
    line(rect.pos.x, rect.pos.y, rect.finishPos.x, rect.pos.y, dc);
}

void PowerPoint::drawText(double x0, double y0, double x1, double y1, const char text[], HDC dc,
    unsigned format /*= DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS*/)
{
    txDrawText(x0, y0, x1, y1, text, format, dc);
}


void PowerPoint::setAlign(unsigned align, HDC dc)
{
    txSetTextAlign(align, dc);
}

void PowerPoint::selectFont(const char* text, int sizey, HDC& dc, int sizex/* = -1*/)
{
    txSelectFont(text, sizey, sizex, FW_DONTCARE, false, false, false, 0, dc);
}


void PowerPoint::setColor(COLORREF color, HDC dc, int thickness)
{
    if (systemSettings->debugMode == 5) printf("SetColor: %d|", color);
    txSetAllColors(color, dc, thickness);
}

void PowerPoint::bitBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2)
{
    txBitBlt(dc1, x0, y0, sizex, sizey, dc2);
}


void PowerPoint::line(Rect rect, HDC dc)
{
    txLine(rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}


void PowerPoint::line(int x1, int y1, int x2, int y2, HDC dc)
{
    txLine(x1, y1, x2, y2, dc);
}

void PowerPoint::line(Vector pos1, Vector pos2, HDC dc)
{
    txLine(pos1.x, pos1.y, pos2.x, pos2.y, dc);
}


void PowerPoint::ellipse(Vector centrePos, Vector halfSize, HDC dc)
{
    txEllipse(centrePos.x - halfSize.x, centrePos.y - halfSize.y, centrePos.x + halfSize.x, centrePos.y + halfSize.y, dc);
}

void PowerPoint::ellipse(int x1, int y1, int x2, int y2, HDC dc)
{
    txEllipse(x1, y1, x2, y2, dc);
}

void PowerPoint::transparentBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2)
{
    txTransparentBlt(dc1, x0, y0, sizex, sizey, dc2, 0, 0, systemSettings->TRANSPARENTCOLOR);
}

void PowerPoint::drawOnScreen(HDC dc, bool useAlpha /*=false*/)
{
    if (!useAlpha)txBitBlt(0, 0, dc);
    if (useAlpha)txAlphaBlend(0, 0, dc);
    txSleep(0);
}

void PowerPoint::cleanTransparentDC()
{
    //setColor(SystemSettings.TRANSPARENTCOLOR, transparentLay.lay);
   // rectangle(0, 0, transparentLay.laySize.x, transparentLay.laySize.y, transparentLay.lay);
    //txClear(transparentLay.lay);
    //deleteTransparency(transparentLay.layBuf, transparentLay.laySize.x * transparentLay.laySize.y);
}

bool PowerPoint::getAsyncKeyState(int symbol)
{
    return txGetAsyncKeyState(symbol);
}

void PowerPoint::deleteTransparency(RGBQUAD* buf, unsigned int totalSize)
{
    for (int i = 0; i < totalSize; i++)
    {
        buf[i].rgbReserved = 255;
    }
}


void PowerPoint::deleteDC(HDC dc)
{
    if (dc) txDeleteDC(dc);
    else printf("DC[%d] не существует\n", (int)dc);
}

int PowerPoint::saveImage(HDC dc, const char* path)
{
    return txSaveImage(path, dc);
}

void PowerPoint::changeWindow(Vector size, Vector pos/* = {}*/)
{
    
    if (pos == pos.getNullVector())
    {
        pos = { .x = abs((size.x - systemSettings->FullSizeOfScreen.x) / 2), .y = abs( (size.y - systemSettings->FullSizeOfScreen.y) / 2) };
    }

    MoveWindow(systemSettings->MAINWINDOW, pos.x, pos.y, size.x, size.y, TRUE);
    HDC outDC = txCreateDIBSection(size.x, size.y);
    txDC() = outDC;

    systemSettings->SizeOfScreen = size;
}