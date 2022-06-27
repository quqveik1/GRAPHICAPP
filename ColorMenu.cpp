#pragma once
#include "ColorMenu.h"

void ColorMenu::draw()
{
    standartManagerDraw(this);

    app->systemSettings->DrawColor = RGB(redComponent, greenComponent, blueComponent);
}