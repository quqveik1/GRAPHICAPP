#pragma once
#include "ColorSlider.cpp"
#include "InputButton.cpp"

struct ColorComponentChanger : Manager
{
    int* component = NULL;
    ColorSlider* slider = NULL;
    InputButton* inputButton = NULL;
    Rect sliderRect = {};
    Vector numSize = {};

    ColorComponentChanger(AbstractAppData* _app, Rect _rect, int* _component) :
        Manager(_app, _rect, 2, true, NULL, {}, _app->systemSettings->TRANSPARENTCOLOR),
        component(_component),
        numSize({ 100, getSize().y })
    {
        sliderRect = rect - rect.pos;
        sliderRect.finishPos.x -= numSize.x;

        slider = new ColorSlider(app, sliderRect, component);
        addWindow(slider);

        Rect inputButtonRect = { .pos = {sliderRect.finishPos.x, 0}, .finishPos = getSize() };

        inputButton = new InputButton(app, inputButtonRect, component, color, TX_BLUE);
        addWindow(inputButton);
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};