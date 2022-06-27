#pragma once
#include "Q_Vector.h"

struct Rect
{
    Vector pos;
    Vector finishPos;
    Vector size;

    bool inRect (double x, double y);
    bool inRect (Vector vector);

    Rect& operator = (const Rect &a1);
    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->finishPos.x; }
    double bottom() const { return this->finishPos.y; }
    Vector getSize () const {return this->finishPos - this->pos; }
    void   countFinishPos() { this->finishPos = this->pos + this->size; }
    bool isValid ();
    
    void draw (HDC dc = txDC());
};

inline Rect operator + (const Rect &rect, const Vector &vector);

bool Rect::isValid ()
{
    if (getSize ().x > 0 && getSize ().y > 0) return true;
    return false;
}

Rect& Rect::operator = (const Rect &a1)
{
    pos = a1.pos;
    finishPos = a1.finishPos;
    size = a1.size;

    return *this;
};

void Rect::draw (HDC dc /*= txDC()*/)
{
    txRectangle (this->pos.x, this->pos.y, this->finishPos.x, this->finishPos.y, dc);
}

bool Rect::inRect (double x, double y)
{
    if (x > this->pos.x && y > this->pos.y)
    {
        if (x < this->finishPos.x && y < this->finishPos.y)
        {
            return true;
        }
    }   
    return false;
}

bool Rect::inRect (Vector vector)
{
    if (isBigger (vector.x, pos.x) ^ isBigger(vector.x, finishPos.x))
    {
        if (isBigger(vector.y, pos.y) ^ isBigger(vector.y, finishPos.y))
        {
            return true;
        }
    }

    /*
    if (vector.x > this->pos.x && vector.y > this->pos.y)
    {
        if (vector.x < this->finishPos.x && vector.y < this->finishPos.y)
        {
            return true;
        }
    } */  
    return false;
}


inline Rect operator + (const Rect& rect, const Vector& vector)
{
    Rect result = rect;
    result.pos += vector;
    result.finishPos += vector;

    return result;
}