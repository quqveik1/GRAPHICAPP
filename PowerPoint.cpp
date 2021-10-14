
#define _CRT_SECURE_NO_WARNINGS
#include "Q_Vector.h"
#include "Q_Rect.h"
#include "Q_Buttons.h"
#include <cmath>



const int DELTACLOCKTIME = 100;


#define printBlt(image)         \
{                               \
    txBitBlt (0, 0, image);     \
    _getch ();                  \
    txClear ();                 \
}



struct Window
{
	Rect rect;
	COLORREF color;
	const char *text;
	bool isClicked;
	HDC dc;

	//void (Window::*funcClicked) ();
	//void (Window::*funcDraw) ();

	virtual void draw ();
	virtual void onClick () {};

	Window (Rect _rect, COLORREF _color = TX_WHITE, const char *_text = "", HDC _dc = NULL) :
		rect (_rect),
		color(_color),
		text (_text), 
		isClicked (false), 
		dc (_dc)

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

	Canvas (Rect _rect, COLORREF _color) : 
		Window (_rect, _color),
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

	CloseButton (Rect _rect, COLORREF _color, HDC _dc) :
		Window (_rect, _color, "", _dc)
	{}

	virtual void draw () override;
	virtual void onClick () override;
};

struct ColorButton : Window
{
	Canvas *mainCanvas;

	ColorButton(Rect _rect, COLORREF _color, Canvas *_mainCanvas, HDC _dc) :
		Window (_rect, _color, "", _dc),
		mainCanvas (_mainCanvas)
	{}

	virtual void onClick ()override;
};

struct SizeButton : Window
{
	int *num;
	int sizeType;

	SizeButton(Rect _rect, int *_num, int _sizeType) :
		Window (_rect),
		num (_num), 
		sizeType (_sizeType)
	{}

	virtual void draw () override;
	virtual void onClick () override;

};

/*
struct SwitchButton :Window
{
	SizeButton *plus;
	SizeButton *minus;

	SwitchButton (Rect _rect, SizeButton *_plus, SizeButton *_minus, HDC _dc) :
		Window (_rect, TX_WHITE, "", _dc),
		plus (_plus),
		minus (_minus)
	{}

	virtual void draw () override;
	virtual void onClick () override;

};	 */

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

	CleanButton (Rect _rect, COLORREF _color, Canvas *_mainCanvas, HDC _dc = NULL, const char *_text = "") :
		Window (_rect, _color, _text, _dc),
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

	Manager (Rect _rect, COLORREF _color, int _length, bool _advancedMode, HDC _dc = NULL) : 
		Window (_rect, _color, "", _dc),
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

	OpenManager (Rect _rect, COLORREF _color, Manager *_manager, HDC _dc = NULL, const char *_text = "")	:
		Window (_rect, _color, _text, _dc),
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
	char *inText;
	int cursorPosition;
	int textLen;
	int lastButton;
	bool buttonClicked;
	bool allowEnter;
	int lastTimeClicked;
	bool lastConditionOfManager;
	bool onlyNums;
	const int MaxSizeOfText;



	StringButton(Rect _rect, COLORREF _color, char *_redactingText, int _redactingTextLength, int _MaxSizeOfText = 5, bool _onlyNums = false) :
		Window(_rect, _color),
		inText (_redactingText),
		lastConditionOfManager (false),
		advancedMode(true),
		cursorPosition(_redactingTextLength - 1),
		textLen (_redactingTextLength + 1),
		lastButton(0),
		buttonClicked(false),
		allowEnter(false),
		lastTimeClicked(0),
		MaxSizeOfText (_MaxSizeOfText + 1),
		onlyNums (_onlyNums)
	{}


	virtual void draw () override;
	void checkSymbols ();
	void cursorMovement (int side);
	void backSpace ();
};

struct StringSizeButton	: StringButton
{
	int *num;
	char redactingText[32] = {};

	StringSizeButton(Rect _rect, COLORREF _color, int _redactingTextLength, int *_num) :
		StringButton (_rect, _color, redactingText, _redactingTextLength, true),
		num (_num)
	{}

	virtual void draw () override;

};

struct NumSlider : Window
{
	NumSlider (Rect _rect) :
		Window (_rect)
	{}
};


struct NumChange : Window
{
	StringButton stringButton;
	SizeButton plusNum;
	SizeButton minusNum;
	HDC plusMinusButtons;
	NumSlider numSlider;
	int *num;
	char inText[32] = {};

	NumChange (Rect _mainRect, Rect _stringRect, Rect _plusRect, Rect _minusRect, Rect _numSliderRect, HDC _plusMinusButtons, int _numLength, int *_num) :
		Window (_mainRect),
		num (_num),
		stringButton (_stringRect, TX_WHITE, inText, _numLength, 5, true),
		plusNum (_plusRect, _num, +1),
		minusNum (_minusRect, _num, -1),
		plusMinusButtons (_plusMinusButtons),
		numSlider (_numSliderRect)
	{}

	virtual void draw () override;
	virtual void onClick () override;

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
void printfDCS (const char *str = "");
void createNumChanger (Manager *menu, NumChange **numchange, Canvas *mainCanvas);


int main ()
{
	txCreateWindow (SCREENSIZE.x, SCREENSIZE.y);
	txSelectFont ("Arial", 28, 10);

	Manager *manager = new Manager({.pos = {0, 0}, .finishPos = {1000, 1000}}, TX_WHITE, 11, true);

	Canvas *mainCanvas = new Canvas({.pos = {0, 0}, .finishPos = {1000, 1000}}, TX_BLACK);
	manager->addWindow (mainCanvas);

	Manager *menu = new Manager({.pos = {0, 0}, .finishPos = {412, 300}}, TX_WHITE, 5, true, txLoadImage ("HUD-3.bmp"));
	manager->addWindow (menu);

		Manager *colorManager = new Manager({.pos = {10, 180}, .finishPos = {170, 220}}, TX_WHITE, 3, false);
		menu->addWindow (colorManager);

			ColorButton *redColor = new ColorButton({.pos = {10, 180}, .finishPos = {50, 220}}, RGB (255, 0, 0), mainCanvas, txLoadImage ("RedButton.bmp"));
			colorManager->addWindow(redColor);
	
			ColorButton *blueColor = new ColorButton({.pos = {70, 180}, .finishPos = {110, 220}}, RGB (0, 0, 255), mainCanvas, txLoadImage ("BlueButton.bmp"));
			colorManager->addWindow(blueColor);

			ColorButton *greenColor = new ColorButton({.pos = {130, 180}, .finishPos = {170, 220}}, RGB (0, 255, 0), mainCanvas, txLoadImage ("GreenButton.bmp"));
			colorManager->addWindow(greenColor);

		OpenManager *openManager = new OpenManager({.pos = {15, 135}, .finishPos = {36, 153}}, TX_WHITE, colorManager, txLoadImage ("OpenColorButton.bmp"));
		menu->addWindow (openManager);

	NumChange *numChange = NULL;
	createNumChanger (menu, &numChange, mainCanvas);


	CleanButton *cleanButton = new CleanButton({.pos = {10, 90}, .finishPos = {94, 121}}, TX_WHITE, mainCanvas, txLoadImage ("CleanButton.bmp"));
	menu->addWindow (cleanButton);

	CloseButton *closeButton = new CloseButton({.pos = {950, 0}, .finishPos = {1000, 50}}, TX_RED, txLoadImage ("CloseButton.bmp"));
	manager->addWindow (closeButton);

	TimeButton *timeButton = new TimeButton ({.pos = {835, 0}, .finishPos = {850, 50}});
	menu->addWindow (timeButton);

	txBegin ();

	engine (manager);

	txDeleteDC (mainCanvas->canvas);
	txDeleteDC (redColor->dc);
	txDeleteDC (blueColor->dc);
	txDeleteDC (greenColor->dc);
	txDeleteDC (numChange->plusMinusButtons);
	txDeleteDC (cleanButton->dc);
	txDeleteDC (menu->dc); 
	txDeleteDC (openManager->dc);
	txDeleteDC (closeButton->dc);
	txDisableAutoPause ();

	return 0;

}

void createNumChanger (Manager *menu, NumChange **numChange, Canvas *mainCanvas)
{
	//Manager *sizeManager = new Manager({.pos = {0, 0}, .finishPos = {389, 75}}, TX_WHITE, 4, true);
	//menu->addWindow (sizeManager);


	*numChange = (new NumChange ({.pos = {296, 44}, .finishPos = {389, 74}}, {.pos = {296, 46}, .finishPos = {362, 74}}, {.pos = {368, 44}, .finishPos = {389, 59}}, {.pos = {368, 59}, .finishPos = {389, 74}}, {.pos = {0, 0}, .finishPos = {368, 74}}, txLoadImage ("SwitchButton.bmp"), 1, &mainCanvas->lineThickness));
	menu->addWindow (*numChange);
}

void printfDCS (const char *str)
{
    printf ("%s\n", str);
    for (int i = 0; i < _txCanvas_UserDCs->size (); i++)
    {
        printf ("%d: %p\n", i, (*_txCanvas_UserDCs)[i]);
    }
    _getch ();
}


void NumChange::draw ()
{
	if (! (*num == 0))
		sprintf (inText, "%d", *num);

	stringButton.draw ();

	if (!strcmp ("", inText))
		*num = 0;
	sscanf  (inText, "%d", num);

	txBitBlt (plusNum.rect.pos.x, plusNum.rect.pos.y, plusMinusButtons); 

	plusNum.draw  ();
	minusNum.draw ();

	if (txMouseButtons () != 1 && plusNum.isClicked != false)
	{
		plusNum.isClicked = false;
	}

	if (txMouseButtons () != 1 && minusNum.isClicked != false)
	{
		minusNum.isClicked = false;
	}
}


void NumChange::onClick ()
{
	int mx = txMouseX ();
	int my = txMouseY ();

	

	if (plusNum.rect.inRect (mx, my))
	{
		if ((*num == 9) || (*num == 99) || (*num == 999) || (*num == 9999) && !plusNum.isClicked)
		{
			stringButton.textLen++;
			stringButton.cursorPosition++;
		}
		if (!plusNum.isClicked)
		{
			stringButton.cursorPosition =  stringButton.textLen-2;
		}
		plusNum.onClick ();
		plusNum.isClicked = true;
	}
	else
	{
		plusNum.isClicked = false;
	}

	if (minusNum.rect.inRect (mx, my))
	{
		if ((*num == 10) || (*num == 100) || (*num == 1000) || (*num == 10000)&& !plusNum.isClicked)
		{
			stringButton.textLen--;
			stringButton.cursorPosition--;
		}
		if (!plusNum.isClicked)
		{
			stringButton.cursorPosition =  stringButton.textLen-2;
		}
		minusNum.onClick ();
		minusNum.isClicked = true;
	}
	else
	{
		minusNum.isClicked = false;
	}


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
		if (dc != NULL) txBitBlt (rect.pos.x, rect.pos.y, dc);
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
		for (int i = newButtonNum - 1; i >= 0; i--)
		{
			
			if (pointers[i]->rect.inRect (mx, my))
			{
				pointers[i]->onClick ();
				pointers[i]->isClicked = true;
				break;
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

void StringSizeButton::draw ()
{
	$s;
	//txSetAllColors (color);
	///txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	 /*
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
	*/

	sprintf (inText, "%d", *num);


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

	inText[textLen]	= 0;


	txSetTextAlign (TA_LEFT);
	txSetAllColors (color);

	//txSelectFont ("Arial", 30);
	
	txTextOut (rect.pos.x, rect.pos.y, inText);

	sscanf (inText, "%d", num);
}

void StringButton::draw ()
{
	$s;
	//txSetAllColors (color);
	///txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	 /*
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
	*/


	bool switched = false;
	bool isShifted = false;
	bool deleteChar = false;
	if (checkDeltaTime (lastTimeClicked))
	{
		if (txGetAsyncKeyState(VK_LEFT) && cursorPosition >= 0)
		{
			cursorMovement (VK_LEFT);
			//printf ("left");
			switched = true;
		}
		if (txGetAsyncKeyState(VK_RIGHT) && cursorPosition <= textLen - 1)
		{
			cursorMovement (VK_RIGHT);
			switched = true;
		}

		if (txGetAsyncKeyState (VK_BACK))
		{
			backSpace ();	
		}
		
		checkSymbols ();
	}
	
	if (clock() % 500 < 250)
	{
		if (!switched)
			shiftArrForward (&inText[cursorPosition + 1], 10);

		inText[cursorPosition + 1] = '|';
		printf ("");
	}
	else
	{
		if (!switched)
			shiftArrForward (&inText[cursorPosition + 1], 10);

		inText[cursorPosition + 1] = ' ';
	}


	if (! (inText[textLen] >= 48 && inText[textLen] <= 125)) 
	{
		//inText[textLen]	= 0;
	}
	else
	{
		//textLen++;
		//cursorPosition++;
	}

	  


	txSetTextAlign (TA_LEFT);
	txSetAllColors (TX_WHITE);
	
	txTextOut (rect.pos.x, rect.pos.y, inText);

	shiftArrBack (&inText[cursorPosition + 1], 10);

	return;
}

void stringToInt (const char *str, const int  strLen, int &num)
{
	num = 0;
	for (int i = 0; i < strLen; i++)
	{
		num += (str[i] - 48) * pow (10, strLen - i - 1);
	}
}


void StringButton::checkSymbols ()
{
	if (!_kbhit ())	return;
	int symbol = _getch ();

	if (cursorPosition + 2 == MaxSizeOfText) return;
	if (! (symbol >= 48 && symbol <= 57) && onlyNums) return;
	if (lastButton == symbol || symbol == '\b') return;
	if (symbol == 48 &&	onlyNums && cursorPosition == -1) return;

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
		//shiftArrBack(&inText[cursorPosition + 1], textLen - cursorPosition);
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
		//txSelectFont ("Arial", 40);
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
	//txSetAllColors (color);
	//txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	if (dc) txBitBlt (rect.pos.x, rect.pos.y, dc);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		//txSelectFont ("Arial", 40);
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
		//txSelectFont ("Arial", 40);
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
		//txSelectFont ("Arial", 40);
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
	//txSetAllColors (color);
	//txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	if (dc) txBitBlt (rect.pos.x, rect.pos.y, dc);

	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		//txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
					rect.pos.y + (rect.finishPos.y - rect.pos.y) / 5, 
					text);
}

void CleanButton::draw()
{
	$s
		//txSetAllColors (color);
		//txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
		txBitBlt (rect.pos.x, rect.pos.y, dc);
		

		txSetTextAlign (TA_CENTER);
			txSetAllColors (TX_BLACK);
			//txSelectFont ("Arial", 20);
			txTextOut (rect.pos.x + (rect.finishPos.x - rect.pos.x) / 2, 
						rect.pos.y + (rect.finishPos.y - rect.pos.y) / 4, 
						text);
}

void CloseButton::draw()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);
	txTransparentBlt (rect.pos.x, rect.pos.y, dc);

		/*
	txSetTextAlign (TA_CENTER);
		txSetAllColors (TX_GRAY);
		//txSelectFont ("Arial", 40);
		txTextOut (rect.pos.x, 
					rect.pos.y, 
					text);
					*/
}

void Window::draw ()
{
	$s
	txSetAllColors (color);
	txRectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y);

	if (dc != NULL) txBitBlt (rect.pos.x, rect.pos.y, dc);

	//txSelectFont ("Arial", 40);
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
			(*num)++;
		}
		if (sizeType < 0 && num > 0)
		{
			(*num)--;
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