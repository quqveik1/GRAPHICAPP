#pragma once


struct CFilter : Manager
{
    int guid = 0;
    const char* name = NULL;

    CFilter(Rect _rect, int _length, bool _advancedMode = true, HDC _dc = NULL, Rect _handle = {}) :
        Manager(_rect, _length, _advancedMode, _dc, _handle)
    {
    }

    virtual void apply() = 0;
};