#pragma once
#include "AddCanvasButton.h"


void AddCanvasButton::onClick(Vector mp)
{
    if (!isClickedLastTime()) canvasManager->addCanvas();
    setMbLastTime();
}