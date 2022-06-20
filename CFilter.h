#pragma once
#include "DrawBibliothek.h"
#include "TransferStructure.h"



struct CFilter : Manager
{
    int guid = 0;
    const char* name = NULL;
    AbstractAppData* app = NULL;

    CFilter(CSystemSettings* _systemsettings, Rect _rect, int _length, AbstractAppData* _app, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}) :
        Manager(_systemsettings, _rect, _length, _advancedMode, _dc, _handle),
        app (_app)
    {
    }

    virtual void apply() = 0;
};