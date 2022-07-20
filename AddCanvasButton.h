#pragma once
#include "SetCanvasButton.cpp"
#include "WindowsLib.cpp"

struct AddCanvasButton : Manager
{
    struct CanvasManager* canvasManager = NULL;
    SetCanvasButton setCanvasButton;
    const char* defaultCanvasName = "Холст";

    Vector newCanvasSize = { 500, 500 };
    bool isEnterActive = false;


    AddCanvasButton(AbstractAppData* _app, HDC _dc, struct CanvasManager* _canvasManager) :
        Manager(_app, {}, 1, true, _dc),
        canvasManager(_canvasManager),
        setCanvasButton(_app, {300, 300}, &newCanvasSize)
    {
        //addWindow(&setCanvasButton);
    }

    virtual SetCanvasButton* getSetCanvasButton();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
};