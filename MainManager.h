#pragma once
#include "DrawBibliothek.h"

struct MainManager : Manager
{
    Vector mousePos = {};
    int clicked = 0;
    Window* activeWindow = 0;


    MainManager(AbstractAppData* _app, Rect _rect, int _length) :
        Manager(_app, _rect, _length)
    {
    }

    virtual void draw() override;
    virtual void onClick(Vector mp) override;

    virtual int getMBCondition() override { return clicked; };

    virtual Vector getMousePos() override { return mousePos; }
    
    virtual Window* getActiveWindow() override { return activeWindow; };

    virtual void setActiveWindow(Window* window) override { activeWindow = window; };
};