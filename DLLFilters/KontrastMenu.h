#pragma once
#include "TwoOptionsMenu.cpp"

struct KontrastMenu : TwoOptionsMenu
{
    KontrastMenu (Rect _rect, Vector _firstDomain, Vector _secondDomain, AbstractAppData *_data = NULL) :
        TwoOptionsMenu (_rect, _firstDomain, _secondDomain, _data)
    {
        name = "Фильтр контрастности";
    }


   
    virtual void apply ();
};