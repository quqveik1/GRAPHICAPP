#pragma once
#include "CopyDC.cpp"


struct ImportDC : CopyDC
{
    bool wasClickedLastTime = false;

    ImportDC(ÑDllSettings* _dllSettings, const char* _name, HDC _iconDC, AbstractAppData* _app) :
        CopyDC (_dllSettings, _name, _iconDC, _app)
    {
    }

    virtual bool use(ProgrammeDate* data, ToolLay* lay, void* output) override;

};