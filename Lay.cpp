#pragma once
#include "Lay.h"


void Lay::createLay(AbstractAppData* _app, Vector _laySize/* = {}*/)
{
    gassert(_app);
    app = _app;

    laySize = _laySize;
    Vector nullVector = { 0, 0 };

    if (laySize == nullVector) laySize = app->systemSettings->DCVECTORSIZE;
    lay = txCreateDIBSection(laySize.x, laySize.y, &layBuf);
    clean();
    outputLay = txCreateDIBSection(laySize.x, laySize.y, &outputBuf);
    clean(outputLay);
}


int Lay::getDownUpCoordinats(int x, int y)
{
    return (int)(x + (laySize.y - y) * laySize.x);
}

void Lay::clean(HDC dc/* = NULL*/)
{
    if (!dc) dc = lay;
    app->setColor(app->systemSettings->TRANSPARENTCOLOR, dc);
    app->rectangle(0, 0, laySize.x, laySize.y, dc);
}
