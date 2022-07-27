#pragma once
#include "..\CopyDC.cpp"


struct ImportDC : CopyDC
{
    bool wasClickedLastTime = false;

    ImportDC(�DllSettings* _dllSettings, const char* _name, HDC _iconDC, AbstractAppData* _app) :
        CopyDC (_dllSettings, _name, _iconDC, _app)
    {
    }

    virtual long use(ToolLay* lay) override;

};