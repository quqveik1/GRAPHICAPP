#pragma once


struct Rect
{
    Vector pos;
    Vector size;
    Vector finishPos;

    bool inRect (double x, double y);

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->finishPos.x; }
    double bottom() const { return this->finishPos.y; }
    Vector getSize () const {return this->finishPos - this->pos; }
};

bool Rect::inRect (double x, double y)
{
    if (x > pos.x && y > pos.y)
    {
        if (x < finishPos.x && y < finishPos.y)
        {
            return true;
        }
    }   
    return false;
}