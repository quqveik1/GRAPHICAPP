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
    CLay* lay = NULL;
    Rect toolZone = {};
    Vector size = { 1, 1 };//rotates between[0; unlim)
    const char* name = NULL;
    Tool* tool = NULL;
    void* toolsData = NULL;
    int thickness = 1;
    

    //condition block
    bool isToolFinished = false;
    bool isEditing = false;


    virtual void needRedraw();

    bool useTool(ProgrammeDate* data);
    void drawTool(HDC dc = NULL);
    void editTool(ProgrammeDate* data);
    bool isInToolZone(ProgrammeDate* data, Vector mp, int mbCondition);




    virtual Tool* getTool();
};