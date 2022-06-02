#pragma once
#include "DrawBibliothek.h"
struct CToolManager
{
    const int ToolsLength = 10;
    struct Tool** tools = new Tool * [ToolsLength];
    int currentLength = 0;

    virtual void addTool(Tool* tool);
};


struct ToolLay
{
    CLay* lay;
    Vector startPos;
    Vector size = { 1, 1 };//rotates between[0; unlim)
    const char* name;
    Tool* tool;
    void* toolsData = NULL;
    int thickness = 1;
    

    //condition block
    bool isToolFinished = false;


    virtual void needRedraw();
};