#pragma once
#include "AppApi.h"
#include "DrawBibliothek.cpp"

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

void PowerPoint::drawCadre(Rect rect, HDC dc, COLORREF color, int thickness)
{
    if (systemSettings->debugMode == 5) printf("Rect: {%lf, %lf}\n", rect.pos.x, rect.pos.y);
    setColor(color, dc, thickness);
    
    int halfThickness = thickness / 2;
    line(rect.pos.x + halfThickness, rect.pos.y + halfThickness - 1, rect.pos.x + halfThickness, rect.finishPos.y - halfThickness, dc);
    line(rect.pos.x + halfThickness, rect.finishPos.y - halfThickness, rect.finishPos.x - halfThickness, rect.finishPos.y - halfThickness, dc);
    line(rect.finishPos.x - halfThickness, rect.pos.y + halfThickness, rect.finishPos.x - halfThickness, rect.finishPos.y - halfThickness, dc);
    line(rect.pos.x + halfThickness, rect.pos.y + halfThickness, rect.finishPos.x - halfThickness, rect.pos.y + halfThickness, dc);
}

void PowerPoint::drawCadre(Vector pos1, Vector pos2, HDC dc, COLORREF color, int thickness)
{
    Rect rect = { .pos = pos1, .finishPos = pos2 };

    drawCadre(rect, dc, color, thickness);
}

void PowerPoint::drawCadre(int x1, int y1, int x2, int y2, HDC dc, COLORREF color, int thickness)
{
    Vector pos1 = { (double)x1, (double)y1 };
    Vector pos2 = { (double)x2, (double)y2 };

    drawCadre(pos1, pos2, dc, color, thickness);
}

Vector PowerPoint::getCentrizedPos(Vector localSize, Vector globalSize)
{
    return (globalSize - localSize) * 0.5;
}


void PowerPoint::drawText(double x0, double y0, double x1, double y1, const char text[], HDC dc,
    unsigned format /*= DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS*/)
{
    txDrawText(x0, y0, x1, y1, text, format, dc);
}

void PowerPoint::drawText(Rect rect, const char text[], HDC dc,
    unsigned format/* = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_WORD_ELLIPSIS*/)
{
    drawText(rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, text, dc, format);
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
    txSetAllColors$(color, dc, thickness);
}

int PowerPoint::getColorComponent(COLORREF color, COLORREF component)
{
    return txExtractColor(color, component);
}

void PowerPoint::setDrawColor(COLORREF color)
{
    systemSettings->DrawColor = color;
}

COLORREF PowerPoint::getPixel(Vector pos, HDC dc)
{
    return txGetPixel(pos.x, pos.y, dc);
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

void PowerPoint::horizontalReflect(HDC dc, RGBQUAD* buf, Vector size, Vector fullDCSize/* = {}*/)
{
    if (fullDCSize == fullDCSize.getNullVector()) fullDCSize = size;
    double posy = fullDCSize.y - size.y;

    RGBQUAD* tempBuf = {};
    HDC tempDC = createDIBSection(size, &tempBuf);


    for (int x = 0; x < size.x; x++)
    {
        for (int y = posy; y < fullDCSize.y; y++)
        {
            int tempY = y - posy;
            int newY = size.y - tempY - 1;
            tempBuf[x + (int)(newY * size.x)] = buf[x + (int)(y * fullDCSize.x)];
        }
    }
    bitBlt(dc, {}, size, tempDC);

    deleteDC(tempDC);
}

void PowerPoint::verticalReflect(HDC dc, RGBQUAD* buf, Vector size, Vector fullDCSize/* = {}*/)
{
    if (fullDCSize == fullDCSize.getNullVector()) fullDCSize = size;
    double posy = fullDCSize.y - size.y;
   
    RGBQUAD* tempBuf = {};
    HDC tempDC = createDIBSection(size, &tempBuf);
    


    for (int x = 0; x < size.x; x++)
    {
        for (int y = posy; y < fullDCSize.y; y++)
        {
            int newX = size.x - x - 1;
            int tempY = y - posy;
            tempBuf[newX + (int)(tempY * size.x)] = buf[x + (int)(y * fullDCSize.x)];
        }
    }
    bitBlt(dc, {}, size, tempDC);

    deleteDC(tempDC);

}


void PowerPoint::bitBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2, int xSource/* = 0*/, int ySource/* = 0*/)
{
    txBitBlt(dc1, x0, y0, sizex, sizey, dc2, xSource, ySource);
}

void PowerPoint::bitBlt(HDC dc1, Vector pos, Vector size, HDC dc2, Vector posSource/* = {}*/)
{
    bitBlt(dc1, pos.x, pos.y, size.x, size.y, dc2, posSource.x, posSource.y);
}

void PowerPoint::transparentBlt(HDC dc1, int x0, int y0, int sizex, int sizey, HDC dc2, int xSource/* = 0*/, int ySource/* = 0*/)
{
    txTransparentBlt(dc1, x0, y0, sizex, sizey, dc2, xSource, ySource, systemSettings->TRANSPARENTCOLOR);
}

void PowerPoint::transparentBlt(HDC dc1, Vector pos, Vector size, HDC dc2, Vector posSource/* = {}*/)
{
    transparentBlt(dc1, pos.x, pos.y, size.x, size.y, dc2, posSource.x, posSource.y);
}

int PowerPoint::stretchBlt(HDC dest, int destPosx, int destPosy, int destSizex, int destSizey, HDC source, int sourcePosx, int sourcePosy, int sourceSizex, int sourceSizey)
{
    return StretchBlt(dest, destPosx, destPosy, destSizex, destSizey, source, sourcePosx, sourcePosy, sourceSizex, sourceSizey, SRCCOPY);
}

int PowerPoint::stretchBlt(HDC dest, Vector destPos, Vector destSize, HDC source, Vector sourcePos, Vector sourceSize)
{
    return stretchBlt(dest, destPos.x, destPos.y, destSize.x, destSize.y, source, sourcePos.x, sourcePos.y, sourceSize.x, sourceSize.y);
}


void PowerPoint::compressImage(HDC& newDC, Vector newSize, HDC oldDC, Vector oldSize)
{   
    assert(oldDC);

    if (!newDC) deleteDC (newDC);
    newDC = createDIBSection(newSize.x, newSize.y);
    assert(newDC);

    assert (stretchBlt(newDC, 0, 0, newSize.x, newSize.y, oldDC, 0, 0, oldSize.x, oldSize.y));
}

void PowerPoint::drawOnScreen(HDC dc, bool useAlpha /*=false*/)
{
    if (!useAlpha)txBitBlt(0, 0, dc);
    if (useAlpha)transparentBlt(txDC(), 0, 0, 0, 0, dc);
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

int PowerPoint::smartDeleteDC(HDC dc)
{
    return DeleteDC(dc);
}

int PowerPoint::saveImage(HDC dc, const char* path)
{
    return txSaveImage(path, dc);
}

void PowerPoint::changeWindow(Vector size/* = {}*/, Vector pos/* = {}*/)
{
    bool wasSizeChanged = true;
    if (pos == pos.getNullVector())
    {
        pos = { .x = abs((size.x - systemSettings->FullSizeOfScreen.x) / 2), .y = abs( (size.y - systemSettings->FullSizeOfScreen.y) / 2) };
    }
    if (size == size.getNullVector())
    {
        size = systemSettings->SizeOfScreen;
        wasSizeChanged = false;
    }

    
    systemSettings->ScreenPos = pos;

    MoveWindow(systemSettings->MAINWINDOW, pos.x, pos.y, size.x, size.y, TRUE);

    SetWindowLong(systemSettings->MAINWINDOW, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(0,0,0)));

    if (sizeHistory[1] != size)
    {
        HDC outDC = txCreateDIBSection(size.x, size.y);
        deleteDC(txDC());
        txDC() = outDC;
        sizeHistory[0] = sizeHistory[1];
        sizeHistory[1] = size;
        systemSettings->lastTimeSizeOfScreen = sizeHistory[0];
    }

    

    systemSettings->SizeOfScreen = size;
    if (wasSizeChanged) isResized = true;
}


void PowerPoint::setCursor(HCURSOR cursor)
{
    activeCursor = cursor;
    lastTimeCursorSetTime = clock();
}

Vector PowerPoint::getCursorPos()
{
    Vector vector = {};
    POINT point = {};
    GetCursorPos(&point);
    vector.x = point.x;
    vector.y = point.y;
    return vector;
}


bool PowerPoint::isFullScreen()
{
    assert(systemSettings);

    return systemSettings->SizeOfScreen == systemSettings->FullSizeOfScreen;

}

Rect PowerPoint::getUserRect()
{
    if (isFullScreen())
    {
        return { .pos = {0, systemSettings->HANDLEHEIGHT}, .finishPos = systemSettings->SizeOfScreen };
    }
    else
    {
        return { .pos = {systemSettings->FrameThickness, systemSettings->HANDLEHEIGHT}, .finishPos = systemSettings->SizeOfScreen - systemSettings->FrameThickness };
    }
}


void PowerPoint::setResized(bool state/* = true*/)
{
    isResized = state;
}


void PowerPoint::controlApp()
{
    if (clock() - lastTimeCursorSetTime > 60)
    {
        setCursor(defaultCursor);
    }
    txClearConsole();
}
