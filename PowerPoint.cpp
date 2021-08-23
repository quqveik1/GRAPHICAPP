
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
	virtual void draw () override;
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

struct SizeNumButton : Window
{
	Canvas *mainCanvas;
	int num;

	virtual void draw () override;
	virtual void onClick () override;
};



struct CleanButton : Window
{
	Canvas *mainCanvas;

	virtual void onClick () override;
};

struct GeneralManager : Window
{
	enum { pointersK = 6};
	Window *pointers[pointersK];
	//bool advancedMode = false;

	virtual void draw () override;
	virtual void onClick () override;
};

struct ColorManager : Window 
{
	enum {buttonsK = 4};
	Window *buttons[buttonsK];

	bool advancedMode = false;

	virtual void draw () override;
	virtual void onClick () override;
};

struct SizeManager : Window
{
	enum {buttonsK = 5};
	Window *buttons[buttonsK];
		
	bool advancedMode = false;

	virtual void draw () override;
	virtual void onClick () override;
};

struct CloseSizeButtons : Window
{
	SizeManager *manager;

	virtual void onClick () override;
};

struct CloseColorManager : Window
{
	ColorManager *manager;

	virtual void onClick () override;
};

struct OpenColorManager : Window
{
	ColorManager *manager;

	virtual void onClick () override;
};





//typedef void (Window::*funcClickedPtr) (const Window *);

		
//COLORREF CurrColor = TX_RED;
//const int PointersNum = 9;
bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;







void RECTangle (const Rect rect, HDC dc = txDC ());
void txSetAllColors (COLORREF color, HDC dc = txDC ());
void engine (GeneralManager &manager);
//void drawButtons (Window *pointers[PointersNum]);
//void checkClicked (Window *pointers[PointersNum]);

int main ()
{
	txCreateWindow (1000, 1000);
	txSelectFont ("Arial", 40);

	
	//Window Windows[WindowNum] = {};
	//Window* pointers[PointersNum] = {};
	GeneralManager manager;

	Canvas mainCanvas = {};
	mainCanvas.rect = {.pos = {0, 50}, .finishPos = {1000, 1000}};
	mainCanvas.color = TX_BLACK;
	mainCanvas.canvas = txCreateCompatibleDC (mainCanvas.rect.getSize().x, mainCanvas.rect.getSize().y);
	manager.pointers[0] = &mainCanvas;
	///Windows[0].canvas = txCreateCompatibleDC (Windows[0].rect.getSize().x, Windows[0].rect.getSize().y);

	ColorManager colorManager = {};
	colorManager.rect = {.pos = {0, 0}, .finishPos = {50, 50}};
	colorManager.color = TX_LIGHTRED;
	manager.pointers[1] = &colorManager;

		ColorButton redColor = {};
		redColor.rect = {.pos = {0, 0}, .finishPos = {50, 50}};
		redColor.color = TX_LIGHTRED;
		redColor.mainCanvas = &mainCanvas;
		colorManager.buttons[0] = &redColor;
	

		ColorButton blueColor = {};
		blueColor.rect = {.pos = {75, 0}, .finishPos = {125, 50}};  
		blueColor.color = TX_LIGHTBLUE;
		blueColor.mainCanvas = &mainCanvas;
		colorManager.buttons[1] = &blueColor;

		ColorButton GreenColor = {};
		GreenColor.rect = {.pos = {150, 0}, .finishPos = {200, 50}};
		GreenColor.color = TX_LIGHTGREEN;
		GreenColor.mainCanvas = &mainCanvas;
		colorManager.buttons[2] = &GreenColor;

	
	CleanButton cleanButton = {};
	cleanButton.rect = {.pos = {400, 0}, .finishPos = {450, 50}};
	cleanButton.color = TX_WHITE;
	cleanButton.mainCanvas = &mainCanvas;
	manager.pointers[2] = &cleanButton;

	CloseButton closeButton = {};
	closeButton.rect = {.pos = {950, 0}, .finishPos = {1000, 50}};
	closeButton.color = TX_RED;
	sprintf (closeButton.text, "X");
	manager.pointers[3] = &closeButton;

	SizeManager sizeManager = {};
	sizeManager.rect = {.pos = {500, 0}, .finishPos = {550, 50}};
	sizeManager.color = TX_BLUE;
	manager.pointers[4] = &sizeManager;


		SizeButton minSize = {};
		minSize.rect = {.pos = {500, 0}, .finishPos = {550, 50}};
		minSize.color = TX_BLUE;
		minSize.mainCanvas = &mainCanvas;
		minSize.sizeType = -1;
		sprintf (minSize.text, "-");
		sizeManager.buttons[0] = &minSize;
	
		SizeButton plusRad = {};
		plusRad.rect = {.pos = {575, 0}, .finishPos = {625, 50}};
		plusRad.color = TX_GREEN;
		plusRad.mainCanvas = &mainCanvas;
		plusRad.sizeType = 1;
		sprintf (plusRad.text, "+");
		sizeManager.buttons[1] = &plusRad;

		SizeNumButton one = {};
		one.rect = {.pos = {650, 0}, .finishPos = {700, 50}};
		one.color = TX_RED;
		one.mainCanvas = &mainCanvas;
		one.num = 1;
		sprintf (one.text, "1");
		sizeManager.buttons[2] = &one;

		SizeNumButton ten = {};
		ten.rect = {.pos = {725, 0}, .finishPos = {775, 50}};
		ten.color = TX_RED;
		ten.mainCanvas = &mainCanvas;
		ten.num = 10;
		sprintf (ten.text, "10");
		sizeManager.buttons[3] = &ten;

		CloseSizeButtons closeSize = {};
		closeSize.rect = {.pos = {800, 0}, .finishPos = {825, 50}};
		closeSize.color = TX_GREEN;
		closeSize.manager = &sizeManager;
		sizeManager.buttons[4] = &closeSize;


	TimeButton timeButton = {};
	timeButton.rect	= {.pos = {835, 0}, .finishPos = {850, 50}};
	timeButton.color = TX_GREEN;
	manager.pointers[5] = &timeButton;

	engine (manager);

	/*
	for (;;)
	{
		//drawButtons (pointers);
		//checkClicked (pointers);
		//drawMouse ();
		if (!IsRunning) break;
	} */

	
	txDeleteDC (mainCanvas.canvas);
	txDisableAutoPause ();

}

void OpenColorManager::onClick ()
{
	manager->advancedMode = !manager->advancedMode;
}

void CloseSizeButtons::onClick ()
{
	manager->advancedMode = false;
}

void SizeNumButton::onClick ()
{
	mainCanvas->lineThickness = num;
}

void SizeNumButton::draw ()
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

void SizeManager::onClick ()
{
	advancedMode = true;
}

void SizeManager::draw()
{
	if (advancedMode)
	{
		for (int i = 0; i < buttonsK; i++)
		{
			buttons[i]->draw();
			if (inButtonClicked (buttons[i]->rect))
			{
				buttons[i]->onClick ();	
			}
		}
	}
	else
	{
		txSetAllColors (color);
		RECTangle (rect);
	}
}

void CloseColorManager::onClick ()
{
	manager->advancedMode = false;
}

void ColorManager::draw ()
{
	if (advancedMode)
	{
		for (int i = 0; i < buttonsK; i++)
		{
			buttons[i]->draw();

			if (inButtonClicked (buttons[i]->rect))
			{
				buttons[i]->onClick ();	
			}
		}
	}
	else
	{
		txSetAllColors (color);
		RECTangle (rect);
	}
}

void ColorManager::onClick ()
{
	advancedMode = true;
}

void engine (GeneralManager &manager)
{
	for (;;)
	{
		manager.draw ();
		manager.onClick ();
		if (!IsRunning) break;
	}
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

void CloseButton::draw()
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

void GeneralManager::onClick ()
{
	for (int i = 0; i < pointersK; i ++)
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

void GeneralManager::draw ()
{
	txSleep (SLEEPTIME);
	txSetAllColors (TX_BLACK);
	txClear ();
	$s
	for (int i = 0; i < pointersK; i ++)
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