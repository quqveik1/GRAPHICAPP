#pragma once
#include "DrawBibliothek.h"

struct AddCanvasButton : Window
{
    CanvasManager* canvasManager;
    AddCanvasButton(AbstractAppData* _app, HDC _dc, CanvasManager* _canvasManager) :
        Window(_app, {}, NULL, _dc),
        canvasManager(_canvasManager)
    {
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};