#pragma once
#include "DrawBibliothek.h"
struct CLay
{
    HDC dc;
    RGBQUAD* buf;
    ToolLay* toolLays[ONELAYTOOLSLIMIT] = {};
}