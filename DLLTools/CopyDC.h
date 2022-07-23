#pragma once
#include "..\Tool.h"
#include "..\DrawBibliothek.h"  
#include "CadreResizingTool.cpp"

struct SaveCopyDC : ToolZoneSave
{
    Vector startSize = {};
    HDC dc = NULL;
    RGBQUAD* buf = NULL;
    HDC stretchedDC = NULL;
    RGBQUAD* stretchedBuf = NULL;
    Vector stretchedDCSize = {};
    Vector stretchedDCFullSize = {};//размер созданного холста
    bool selectedZone = false;
    bool zoneCopied = false;
};


struct CopyDC : CadreResizingTool
{
    COLORREF frameColor = TX_BLACK;
    
    CopyDC(СDllSettings* _dllSettings, const char* _name, HDC _iconDC, AbstractAppData* _app) :
        CadreResizingTool(_dllSettings, _name, sizeof(SaveCopyDC), _iconDC, _app)
    {  
    }

    virtual void countToolZone() override;
    void sizeSignControl(Vector* pos, Vector* size);
    void controlStretchedDCFullSize(Vector absSize);

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output) override;
    virtual HDC load(ToolLay* toollay, HDC dc = NULL) override;
    virtual int destroy(ToolLay* toollay) override;
};