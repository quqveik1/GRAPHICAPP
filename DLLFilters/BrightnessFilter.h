#pragma once
#include "TwoOptionsMenu.h"
struct BrightnessMenu : TwoOptionsMenu
{
    BrightnessMenu (CSystemSettings* _systemSettings, Rect _rect, Vector _firstDomain, Vector _secondDomain, AbstractAppData *_data = NULL) :
        TwoOptionsMenu (_systemSettings, _rect, _firstDomain, _secondDomain, _data)
    {
        name = "‘ильтр €ркости";
    }

    virtual void useAlgorithm();
    virtual void apply ();
};