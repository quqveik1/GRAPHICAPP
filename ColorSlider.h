#pragma once
#include "DrawBibliothek.h"

struct ColorSlider : Manager
{
    int* colorParametr = NULL;
    HDC    pointSlider = NULL; 
    Vector pointSliderPos = {};
    Vector pointSliderSize = {10, 9};
    bool isSliderClicked = false;
    double kOfParametr = 0;

    bool* confirmedColor = NULL;

    ColorSlider(AbstractAppData* _app, Rect _rect, int* _colorParametr, bool* _confirmedColor) :
        Manager(_app, _rect, 1, true, NULL, {}, RGB (144, 144, 144)),
        colorParametr (_colorParametr),
        pointSlider (app->loadManager->loadImage("SliderPointer.bmp")),
        confirmedColor (_confirmedColor)
    {
        needTransparencyOutput = true;

        kOfParametr = 255 / (getSize().x - pointSliderSize.x);
        pointSliderPos.x = *colorParametr / kOfParametr;
    };

    Rect getPointSliderRect();
    void confirmColor();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};
