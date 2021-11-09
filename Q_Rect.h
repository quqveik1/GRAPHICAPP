#pragma once


struct Rect
{
    Vector pos;
    Vector finishPos;
    Vector size;

    bool inRect (double x, double y);

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->finishPos.x; }
    double bottom() const { return this->finishPos.y; }
    Vector getSize () const {return this->finishPos - this->pos; }
    
    void draw ();
};

void Rect::draw ()
{
    txRectangle (this->pos.x, this->pos.y, this->finishPos.x, this->finishPos.y);
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