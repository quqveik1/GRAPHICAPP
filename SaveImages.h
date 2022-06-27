#pragma once
#include "CanvasManager.h"

struct SaveImages : Window
{
    CanvasManager* canvasManager;

    SaveImages(AbstractAppData* _app, CanvasManager* _canvasManager) :
        Window(_app, {}, _app->systemSettings->MenuColor, NULL, NULL, "", false),
        canvasManager(_canvasManager)
    {
    }

    virtual void draw() override;
};