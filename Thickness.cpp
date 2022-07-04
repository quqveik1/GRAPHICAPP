#pragma once
#include "Thickness.h"
void ThicknessMenu::draw()
{
    app->setColor(color, finalDC);
    app->rectangle(rect - rect.pos, finalDC);
    standartManagerDraw(this);

    handle.print(finalDC);
    app->setColor(app->systemSettings->BackgroundColor, finalDC);
    app->line(0, handle.rect.finishPos.y, getSize().x, handle.rect.finishPos.y, finalDC);


}

void ThicknessMenu::onClick(Vector mp)
{
    standartManagerOnClick(this, mp);
}