
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"







struct Window
{
	Rect rect = {};

	void (Window::*funcClicked) ();
	void (Window::*funcDraw) ();

	void draw ();

	//void drawButtons (Window *pointers[PointersNum]);
	//void checkClicked (Window *pointers[PointersNum]);
	
	COLORREF color;
	char text [20] = {};
	bool isClicked = false;
};

struct Canvas : Window
{
	HDC canvas;
	int lineThickness = 3;
	COLORREF drawColor = TX_RED;
	bool clearBackground= false;

	Vector lastClick = {};

	void drawCanvas (const Window *window);
	void clicked (const Window *pointer);
};

struct TimeButton : Window
{
	void printTime ();
};

struct CloseButton : Window
{
	void endprogramm ();
};

struct ColorButton : Window
{
	Canvas *mainCanvas;

	void reactColor ();
};

struct SizeButton : Window
{
	Canvas *mainCanvas;

	void minusSize ();
	void plusSize ();
	void textDraw ();

};

struct CleanButton : Window
{
	Canvas *mainCanvas;
	void cleanScreen ();

};



//typedef void (Window::*funcClickedPtr) (const Window *);

		
//COLORREF CurrColor = TX_RED;
const int PointersNum = 9;
bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;







void RECTangle (const Rect rect, HDC dc = txDC ());
void txSetAllColors (COLORREF color, HDC dc = txDC ());
void drawButtons (Window *pointers[PointersNum]);
void checkClicked (Window *pointers[PointersNum]);

int main ()
{
	txCreateWindow (1000, 1000);

	
	//Window Windows[WindowNum] = {};
	Window* pointers[PointersNum] = {};

	Canvas mainCanvas = {};
	mainCanvas.rect = {.pos = {0, 50}, .finishPos = {1000, 1000}};
	mainCanvas.color = TX_BLACK;
	mainCanvas.funcClicked = (void (Window::*) ()) &Canvas::clicked;
	mainCanvas.funcDraw	= (void (Window::*) ()) &Canvas::drawCanvas;
	mainCanvas.canvas = txCreateCompatibleDC (mainCanvas.rect.getSize().x, mainCanvas.rect.getSize().y);

	pointers[0] = &mainCanvas;
	///Windows[0].canvas = txCreateCompatibleDC (Windows[0].rect.getSize().x, Windows[0].rect.getSize().y);

	ColorButton redColor = {};
	redColor.rect = {.pos = {0, 0}, .finishPos = {50, 50}};
	redColor.color = TX_LIGHTRED;
	redColor.mainCanvas = &mainCanvas;
	redColor.funcClicked = (void (Window::*) ()) &ColorButton::reactColor;
	redColor.funcDraw = (void (Window::*) ()) &ColorButton::draw;
	pointers[1] = &redColor;

	ColorButton blueColor = {};
	blueColor.rect = {.pos = {100, 0}, .finishPos = {150, 50}};  
	blueColor.color = TX_LIGHTBLUE;
	blueColor.mainCanvas = &mainCanvas;
	blueColor.funcClicked = (void (Window::*) ()) &ColorButton::reactColor;
	blueColor.funcDraw = (void (Window::*) ()) &ColorButton::draw;
	pointers[2] = &blueColor;

	ColorButton GreenColor = {};
	GreenColor.rect = {.pos = {200, 0}, .finishPos = {250, 50}};
	GreenColor.color = TX_LIGHTGREEN;
	GreenColor.mainCanvas = &mainCanvas;
	GreenColor.funcClicked = (void (Window::*) ()) &ColorButton::reactColor;
	GreenColor.funcDraw = (void (Window::*) ()) &ColorButton::draw;
	pointers[3] = &GreenColor;
	
	CleanButton cleanButton = {};
	cleanButton.rect = {.pos = {300, 0}, .finishPos = {350, 50}};
	cleanButton.color = TX_WHITE;
	cleanButton.mainCanvas = &mainCanvas;
	cleanButton.funcClicked = (void (Window::*) ()) &CleanButton::cleanScreen;
	cleanButton.funcDraw = (void (Window::*) ()) &CleanButton::draw;
	pointers[4] = &cleanButton;

	CloseButton closeButton = {};
	closeButton.rect = {.pos = {950, 0}, .finishPos = {1000, 50}};
	closeButton.color = TX_RED;
	closeButton.funcClicked = (void (Window::*) ()) &CloseButton::endprogramm;
	closeButton.funcDraw = (void (Window::*) ()) &CloseButton::draw;
	pointers[5] = &closeButton;

	SizeButton minSize = {};
	minSize.rect = {.pos = {500, 0}, .finishPos = {550, 50}};
	minSize.color = TX_BLUE;
	minSize.mainCanvas = &mainCanvas;
	minSize.funcClicked = (void (Window::*) ()) &SizeButton::minusSize;
	minSize.funcDraw    = (void (Window::*) ()) &SizeButton::textDraw;
	sprintf (minSize.text, "-");
	pointers[6] = &minSize;
	
	SizeButton plusRad = {};
	plusRad.rect = {.pos = {600, 0}, .finishPos = {650, 50}};
	plusRad.color = TX_GREEN;
	plusRad.mainCanvas = &mainCanvas;
	plusRad.funcClicked = (void (Window::*) ()) &SizeButton::plusSize;
	plusRad.funcDraw    = (void (Window::*) ()) &SizeButton::textDraw;
	sprintf (plusRad.text, "+");
	pointers[7] = &plusRad;

	TimeButton timeButton = {};
	timeButton.rect	= {.pos = {700, 0}, .finishPos = {850, 50}};
	timeButton.color = TX_GREEN;
	timeButton.funcDraw = (void (Window::*) ()) &TimeButton::printTime;
	pointers[8] = &timeButton;


	for (;;)
	{
		drawButtons (pointers);
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


void TimeButton::printTime ()
{
	txSetAllColors (TX_BLACK);
	RECTangle (rect);
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
	txTextOut (rect.pos.x, rect.pos.y, newStr);

}

void SizeButton::textDraw ()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 4, 
					text);
}

void Window::draw ()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);	
}

void Canvas::drawCanvas (const Window *window)
{
	Canvas* canvasPtr = (Canvas*) window;

	assert (canvasPtr->canvas);
	txBitBlt (window->rect.pos.x, window->rect.pos.y, canvasPtr->canvas);
	for(;;)
	{
		printf ("1");
	}

	if (txMouseButtons () == 2 && canvasPtr->isClicked)
	{
		txSetAllColors (canvasPtr->drawColor, canvasPtr->canvas);
		txLine (canvasPtr->lastClick.x, canvasPtr->lastClick.y - canvasPtr->rect.pos.y, txMouseX (), txMouseY () - canvasPtr->rect.pos.y, canvasPtr->canvas); 
		canvasPtr->isClicked = false;
	}

	if (canvasPtr->isClicked)
	{
		txSetAllColors (canvasPtr->drawColor);
		txLine (canvasPtr->lastClick.x, canvasPtr->lastClick.y, txMouseX (), txMouseY ());
	}

	if (canvasPtr->clearBackground)
	{
		txSetAllColors (TX_BLACK, canvasPtr->canvas);
		RECTangle (canvasPtr->rect, canvasPtr->canvas);
		canvasPtr->clearBackground = false;
	}
}

void SizeButton::minusSize ()
{
	if (!isClicked)
	{
		mainCanvas->lineThickness--;
	}
}

void SizeButton::plusSize ()
{
	if (!isClicked)
	{
		mainCanvas->lineThickness++;
	}
}

void CloseButton::endprogramm ()
{
	IsRunning = false;
}

void Canvas::clicked (const Window *window)
{
	
	

	Canvas* canvasPtr = (Canvas*) window;

	txSetAllColors (canvasPtr->drawColor);

	int currMouseButton = txMouseButtons ();
	
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
			pointers[i]->funcClicked;
			pointers[i]->isClicked = true;
		}
		else
		{
			pointers[i]->isClicked = false;
		}
	}
}

void drawButtons (Window *pointers[PointersNum])
{
	txSleep (SLEEPTIME);
	txSetAllColors (TX_BLACK);
	txClear ();
	$s
	for (int i = 0; i < PointersNum; i ++)
	{
		//void (Window::*ptrDraw)() = Window::*funcDraw;
		(pointers[i]->*(pointers[i]->funcDraw))();
		//printf ("1");
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

void ColorButton::reactColor ()
{
	mainCanvas->drawColor = color;
}


void CleanButton::cleanScreen ()
{
	mainCanvas->clearBackground = true;
}