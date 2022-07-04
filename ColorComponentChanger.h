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
    int minLimit = 0;
    int maxLimit = 255;

    ColorComponentChanger(AbstractAppData* _app, Rect _rect, int* _component, bool* _confirmColor) :
        Manager(_app, _rect, 2, true, NULL, {}, _app->systemSettings->TRANSPARENTCOLOR),
        component(_component),
        numSize({ 50, getSize().y })
    {
        needTransparencyOutput = true;
        sliderRect = rect - rect.pos;
        sliderRect.finishPos.x -= numSize.x;

        slider = new ColorSlider(app, sliderRect, component, _confirmColor);
        addWindow(slider);

        Rect inputButtonRect = { .pos = {sliderRect.finishPos.x + 5, 0}, .finishPos = getSize() };

        inputButton = new InputButton(app, inputButtonRect, component, &minLimit, &maxLimit, color, RGB(144, 144, 144), RGB (200, 200, 200), _confirmColor);
        addWindow(inputButton);
    }
    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};