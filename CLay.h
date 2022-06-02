#pragma once
#include "DrawBibliothek.h"
struct CLay
{
    Lay lay = {};
    int lastToolNum = -1;
    bool needToRedraw = false;
    struct ToolLay* toolLays[ONELAYTOOLSLIMIT] = {};


    virtual void createLay(Vector _size = DCVECTORSIZE);
    virtual void addTool (ToolLay* tool);

    virtual HDC getDCForToolLoad ();
    virtual HDC getPermanentDC();

    virtual ToolLay* getActiveLay();

    virtual void needRedraw();
    virtual void noMoreRedraw();
    virtual bool redrawStatus();
    virtual void redraw();
};