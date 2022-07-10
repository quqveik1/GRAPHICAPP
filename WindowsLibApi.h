#pragma once

#include "Q_Vector.h"
#include "AbstractApp.h"
#include "WindowsLib.h"
struct Window;
struct Manager;


struct WindowsLibApi
{
    virtual void resize(Window* window, Rect newRect);
    virtual bool addWindow(Manager* manager, Window* window);
    virtual int clickHandle(Manager* manager);
    virtual Vector getAbsCoordinats(Window* window, bool coordinatsWithHandle = false);
    virtual struct Window* isActiveWindowBelow(Manager* manager);
    virtual void controlHandle(Manager* manager);
    virtual void screenChanged(Manager* manager);

    virtual int standartWindowDraw(struct Window* window);
    virtual int standartManagerDraw(struct Manager* manager);
    virtual int standartManagerOnClick(struct Manager* manager, Vector mp);
};
