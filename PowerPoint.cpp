
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"







struct Window
{
	Rect rect = {};

	//void (Window::*funcClicked) ();
	//void (Window::*funcDraw) ();

	virtual void draw ();
	virtual void onClick () {};
	
	COLORREF color;
	char text [20] = {};
	bool isClicked = false;
};

struct Canvas : Window
{
	HDC canvas;
	int lineThickness = 3;
	COLORREF drawColor = TX_RED;
	bool clearBackground = false;

	bool wasClicked = false;
	Vector lastClick = {};

	virtual void draw () override;
	virtual void onClick () override;
};

struct TimeButton : Window
{
	virtual void draw () override;
};

struct CloseButton : Window
{
	virtual void onClick () override;
};

struct ColorButton : Window
{
	Canvas *mainCanvas;

	virtual void onClick ()override;
};

struct SizeButton : Window
{
	Canvas *mainCanvas;
	int sizeType;

	virtual void draw () override;
	virtual void onClick () override;

};

struct CleanButton : Window
{
	Canvas *mainCanvas;

	virtual void onClick () override;
};

struct ManagerButton : Window
{
	enum { pointersK = 3};
	Window *pointers[pointersK];
	bool advancedMode = false;

	virtual void draw () override;
	virtual void onClick () override;
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
	mainCanvas.canvas = txCreateCompatibleDC (mainCanvas.rect.getSize().x, mainCanvas.rect.getSize().y);

	pointers[0] = &mainCanvas;
	///Windows[0].canvas = txCreateCompatibleDC (Windows[0].rect.getSize().x, Windows[0].rect.getSize().y);

	ColorButton redColor = {};
	redColor.rect = {.pos = {0, 0}, .finishPos = {50, 50}};
	redColor.color = TX_LIGHTRED;
	redColor.mainCanvas = &mainCanvas;
	pointers[1] = &redColor;

	ColorButton blueColor = {};
	blueColor.rect = {.pos = {100, 0}, .finishPos = {150, 50}};  
	blueColor.color = TX_LIGHTBLUE;
	blueColor.mainCanvas = &mainCanvas;
	pointers[2] = &blueColor;

	ColorButton GreenColor = {};
	GreenColor.rect = {.pos = {200, 0}, .finishPos = {250, 50}};
	GreenColor.color = TX_LIGHTGREEN;
	GreenColor.mainCanvas = &mainCanvas;
	pointers[3] = &GreenColor;
	
	CleanButton cleanButton = {};
	cleanButton.rect = {.pos = {300, 0}, .finishPos = {350, 50}};
	cleanButton.color = TX_WHITE;
	cleanButton.mainCanvas = &mainCanvas;
	pointers[4] = &cleanButton;

	CloseButton closeButton = {};
	closeButton.rect = {.pos = {950, 0}, .finishPos = {1000, 50}};
	closeButton.color = TX_RED;
	pointers[5] = &closeButton;

	SizeButton minSize = {};
	minSize.rect = {.pos = {500, 0}, .finishPos = {550, 50}};
	minSize.color = TX_BLUE;
	minSize.mainCanvas = &mainCanvas;
	minSize.sizeType = -1;
	sprintf (minSize.text, "-");
	pointers[6] = &minSize;
	
	SizeButton plusRad = {};
	plusRad.rect = {.pos = {600, 0}, .finishPos = {650, 50}};
	plusRad.color = TX_GREEN;
	plusRad.mainCanvas = &mainCanvas;
	plusRad.sizeType = 1;
	sprintf (plusRad.text, "+");
	pointers[7] = &plusRad;

	TimeButton timeButton = {};
	timeButton.rect	= {.pos = {700, 0}, .finishPos = {850, 50}};
	timeButton.color = TX_GREEN;
	pointers[8] = &timeButton;


	for (;;)
	{
		drawButtons (pointers);
		checkClicked (pointers);
		//drawMouse ();
		if (!IsRunning) break;
	}

	
	txDeleteDC (mainCanvas.canvas);
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


void TimeButton::draw ()
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



void SizeButton::draw ()
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

void Canvas::draw ()
{
	// canvasPtr = this;

	assert (canvas);
	txBitBlt (rect.pos.x, rect.pos.y, canvas);

	if (txMouseButtons () == 2 && wasClicked)
	{
		txSetAllColors (drawColor, canvas);
		txSetColor (drawColor, lineThickness, canvas);
		txLine (lastClick.x, lastClick.y - rect.pos.y, txMouseX (), txMouseY () - rect.pos.y, canvas); 
		wasClicked = false;
	}

	if (wasClicked)
	{
		txSetAllColors (drawColor);
		txSetColor (drawColor, lineThickness);
		if (txMouseY () < rect.pos.y)
		{
			txLine (lastClick.x, lastClick.y, txMouseX (), rect.pos.y);	
		}
		else
		{
			txLine (lastClick.x, lastClick.y, txMouseX (), txMouseY ());
		}
	}

	if (clearBackground)
	{
		txSetAllColors (TX_BLACK, canvas);
		txRectangle (rect.pos.x, 0, rect.finishPos.x, rect.finishPos.y, canvas);
		clearBackground = false;
	}
}

void SizeButton::onClick ()
{
	if (!isClicked)
	{
		int mx = txMouseX ();
		if (sizeType > 0)
		{
			mainCanvas->lineThickness++;
		}
		if (sizeType < 0 && mainCanvas->lineThickness > 0)
		{
			mainCanvas->lineThickness--;
		}
	}
}

void CloseButton::onClick ()
{
	IsRunning = false;
}

void Canvas::onClick ()
{

	txSetAllColors ( drawColor);

	int currMouseButton = txMouseButtons ();
	
	if (!(isClicked))
	{
		int mx = txMouseX ();
		int my = txMouseY ();
		lastClick = {.x = txMouseX (), .y = txMouseY ()};
		wasClicked = true;
	}
}

void checkClicked (Window *pointers[PointersNum])
{
	for (int i = 0; i < PointersNum; i ++)
	{
		if (inButtonClicked (pointers[i]->rect))
		{
			//(pointers[i]->*(pointers[i]->funcClicked))();
			pointers[i]->onClick ();
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
		//(pointers[i]->*(pointers[i]->draw))();
		pointers[i]->draw ();
	}
}

void ColorButton::onClick ()
{
	mainCanvas->drawColor = color;
}


void CleanButton::onClick ()
{
	mainCanvas->clearBackground = true;
}