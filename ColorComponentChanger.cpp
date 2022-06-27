#pragma once
#include "ColorComponentChanger.h"

void ColorComponentChanger::draw()
{
    app->setColor(color, finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);

    slider->draw();
    app->transparentBlt(finalDC, slider->rect.pos.x, slider->rect.pos.y, 0, 0, slider->finalDC);

    char output[MAX_PATH] = {};

    int result = sprintf(output, "%d", *component);

    app->selectFont(app->systemSettings->FONTNAME, 24, finalDC);
    app->setColor(app->systemSettings->TextColor, finalDC);
    app->drawText(slider->rect.finishPos.x, 0, getSize().x, getSize().y, output, finalDC, DT_VCENTER | DT_CENTER);


}

void ColorComponentChanger::onClick(Vector mp)
{
    if (slider->rect.inRect(mp)) slider->onClick(mp - slider->rect.pos);
}