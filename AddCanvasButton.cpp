#pragma once
#include "AddCanvasButton.h"
#include "WindowsLibApi.h"


void AddCanvasButton::draw()
{
    app->windowsLibApi->standartWindowDraw(this);
    setMbLastTime();
}


void AddCanvasButton::onClick(Vector mp)
{
    if (!isClickedLastTime()) canvasManager->addCanvas("TestName", {500, 500});
    setMbLastTime();
}