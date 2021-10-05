
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"



const int DELTACLOCKTIME = 100;



struct Window
{
	Rect rect;
	COLORREF color;
	const char *text;
	bool isClicked;

	//void (Window::*funcClicked) ();
	//void (Window::*funcDraw) ();

	virtual void draw ();
	virtual void onClick () {};

	Window (Rect _rect, COLORREF _color = TX_WHITE, const char *_text = "") :
		rect (_rect),
		color(_color),
		text (_text), 
		isClicked (false)

	{}
};

struct Canvas : Window
{
	HDC canvas;
	int lineThickness = 3;
	COLORREF drawColor = TX_RED;
	bool clearBackground = true;

	bool wasClicked = false;
	Vector lastClick = {};

	Canvas () : 
		Window ({.pos = {0, 50}, .finishPos = {1000, 1000}}, TX_BLACK),
		canvas (txCreateCompatibleDC (rect.getSize().x, rect.getSize().y))
	{}

	virtual void draw () override;
	virtual void onClick () override;
};


struct TimeButton : Window
{
	TimeButton (Rect _rect)	:
		Window (_rect)
	{}

	virtual void draw () override;
};

struct CloseButton : Window
{ 

	CloseButton (Rect _rect, COLORREF _color, const char *_text) :
		Window (_rect, _color, _text)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};

struct ColorButton : Window
{
	Canvas *mainCanvas;

	ColorButton(Rect _rect, COLORREF _color, Canvas *_mainCanvas) :
		Window (_rect, _color),
		mainCanvas (_mainCanvas) 
	{}

	virtual void onClick ()override;
};

struct SizeButton : Window
{
	Canvas *mainCanvas;
	int sizeType;

	SizeButton(Rect _rect, COLORREF _color, Canvas *_mainCanvas, int _sizeType, const char *_text) :
		Window (_rect, _color, _text),
		mainCanvas (_mainCanvas), 
		sizeType (_sizeType)
	{}

	virtual void draw () override;
	virtual void onClick () override;

};

struct SizeNumButton : Window
{
	Canvas *mainCanvas;
	int num;

	SizeNumButton(Rect _rect, COLORREF _color, Canvas *_mainCanvas, int _num, const char *_text)   :
		Window (_rect, _color, _text),
		mainCanvas (_mainCanvas), 
		num (_num)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};



struct CleanButton : Window
{
	Canvas *mainCanvas;

	CleanButton (Rect _rect, COLORREF _color, Canvas *_mainCanvas, const char *_text) :
		Window (_rect, _color, _text),
		mainCanvas (_mainCanvas)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};

struct Manager : Window
{
	int length;
	Window **pointers = NULL;
	int newButtonNum;
	bool advancedMode;

	Manager (Rect _rect, COLORREF _color, int _length, bool _advancedMode) : 
		Window (_rect, _color),
		length (_length),
		pointers (new Window*[length]{}),
		advancedMode (_advancedMode),
		newButtonNum (0)
	{}

	bool addWindow (Window *window);

	virtual void draw () override;
	virtual void onClick () override;
};

struct OpenManager : Window
{
	Manager *manager;

	OpenManager (Rect _rect, COLORREF _color, Manager *_manager, const char *_text)	:
		Window (_rect, _color, _text),
		manager (_manager)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};


struct ColorManager : Window 
{
	int length;
	Window **buttons;

	bool advancedMode;

	int newButtonNum;
	bool addWindow (Window *window);

	ColorManager (Rect _rect, int _length) :
		Window (_rect),
		length (_length),
		buttons (new Window*[length]{}),
		advancedMode (false),
		newButtonNum (0)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};

struct SizeManager : Window
{
	int length = 4;
	Window **buttons = new Window*[length];
		
	bool advancedMode = false;

	int newButtonNum = 0;
	bool addWindow (Window *window);

	SizeManager (Rect _rect, int _length) :
		Window (_rect),
		length (_length)
	{}

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

	OpenColorManager (Rect _rect, COLORREF _color, ColorManager *_manager, const char *_text) :
		Window (_rect, _color, _text),
		manager (_manager)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};

struct OpenSizeManager : Window
{
	SizeManager *manager;

	OpenSizeManager (Rect _rect, COLORREF _color, SizeManager *_manager, const char *_text)	:
		Window (_rect, _color, _text),
		manager (_manager)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};


struct StringButton : Window
{
	bool advancedMode;
	char inText[32] = {};
	int cursorPosition;
	int textLen;
	int placeNewNum;
	int lastButton;
	bool buttonClicked;
	bool allowEnter;
	int lastTimeClicked;
	Window *saveButton;
	bool lastConditionOfManager;
	Canvas *canvas;



	StringButton(Rect _rect, COLORREF _color, Window *_saveButton, Canvas *_canvas) :
		Window(_rect, _color),
		saveButton (_saveButton),
		canvas (_canvas),
		lastConditionOfManager (false),
		advancedMode(true),
		cursorPosition(-1),
		textLen (1),
		placeNewNum (-1),
		lastButton(0),
		buttonClicked(false),
		allowEnter(false),
		lastTimeClicked(0)
	{}


	virtual void draw () override;
	void checkSymbols ();
	void cursorMovement (int side);
	void backSpace ();
	void saveDC ();


};





//typedef void (Window::*funcClickedPtr) (const Window *);

		
//COLORREF CurrColor = TX_RED;
//const int PointersNum = 9;
bool IsRunning = true;
int Radius = 2;
int SLEEPTIME = 30;
const Vector SCREENSIZE = {1000, 1000};







void RECTangle (const Rect rect, HDC dc = txDC ());
void txSetAllColors (COLORREF color, HDC dc = txDC ());
void engine (Manager *manager);
void shiftArrBack (char arr[], int length);
void shiftArrForward (char arr[], int length);
bool checkDeltaTime (int lastTimeClicked);


int main ()
{
	txCreateWindow (SCREENSIZE.x, SCREENSIZE.y);
	txSelectFont ("Arial", 20, 13);
	_mkdir ("Images");
	//printf ("פûאפûא");

	
	//Window Windows[WindowNum] = {};
	//Window* pointers[PointersNum] = {};
	Manager *manager = new Manager({.pos = {0, 0}, .finishPos = {1000, 1000}}, TX_WHITE, 10, true);

	Canvas *mainCanvas = new Canvas();
	manager->addWindow (mainCanvas);

	Manager *colorManager = new Manager({.pos = {75, 0}, .finishPos = {250, 50}}, TX_WHITE, 3, false);
	manager->addWindow (colorManager);

		ColorButton *redColor = new ColorButton({.pos = {75, 0}, .finishPos = {125, 50}}, TX_LIGHTRED, mainCanvas);
		colorManager->addWindow(redColor);
	
		ColorButton *blueColor = new ColorButton({.pos = {150, 0}, .finishPos = {200, 50}}, TX_LIGHTBLUE, mainCanvas);
		colorManager->addWindow(blueColor);

		ColorButton *greenColor = new ColorButton({.pos = {225, 0}, .finishPos = {275, 50}}, TX_LIGHTGREEN, mainCanvas);
		colorManager->addWindow(greenColor);

	OpenManager *openManager = new OpenManager({.pos = {0, 0}, .finishPos = {50, 50}}, TX_WHITE, colorManager, "->");
	manager->addWindow (openManager);

	CleanButton *cleanButton = new CleanButton({.pos = {400, 0}, .finishPos = {450, 50}}, TX_WHITE, mainCanvas, "Clear");
	manager->addWindow (cleanButton);

	CloseButton *closeButton = new CloseButton({.pos = {950, 0}, .finishPos = {1000, 50}}, TX_RED, "X");
	manager->addWindow (closeButton);

	Manager *sizeManager = new Manager({.pos = {575, 0}, .finishPos = {840, 50}}, TX_WHITE, 4, false);
	manager->addWindow (sizeManager);

		SizeButton *minSize = new SizeButton({.pos = {575, 0}, .finishPos = {625, 50}}, TX_BLUE, mainCanvas, -1, "-");
		sizeManager->addWindow (minSize);
	
		SizeButton *plusRad = new SizeButton({.pos = {650, 0}, .finishPos = {700, 50}}, TX_GREEN, mainCanvas, 1, "+");
		sizeManager->addWindow (plusRad);

		SizeNumButton *one = new SizeNumButton({.pos = {725, 0}, .finishPos = {775, 50}}, TX_RED, mainCanvas, 1, "1");
		sizeManager->addWindow (one);

		SizeNumButton *ten = new SizeNumButton({.pos = {790, 0}, .finishPos = {840, 50}}, TX_RED, mainCanvas, 10, "10");
		sizeManager->addWindow (ten);

	OpenManager *openSizeManager = new OpenManager({.pos = {500, 0}, .finishPos = {550, 50}}, TX_WHITE, sizeManager, "->");
	manager->addWindow (openSizeManager);

	TimeButton *timeButton = new TimeButton ({.pos = {835, 0}, .finishPos = {850, 50}});
	manager->addWindow (timeButton);

	Manager *stringManager = new Manager({.pos = {300, 0}, .finishPos = {650, 100}}, TX_WHITE, 2, false); 
	manager->addWindow (stringManager);

		Window *saveButton = new Window ({.pos = {345, 0}, .finishPos = {395, 50}}, TX_WHITE, "<-");
		stringManager->addWindow (saveButton);

		StringButton *stringButton = new StringButton ({.pos = {285, 55}, .finishPos = {650, 105}}, TX_WHITE, saveButton, mainCanvas);
		stringManager->addWindow (stringButton);
		

	OpenManager *openSaveNameManager = new OpenManager ({.pos = {285, 0}, .finishPos = {335, 50}}, TX_WHITE, stringManager, "S");
	manager->addWindow (openSaveNameManager);

	

	txBegin ();

	engine (manager);

	/*
	for (;;)
	{
		//drawButtons (pointers);
		//checkClicked (pointers);
		//drawMouse ();
		if (!IsRunning) break;
	} */

	//delete (manager);
	
	txDeleteDC (mainCanvas->canvas);
	txDisableAutoPause ();

	return 0;

}

bool Manager::addWindow (Window *window)
{
	if (newButtonNum >= length) return 0;

	pointers[newButtonNum] = window;
	newButtonNum++;
	
	return 1;
}

void Manager::draw ()
{
	if (advancedMode)
	{
		for (int i = 0; i < newButtonNum; i++)
		{
			pointers[i]->draw();
			if (txMouseButtons () != 1 && pointers[i]->isClicked != false)
			{
				pointers[i]->isClicked = false;
			}
		}
	}
}	

void Manager::onClick ()
{
	int mx = txMouseX ();
	int my = txMouseY ();
	if (advancedMode)
	{
		for (int i = 0; i < newButtonNum; i++)
		{
			
			if (pointers[i]->rect.inRect (mx, my))
			{
				pointers[i]->onClick ();
				pointers[i]->isClicked = true;
			}
			else
			{
				pointers[i]->isClicked = false;
			}
		}
	}
}



bool ColorManager::addWindow (Window *window)
{
	if (newButtonNum >= length) return 0;

	buttons[newButtonNum] = window;
	newButtonNum++;

	return 1;
}

bool SizeManager::addWindow (Window *window)
{
	if (newButtonNum >= length) return 0;

	buttons[newButtonNum] = window;
	newButtonNum++;
	
	return 1;
}

void StringButton::draw ()
{
	$s;
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	if (saveButton->isClicked == true) 
	{
		lastConditionOfManager = true;
	}
	

	if (lastConditionOfManager == true && saveButton->isClicked == false)
	{
		char path[OFS_MAXPATHNAME] = {};
		sprintf (path, "Images/%s.bmp", inText);

		printf ("%d", txSaveImage (path, canvas->canvas));

		lastConditionOfManager = false;
	}

	bool isShifted = false;
	bool deleteChar = false;
	if (checkDeltaTime (lastTimeClicked))
	{
		if (txGetAsyncKeyState(VK_LEFT))
		{
			cursorMovement (VK_LEFT);
		}
		if (txGetAsyncKeyState(VK_RIGHT))
		{
			cursorMovement (VK_RIGHT);
		}

		if (txGetAsyncKeyState (VK_BACK))
		{
			backSpace ();	
		}
		
		checkSymbols ();
	}
	
	if (clock() % 500 < 250)
	{
		inText[cursorPosition + 1] = '|';
	}
	else
	{
		inText[cursorPosition + 1] = ' ';
	}


	txSetTextAlign (TA_LEFT);
	txSetAllColors (TX_BLACK);

	txTextOut (rect.pos.x, rect.pos.y, inText);
}


void StringButton::checkSymbols ()
{
	if (!_kbhit ())	return;
	
	int symbol = _getch ();

	if (lastButton == symbol || symbol == '\b') return;

	lastTimeClicked = clock();
	shiftArrForward(&inText[cursorPosition + 1], textLen - cursorPosition);
	inText[cursorPosition + 1] = symbol;
	cursorPosition++;
	textLen++;
}

bool checkDeltaTime (int lastTimeClicked)
{
	return clock () - lastTimeClicked > DELTACLOCKTIME;
}

void StringButton::backSpace ()
{
	if (cursorPosition >= 0)
	{
		lastTimeClicked = clock();
		shiftArrBack(&inText[cursorPosition], textLen - cursorPosition  + 1);
		cursorPosition--;
		textLen--;
	}
}


void StringButton::cursorMovement (int side)
{
	bool validMovement = false;	
	if (side == VK_RIGHT) validMovement = (cursorPosition < textLen - 2);
	if (side == VK_LEFT)  validMovement = (cursorPosition >= 0);

	if (validMovement && clock () - lastTimeClicked > DELTACLOCKTIME)
	{
		lastTimeClicked = clock();
		shiftArrBack(&inText[cursorPosition + 1], textLen - cursorPosition);
		if (side == VK_RIGHT) cursorPosition++;
		if (side == VK_LEFT) cursorPosition--;
		shiftArrForward(&inText[cursorPosition + 1], textLen - cursorPosition);
	}
}

void shiftArrForward (char arr[], int length)
{
	char copyChar = arr[0];

	for (int i = 1; i < length; i++)
	{
		char rememberArr = arr[i];
		arr[i] = copyChar;
		copyChar = rememberArr;
	}
}

void shiftArrBack (char arr[], int length)
{
	char copyChar = '\0';

	for (int i = 0; i < length; i++)
	{
		if (i == length - 1)
		{
			arr[i] = '\0';
		}
		else
		{
			arr[i] = arr[i + 1];
		}
		
	}
}

void OpenSizeManager::draw()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void OpenSizeManager::onClick ()
{
	if (!isClicked)
	{
		manager->advancedMode = !manager->advancedMode;
	}
}

void OpenManager::onClick ()
{
	if (!isClicked)
	{
		manager->advancedMode = !manager->advancedMode;
		manager->draw ();
	}
}

void OpenManager::draw()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void OpenColorManager::draw()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void OpenColorManager::onClick ()
{
	if (!isClicked)
	{
		manager->advancedMode = !manager->advancedMode;
	}
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
		for (int i = 0; i < length; i++)
		{
			buttons[i]->draw();
			if (inButtonClicked (buttons[i]->rect))
			{
				buttons[i]->onClick ();	
			}
		}
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
		for (int i = 0; i < length; i++)
		{
			buttons[i]->draw();

			if (inButtonClicked (buttons[i]->rect))
			{
				buttons[i]->onClick ();	
			}
		}
	}
}

void ColorManager::onClick ()
{
	advancedMode = true;
}

void engine (Manager *manager)
{
	for (;;)
	{
		manager->draw ();
		if (txMouseButtons () == 1 && manager->rect.inRect (txMouseX (), txMouseY ()))
		{
			manager->onClick ();
			if (!IsRunning) break;
		}
		txSleep (30);

		txSetAllColors (TX_BLACK);
		txRectangle (manager->rect.pos.x, manager->rect.pos.y, manager->rect.finishPos.x, manager->rect.finishPos.y);
		//txClear ();
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
	//RECTangle (rect);
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
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 5, 
					text);
}

void CleanButton::draw()
{
	$s
		txSetAllColors (color);
		txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

		txSetTextAlign (TA_CENTER);
			txSetAllColors (TX_BLACK);
			txSelectFont ("Arial", 20);
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
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
}

void Window::draw ()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	txSelectFont ("Arial", 40);
	txSetAllColors (TX_BLACK);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 5, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 10, 
					text);
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

/*
void GeneralManager::onClick ()
{
	for (int i = 0; i < length; i ++)
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
	for (int i = 0; i < length; i ++)
	{
		//(pointers[i]->*(pointers[i]->draw))();
		pointers[i]->draw ();
	}
} */

void ColorButton::onClick ()
{
	mainCanvas->drawColor = color;
}


void CleanButton::onClick ()
{
	mainCanvas->clearBackground = true;
}