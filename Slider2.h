#pragma once
#include "DrawBibliothek.h"

struct Slider2 : Manager
{
    double doubleVersionOfParameter = NULL;
    int* parametr = NULL;
    int* minParameter = NULL;
    int* maxParameter = NULL;
    HDC    pointSlider = NULL; 
    Vector pointSliderPos = {};
    Vector pointSliderSize = {10, 9};
    bool isSliderClicked = false;
    double kOfParametr = 0;

    bool* confirmed = NULL;

    Slider2(AbstractAppData* _app, Rect _rect, int* _parametr, int* _minParameter, int* _maxParameter, bool* _confirmed = NULL) :
        Manager(_app, _rect, 1, true, NULL, {}, RGB (144, 144, 144)),
        parametr (_parametr),
        pointSlider (app->loadManager->loadImage("SliderPointer.bmp")),
        confirmed (_confirmed),
        minParameter (_minParameter),
        maxParameter (_maxParameter)
    {
        needTransparencyOutput = true;

        kOfParametr = (*maxParameter - *minParameter) / (getSize().x - pointSliderSize.x);
        pointSliderPos.x = *parametr / kOfParametr;

        doubleVersionOfParameter = *parametr;
    };

    Rect getPointSliderRect();
    void confirm();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};
