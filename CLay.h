#pragma once
#include "DrawBibliothek.h"
struct CLay
{
    Lay lay = {};
    int toolLength = 0;
    int activeToolNum = -1;
    bool needToRedraw = false;
    struct ToolLay* toolLays[ONELAYTOOLSLIMIT] = {};


    virtual void createLay(Vector _size = DCVECTORSIZE);
    virtual void addToolLay (ToolLay* tool);

    virtual HDC getOutputDC ();
    virtual RGBQUAD* getOutputBuf();
    virtual HDC getPermanentDC();
    virtual RGBQUAD* getPermanentBuf();


    virtual ToolLay* getActiveToolLay();
    virtual int getCurrentSize();
    virtual int getActiveToolLayNum();
    virtual void setActiveToolLayNum(int num);
    virtual ToolLay** getToolLays();
    Lay* getLay();
    virtual Vector getLaySize();

    virtual void needRedraw();
    virtual void noMoreRedraw();
    virtual bool redrawStatus();
    virtual void redraw();
    virtual void editTool(ProgrammeDate* data);
};