//#include "RingsProject.cpp"
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"

#define txSetAllColors(color)  { txSetFillColor (color); txSetColor (color); }



struct Button
{
	Rect rect = {};
	void (*funcClicked) (const Button &button);
	COLORREF color;
};

		
COLORREF CurrColor = TX_RED;
const int ButtonsNum = 5;


void reactColor (const Button &button);
void cleanScreen (const Button &button);
void drawButton (Button Buttons[ButtonsNum]);
void checkClicked (Button Buttons[ButtonsNum]);
void drawMouse (const Button &button);


int main ()
{
	txCreateWindow (1000, 1000);

	
	Button Buttons[ButtonsNum] = {};	

	Buttons[0].rect = {.pos = {0, 0}, .finishPos = {50, 50}};
	Buttons[0].color = TX_RED;
	Buttons[0].funcClicked = reactColor;

	Buttons[1].rect = {.pos = {100, 0}, .finishPos = {150, 50}};  
	Buttons[1].color = TX_BLUE;
	Buttons[1].funcClicked = reactColor;

	Buttons[2].rect = {.pos = {200, 0}, .finishPos = {250, 50}};
	Buttons[2].color = TX_GREEN;
	Buttons[2].funcClicked = reactColor;

	Buttons[3].rect = {.pos = {300, 0}, .finishPos = {350, 50}};
	Buttons[3].color = TX_WHITE;
	Buttons[3].funcClicked = cleanScreen;

	Buttons[3].rect = {.pos = {300, 0}, .finishPos = {350, 50}};
	Buttons[3].color = TX_WHITE;
	Buttons[3].funcClicked = cleanScreen;

	Buttons[4].rect = {.pos = {0, 50}, .finishPos = {1000, 1000}};
	Buttons[4].color = TX_BLACK;
	Buttons[4].funcClicked = drawMouse;

	for (;;)
	{
		drawButton (Buttons);
		checkClicked (Buttons);
		//drawMouse ();
	}

}


void drawMouse (const Button &button)
{
	txSetAllColors (CurrColor);
	txCircle (txMouseX (), txMouseY (), 2);
}


void checkClicked (Button Buttons[ButtonsNum])
{
	for (int i = 0; i < ButtonsNum; i ++)
	{
		if (inButtonClicked (Buttons[i].rect))
		{
			Buttons[i].funcClicked (Buttons[i]);
		}
	}
}

void drawButton (Button Buttons[ButtonsNum])
{
	$s
	for (int i = 0; i < 4; i ++)
	{
		txSetAllColors (Buttons[i].color)
		txRectangle (Buttons[i].rect.pos.x, Buttons[i].rect.pos.y, Buttons[i].rect.finishPos.x, Buttons[i].rect.finishPos.y);
	}
}

void reactColor (const Button &button)
{
	CurrColor = button.color;
}


void cleanScreen (const Button &button)
{
	txSetAllColors (TX_BLACK)
	txRectangle (0, 0, 1000, 1000);
}