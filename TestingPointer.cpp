#include "TXLib.h"
#include "Q_Vector.h"
#include "TXWave.h"
#include "Q_Buttons.h" 

struct Rect
{
    Vector pos;
    Vector size;
    Vector finishPos;

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->finishPos.x; }
    double bottom() const { return this->finishPos.y; }
};


struct Button
{
	Rect rect = {};
	void (*funcClicked) ();
};

struct colorButton : Button
{
	COLORREF color;
};


Button button1 = {};
Button button2 = {};

colorButton button3 = {};
colorButton button4 = {};


void changeButton ()
{
	printf ("1\n");
}


void changeColorButton ()
{
	printf ("Color\n");
}

int main ()
{

}


