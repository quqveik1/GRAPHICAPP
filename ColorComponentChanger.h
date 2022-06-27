#pragma once
#include "ColorSlider.cpp"

struct ColorComponentChanger : Manager
{
    int* component = NULL;
    ColorSlider* slider = NULL;
    Rect sliderRect = {};
    Vector numSize = {};

    ColorComponentChanger(AbstractAppData* _app, Rect _rect, int* _component) :
        Manager(_app, _rect, 1, true, NULL, {}, _app->systemSettings->TRANSPARENTCOLOR),
        component(_component),
        numSize({ 100, getSize().y })
    {
        sliderRect = rect - rect.pos;
        sliderRect.finishPos.x -= numSize.x;

        slider = new ColorSlider(app, sliderRect, component);
        addWindow(slider);
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};