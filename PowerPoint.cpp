//#include "RingsProject.cpp"
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"







struct Window
{
	Rect rect = {};
	void (*funcClicked) (const Window *window);
	void (*funcDraw) (const Window *window);
	COLORREF color;
	char text [20] = {};
};

struct Canvas : Window
{
	HDC canvas;
	Vector lastClick = {};
	bool isClicked = false;
};

		
COLORREF CurrColor = TX_RED;
const int PointersNum = 9;
bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;
bool ClearBackgroung = false;


void reactColor (const Window *window);
void cleanScreen (const Window *window);
void canvasDrawLine (const Window *pointer);

void checkClicked (Window *pointers[PointersNum]);
void drawButton (Window *pointers[PointersNum]);

void endprogramm (const Window *window);
void minusRadius (const Window *window);
void plusRadius (const Window *window);
void drawCanvas (const Window *window);
void standartDraw (const Window *window);
void textDraw (const Window *window);
void printTime (const Window *window);
void RECTangle (const Rect rect, HDC dc = txDC ());
void txSetAllColors (COLORREF color, HDC dc = txDC ());

int main ()
{
	txCreateWindow (1000, 1000);

	
	//Window Windows[WindowNum] = {};
	Window* pointers[PointersNum] = {};

	Canvas mainCanvas = {};
	mainCanvas.rect = {.pos = {0, 50}, .finishPos = {1000, 1000}};
	mainCanvas.color = TX_BLACK;
	mainCanvas.funcClicked = canvasDrawLine;
	mainCanvas.funcDraw	= drawCanvas;
	mainCanvas.canvas = txCreateCompatibleDC (mainCanvas.rect.getSize().x, mainCanvas.rect.getSize().y);
	pointers[0] = &mainCanvas;
	///Windows[0].canvas = txCreateCompatibleDC (Windows[0].rect.getSize().x, Windows[0].rect.getSize().y);

	Window redColor = {};
	redColor.rect = {.pos = {0, 0}, .finishPos = {50, 50}};
	redColor.color = TX_LIGHTRED;
	redColor.funcClicked = reactColor;
	redColor.funcDraw = standartDraw;
	pointers[1] = &redColor;

	Window blueColor = {};
	blueColor.rect = {.pos = {100, 0}, .finishPos = {150, 50}};  
	blueColor.color = TX_LIGHTBLUE;
	blueColor.funcClicked = reactColor;
	blueColor.funcDraw = standartDraw;
	pointers[2] = &blueColor;

	Window GreenColor = {};
	GreenColor.rect = {.pos = {200, 0}, .finishPos = {250, 50}};
	GreenColor.color = TX_LIGHTGREEN;
	GreenColor.funcClicked = reactColor;
	GreenColor.funcDraw = standartDraw;
	pointers[3] = &GreenColor;
	
	Window cleanButton = {};
	cleanButton.rect = {.pos = {300, 0}, .finishPos = {350, 50}};
	cleanButton.color = TX_WHITE;
	cleanButton.funcClicked = cleanScreen;
	cleanButton.funcDraw = standartDraw;
	pointers[4] = &cleanButton;

	Window closeButton = {};
	closeButton.rect = {.pos = {950, 0}, .finishPos = {1000, 50}};
	closeButton.color = TX_RED;
	closeButton.funcClicked = endprogramm;
	closeButton.funcDraw = standartDraw;
	pointers[5] = &closeButton;

	Window minRadius = {};
	minRadius.rect = {.pos = {500, 0}, .finishPos = {550, 50}};
	minRadius.color = TX_BLUE;
	minRadius.funcClicked = minusRadius;
	minRadius.funcDraw = textDraw;
	sprintf (minRadius.text, "-");
	pointers[6] = &minRadius;
	
	Window plusRad = {};
	plusRad.rect = {.pos = {600, 0}, .finishPos = {650, 50}};
	plusRad.color = TX_GREEN;
	plusRad.funcClicked = plusRadius;
	plusRad.funcDraw = textDraw;
	sprintf (plusRad.text, "+");
	pointers[7] = &plusRad;

	Window timneButton = {};
	timneButton.rect	= {.pos = {700, 0}, .finishPos = {850, 50}};
	timneButton.color = TX_GREEN;
	timneButton.funcDraw = printTime;
	pointers[8] = &timneButton;


	for (;;)
	{
		drawButton (pointers);
		checkClicked (pointers);
		//drawMouse ();
		if (!IsRunning) break;
	}

	txDisableAutoPause ();

}

void txSetAllColors (COLORREF color, HDC dc /*= txDc ()*/)
{
	txSetFillColor (color, dc); 
	txSetColor (color, 1, dc);
}

void RECTangle (const Rect rect, HDC dc /* = txDc ()*/)
{
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}


void printTime (const Window *window)
{
	txSetAllColors (TX_BLACK);
	RECTangle (window->rect);
	time_t t = time (NULL);
	t = t % (24 * 3600);

	char newStr[50] = {};
	int hours =	t / 3600;
	int minutes = t / 60 - hours * 60;
	int second  = t - hours * 3600 - minutes * 60;

	//strftime (newStr, 50, "%X", localtime (time));

	sprintf (newStr, "%d:%02d:%02d", hours + 3, minutes, second);

	txSetTextAlign (TA_LEFT);
	txSetAllColors (TX_WHITE);
	txTextOut (window->rect.pos.x, window->rect.pos.y, newStr);

}

void textDraw (const Window *window)
{
	$s
	txSetAllColors (window->color);
	txRectangle (window->rect.pos.x, window->rect.pos.y, window->rect.finishPos.x, window->rect.finishPos.y);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (window->rect.pos.x + (window->rect.finishPos.x - window->rect.pos.x) / 2, 
					window->rect.pos.y + (window->rect.finishPos.y - window->rect.pos.y) / 4, 
					window->text);
}

void standartDraw (const Window *window)
{
	$s
	txSetAllColors (window->color);
	txRectangle (window->rect.pos.x, window->rect.pos.y, window->rect.finishPos.x, window->rect.finishPos.y);	
}

void drawCanvas (const Window *window)
{
	Canvas* canvasPtr = (Canvas*) window;
	txBitBlt (window->rect.pos.x, window->rect.pos.y, canvasPtr->canvas);

	if (txMouseButtons () == 2 && canvasPtr->isClicked)
	{
		txSetAllColors (CurrColor, canvasPtr->canvas);
		txLine (canvasPtr->lastClick.x, canvasPtr->lastClick.y - canvasPtr->rect.pos.y, txMouseX (), txMouseY () - canvasPtr->rect.pos.y, canvasPtr->canvas); 
		canvasPtr->isClicked = false;
	}

	if (canvasPtr->isClicked)
	{
		txSetAllColors (CurrColor);
		txLine (canvasPtr->lastClick.x, canvasPtr->lastClick.y, txMouseX (), txMouseY ());
	}

	if (ClearBackgroung)
	{
		txSetAllColors (TX_BLACK, canvasPtr->canvas);
		RECTangle (canvasPtr->rect, canvasPtr->canvas);
		ClearBackgroung = false;
	}
}

void minusRadius (const Window *window)
{
	if (Radius >= 1)
	{
		Radius--;
	}
	while (txMouseButtons () == 1);
}

void plusRadius (const Window *window)
{
	Radius++;
	while (txMouseButtons () == 1);
}

void endprogramm (const Window *window)
{
	IsRunning = false;
}

void canvasDrawLine (const Window *window)
{
	
	txSetAllColors (CurrColor);
	//txRectangle (100, 100, 200, 200, window->canvas);
	//txCircle (txMouseX (), txMouseY (), Radius);
	Canvas* canvasPtr = (Canvas*) window;

	int currMouseButton = txMouseButtons ();

	/*
	if (currMouseButton == 1 && leftClicked)
	{
		leftClicked = false;
		txLine (startPos.x, startPos.y, txMouseX (), txMouseY (), window->canvas);
	}

	if (txMouseButtons () == 1 && !leftClicked)
	{
		startPos = {.x = (double) txMousePos().x, .y = (double) txMousePos ().y};
		leftClicked = true;
		while (txMouseButtons () == 1){}
	}

	if (leftClicked)
	{
		txLine (startPos.x, startPos.y, txMouseX (), txMouseY ());	
	}  */

	/*
	if (txMouseButtons () == 1)
	{
		while (txMouseButtons () == 1){};

		startPos = {.x = (double) txMousePos().x, .y = (double) txMousePos ().y};

		for (;;)
		{
			drawCanvas (window);
			
			if (txMousePos().y > window->rect.pos.y)
			{
				txLine (startPos.x, startPos.y, txMousePos().x, txMousePos().y);
			}
			txSleep (SLEEPTIME);

			if (txMouseButtons () == 1)
			{
				txSetAllColors (CurrColor);
				txLine (startPos.x, startPos.y - window.rect.pos.y, txMouseX (), txMouseY () - 50, window.canvas);
				while (txMouseButtons () == 1){};
				break;
			}
		}
	}
	*/

	
	if (!(canvasPtr->isClicked))
	{
		canvasPtr->lastClick = {.x = txMouseX (), .y = txMouseY ()};
		canvasPtr->isClicked = true;
	}
}

void checkClicked (Window *pointers[PointersNum])
{
	for (int i = 0; i < PointersNum; i ++)
	{
		if (inButtonClicked (pointers[i]->rect))
		{
			pointers[i]->funcClicked (pointers[i]);
		}
	}
}

void drawButton (Window *pointers[PointersNum])
{
	txSleep (SLEEPTIME);
	txSetAllColors (TX_BLACK);
	txClear ();
	$s
	for (int i = 0; i < PointersNum; i ++)
	{

		pointers[i]->funcDraw (pointers[i]);
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

void reactColor (const Window *window)
{
	CurrColor = window->color;
}


void cleanScreen (const Window *window)
{
	ClearBackgroung = true;
}