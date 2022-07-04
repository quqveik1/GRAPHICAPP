#pragma once
#include "TwoOptionsMenu.cpp"

struct KontrastMenu : TwoOptionsMenu
{
    KontrastMenu (AbstractAppData* _app, Rect _rect, Vector _firstDomain, Vector _secondDomain) :
        TwoOptionsMenu (_app, _rect, _firstDomain, _secondDomain)
    {
        name = "Фильтр контрастности";
    }


    virtual void useAlgorithm();
    virtual void apply ();
};