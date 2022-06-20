#pragma once
#include "TwoOptionsMenu.cpp"

struct KontrastMenu : TwoOptionsMenu
{
    KontrastMenu (CSystemSettings* _systemSettings, Rect _rect, Vector _firstDomain, Vector _secondDomain, AbstractAppData *_data = NULL) :
        TwoOptionsMenu (_systemSettings, _rect, _firstDomain, _secondDomain, _data)
    {
        name = "Фильтр контрастности";
    }


    virtual void useAlgorithm();
    virtual void apply ();
};