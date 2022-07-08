#pragma once
#include "Q_Vector.h"
struct ProgrammeDate
{
    Vector absMouseCoordinats;
    Vector mousePos;
    Vector managerPos;
    Vector canvasCoordinats = {};
    COLORREF color;
    COLORREF backGroundColor;
    int thickness;
    int gummiThickness;
    int clickedMB;
    Vector size;

    ProgrammeDate(Vector _absMouseCoordinats, Vector _managerPos, Vector _size, COLORREF _color) :
        absMouseCoordinats(_absMouseCoordinats),
        managerPos(_managerPos),
        size(_size),
        color(_color)
    {}

    virtual Vector getMousePos() { return mousePos; };

};