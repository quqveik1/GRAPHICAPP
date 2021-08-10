//#include "RingsProject.cpp"
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"

#define txSetAllColors(color)  { txSetFillColor (color); txSetColor (color); }




struct Window
{
	Rect rect = {};
	void (*funcClicked) (const Window &window);
	void (*funcDraw) (const Window &window);
	HDC canvas;
	COLORREF color;
	char text [20] = {};
};

		
COLORREF CurrColor = TX_RED;
const int WindowNum = 8;
bool IsRunning = true;
int Radius = 2;


void reactColor (const Window &window);
void cleanScreen (const Window &window);
void drawButton (Window window[WindowNum]);
void checkClicked (Window window[WindowNum]);
void drawMouse (const Window &window);
void endprogramm (const Window &wndow);
void minusRadius (const Window &window);
void plusRadius (const Window &window);
void drawCanvas (const Window &window);
void standartDraw (const Window &window);
void textDraw (const Window &window);

int main ()
{
	txCreateWindow (1000, 1000);

	
	Window Windows[WindowNum] = {};	


	Windows[0].rect = {.pos = {0, 50}, .finishPos = {1000, 1000}};
	Windows[0].color = TX_BLACK;
	Windows[0].funcClicked = drawMouse;
	Windows[0].funcDraw	= drawCanvas;
	Windows[0].canvas = txCreateCompatibleDC (Windows[0].rect.getSize().x, Windows[0].rect.getSize().y);

	Windows[1].rect = {.pos = {0, 0}, .finishPos = {50, 50}};
	Windows[1].color = TX_LIGHTRED;
	Windows[1].funcClicked = reactColor;
	Windows[1].funcDraw = standartDraw;

	Windows[2].rect = {.pos = {100, 0}, .finishPos = {150, 50}};  
	Windows[2].color = TX_LIGHTBLUE;
	Windows[2].funcClicked = reactColor;
	Windows[2].funcDraw = standartDraw;

	Windows[3].rect = {.pos = {200, 0}, .finishPos = {250, 50}};
	Windows[3].color = TX_LIGHTGREEN;
	Windows[3].funcClicked = reactColor;
	Windows[3].funcDraw = standartDraw;

	Windows[4].rect = {.pos = {300, 0}, .finishPos = {350, 50}};
	Windows[4].color = TX_WHITE;
	Windows[4].funcClicked = cleanScreen;
	Windows[4].funcDraw = standartDraw;

	Windows[5].rect = {.pos = {950, 0}, .finishPos = {1000, 50}};
	Windows[5].color = TX_RED;
	Windows[5].funcClicked = endprogramm;
	Windows[5].funcDraw = standartDraw;

	Windows[6].rect = {.pos = {500, 0}, .finishPos = {550, 50}};
	Windows[6].color = TX_BLUE;
	Windows[6].funcClicked = minusRadius;
	Windows[6].funcDraw = textDraw;
	sprintf (Windows[6].text, "-");
	
	Windows[7].rect = {.pos = {600, 0}, .finishPos = {650, 50}};
	Windows[7].color = TX_GREEN;
	Windows[7].funcClicked = plusRadius;
	Windows[7].funcDraw = textDraw;
	sprintf (Windows[7].text, "+");

	for (;;)
	{
		drawButton (Windows);
		checkClicked (Windows);
		//drawMouse ();
		if (!IsRunning) break;
	}

	txDisableAutoPause ();

}

void textDraw (const Window &window)
{
	$s
	txSetAllColors (window.color)
	txRectangle (window.rect.pos.x, window.rect.pos.y, window.rect.finishPos.x, window.rect.finishPos.y);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (window.rect.pos.x + (window.rect.finishPos.x - window.rect.pos.x) / 2, 
					window.rect.pos.y + (window.rect.finishPos.y - window.rect.pos.y) / 4, 
					window.text);
}

void standartDraw (const Window &window)
{
	$s
	txSetAllColors (window.color)
	txRectangle (window.rect.pos.x, window.rect.pos.y, window.rect.finishPos.x, window.rect.finishPos.y);	
}

void drawCanvas (const Window &window)
{
	txBitBlt (window.rect.pos.x, window.rect.pos.y, window.canvas);
}

void minusRadius (const Window &window)
{
	if (Radius >= 1)
	{
		Radius--;
	}
	while (txMouseButtons () == 1);
}

void plusRadius (const Window &window)
{
	Radius++;
	while (txMouseButtons () == 1);
}

void endprogramm (const Window &window)
{
	IsRunning = false;
}

void drawMouse (const Window &window)
{
	
	txSetAllColors (CurrColor);
	txRectangle (100, 100, 200, 200, window.canvas);
	//txCircle (txMouseX (), txMouseY (), Radius);
	static bool leftClicked = false;
	static Vector startPos = {};

	if (txMouseButtons () == 1 && !leftClicked)
	{
		startPos = {.x = (double) txMousePos().x, .y = (double) txMousePos ().y};
		leftClicked = true;
	}

	if (leftClicked)
	{
		txLine (startPos.x, startPos.y, txMouseX (), txMouseY ());	
	}

	int currMouseButton = txMouseButtons ();

	if (txMouseButtons () == 1 && leftClicked)
	{
		leftClicked = false;
		txLine (startPos.x, startPos.y, txMouseX (), txMouseY (), window.canvas);
	}

}

void checkClicked (Window Windows[WindowNum])
{
	for (int i = 0; i < WindowNum; i ++)
	{
		if (inButtonClicked (Windows[i].rect))
		{
			Windows[i].funcClicked (Windows[i]);
		}
	}
}

void drawButton (Window Windows[WindowNum])
{
	txSleep (30);
	$s
	for (int i = 0; i < WindowNum; i ++)
	{

		Windows[i].funcDraw (Windows[i]);
		/*
		txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (Windows[i].rect.pos.x + (Windows[i].rect.finishPos.x - Windows[i].rect.pos.x) / 2, 
					Windows[i].rect.pos.y + (Windows[i].rect.finishPos.y - Windows[i].rect.pos.y) / 4, 
					Windows[i].text);
					*/
	}
}

void reactColor (const Window &window)
{
	CurrColor = window.color;
}


void cleanScreen (const Window &window)
{
	txSetAllColors (TX_BLACK)
	txRectangle (0, 0, 1000, 1000);
}