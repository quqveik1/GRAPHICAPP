#pragma once
#include "Menu.h"


void Menu::draw()
{
    if (dc) app->bitBlt(finalDC, 0, 0, 0, 0, dc);

    controlHandle();

    onUpdate();

    drawOnFinalDC(handle);

    for (int i = 0; i < currentSize; i++)
    {
        drawOneLine(i);
    }
}

void Menu::onClick(Vector mp)
{
    setActiveWindow(this);

    int missClicked = true;
    clickHandle();

    mp.y -= handle.rect.finishPos.y;

    int mx = mp.x;
    int my = mp.y;




    if (advancedMode)
    {
        reDraw = true;
        missClicked = onClickLine(mp);
    }
}
