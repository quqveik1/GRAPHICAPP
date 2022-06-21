#pragma once
#include "TwoOptionsMenu.h"
struct BrightnessMenu : TwoOptionsMenu
{
    BrightnessMenu (AbstractAppData* _app, Rect _rect, Vector _firstDomain, Vector _secondDomain, AbstractAppData *_data = NULL) :
        TwoOptionsMenu (_app, _rect, _firstDomain, _secondDomain)
    {
        name = "‘ильтр €ркости";
    }

    virtual void useAlgorithm();
    virtual void apply ();
};