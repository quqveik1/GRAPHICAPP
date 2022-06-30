#pragma once 
#include "ColorSlider.h"



Rect ColorSlider::getPointSliderRect()
{
    Rect tempRect = { .pos = pointSliderPos, .finishPos = pointSliderPos + pointSliderSize };
    return tempRect;
}


void ColorSlider::confirmColor()
{
    colorConfirmed = true;
}

void ColorSlider::draw()
{
    app->setColor(app->systemSettings->TRANSPARENTCOLOR, finalDC);
    app->rectangle(0, 0, getSize().x, getSize().y, finalDC);

    app->setColor(color, finalDC);
    app->rectangle(pointSliderSize.x / 2, pointSliderSize.y, getSize().x - pointSliderSize.x / 2, getSize().y, finalDC);

    app->transparentBlt(finalDC, pointSliderPos.x, 0, 0, 0, pointSlider);

    if (isSliderClicked)
    {
        if (!getMBCondition())
        {
            isSliderClicked = false;
            confirmColor();
        }

        pointSliderPos.x += getMousePos().x - mousePosLastTime.x;
        if (isSmaller(pointSliderPos.x, 0)) pointSliderPos.x = 0;
        if (isBigger(pointSliderPos.x, getSize().x - pointSliderSize.x)) pointSliderPos.x = getSize().x - pointSliderSize.x;
        *colorParametr = pointSliderPos.x * kOfParametr;
        setLastTimeMP();
    }

    pointSliderPos.x = *colorParametr / kOfParametr;

    setMbLastTime();

}


void ColorSlider::onClick(Vector mp)
{ 
    setActiveWindow(this);
    if (getPointSliderRect().inRect(mp) && !isClickedLastTime())
    {
        isSliderClicked = true;
        setLastTimeMP();
    }
}