#pragma once

#include "DrawBibliothek.h"
          

struct ProgressBar : Window
{
    double* totalNum = NULL;
    double* currentNum = NULL;

    ProgressBar(Rect _rect, COLORREF _color = TX_LIGHTGREEN) :
        Window(_rect, _color)
    {}

    void setProgress(double* total, double* current);

    virtual void draw() override;
};