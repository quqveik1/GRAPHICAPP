#pragma once
#include "DrawBibliothek.h"


struct Handle : Manager
{
    int addToBackElemetsPos[10] = {};
    int numberOfAddToBackElements = 0;
    bool wasCommonHandlePlaceClicked = false;
    Vector lastTimeMousePos = {};

    Handle(AbstractAppData* _app, Rect _rect) :
        Manager(_app, _rect, 10, true, NULL, {}, _app->systemSettings->MenuColor)
    {}

    virtual bool addWindowToStart(Window* window);
    virtual bool addWindowToBack(Window* window);

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};