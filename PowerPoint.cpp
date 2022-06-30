#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "DrawBibliothek.cpp"
#include "GlobalOptions.h"
#include "Q_Buttons.h"
#include <cmath>
#include "StandartFunctions.h"
#include "CurvesFilter.h"
#include "Slider.cpp"
#include "Canvas.h"
#include "TransferStructure.h"
#include "DLLFiltersManager.cpp"
#include "Tool.h"
#include "DLLToolsManager.cpp"
#include "CLay.cpp"
#include "MainTools.cpp"
#include "CanvasManager.cpp"
#include "ProgressBar.h"
#include "LoadManager.cpp"
#include "Lay.cpp"
#include "GlobalOptions.cpp"
#include "AppApi.cpp"
#include "ToolsMenu.cpp"
#include "OpenManager.cpp"
#include "List.cpp"
#include "ThreeUpWindows.cpp"
#include "LaysMenu.cpp" 
#include "TouchButton.cpp"
#include "AddCanvasButton.cpp"
#include "Handle.cpp"
#include "MainManager.cpp"
#include "SaveImages.cpp"
#include "ColorComponentChanger.cpp"
#include "ColorMenu.cpp"

/*
struct TimeButton : Window
{
	int font;
	TimeButton (AbstractAppData* _app, Rect _rect, COLORREF _color = TX_RED, int _font = NULL)	:
		Window (_app, _rect, _color),
		font (_font)
	{
        if (!font) font = _app->systemSettings->MainFont;
    }

	virtual void draw () override;
};




struct StringButton : Window
{
	bool advancedMode;
	char *inText;
	int cursorPosition;
	int textLen;
	int lastButton;
	int lastTimeClicked;
	bool onlyNums;
	const int MaxNum;



	StringButton(AbstractAppData* _app, Rect _rect, COLORREF _color, char *_redactingText, int _redactingTextLength, Manager *_manager, int _MaxNum = 20, bool _onlyNums = false) :
		Window(_app, _rect, _color, NULL, _manager),
		inText (_redactingText),
		advancedMode(true),
		cursorPosition(_redactingTextLength - 1),
		textLen (_redactingTextLength + 1),
		lastButton (0),
		lastTimeClicked (0),
		onlyNums (_onlyNums),
		MaxNum (_MaxNum)
	{}


	virtual void draw () override;
	void checkSymbols ();
	void cursorMovement (int side);
	void backSpace ();
};

struct StatusBar : Manager
{

	TimeButton *timeButton = NULL;
	ProgressBar *progressBar = NULL;

	StatusBar(AbstractAppData* _app, Rect _rect, COLORREF _color) :
		Manager(_app, _rect, 3, true, NULL, {}, _color)
	{}


	virtual void draw() override;
};
*/




void Engine (MainManager* manager);

void RECTangle (const Rect rect, HDC dc = txDC ());
void shiftArrBack    (char arr[], int length);
void shiftArrForward (char arr[], int length);
void printfDCS (const char *str = "");
bool swapDC(HDC dest, int xDest, int yDest, int wDest, int hDest,
    HDC src, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rOp);


int main (int argc, int *argv[])
{
    PowerPoint* appData = new PowerPoint;
    CSystemSettings SystemSettings;
    appData->systemSettings = &SystemSettings;

    CToolManager ToolManager;
    appData->toolManager = &ToolManager;

    CLoadManager LoadManager(appData);
    appData->loadManager = &LoadManager;
    
    appData->currColor = &SystemSettings.DrawColor;

    setDefaultSystemSettings(appData->systemSettings);
    
    appData->systemSettings->read("Settings\\FullSettings.settings");
    setSystemSettings(appData->systemSettings, "Settings\\Settings.txt");  
    setDynamicSystemSettings(appData->systemSettings);


    _txWindowStyle = appData->systemSettings->WindowStyle;
    _txSwapBuffers = swapDC;
    appData->systemSettings->MAINWINDOW = txCreateWindow (appData->systemSettings->FullSizeOfScreen.x, appData->systemSettings->FullSizeOfScreen.y);

    appData->changeWindow(appData->systemSettings->SizeOfScreen, appData->systemSettings->ScreenPos);
    appData->setResized(false);

    MainManager* manager = new MainManager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen }, 20);

    ToolSave toolSave = {};

    CanvasManager* canvasManager = new CanvasManager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen });
    appData->canvasManager = canvasManager;
	manager->addWindow (canvasManager);

    Handle* mainhandle = new Handle(appData, { .pos = {0, 0}, .finishPos = {appData->systemSettings->SizeOfScreen.x, appData->systemSettings->HANDLEHEIGHT} });
    manager->addWindow(mainhandle);


    if (appData->systemSettings->debugMode >= 0) printf("Инструменты начали загружаться\n");
    DLLToolsManager* dllToolsManager = new DLLToolsManager(appData, "Settings\\DLLPathList.txt");
    dllToolsManager->loadLibs();
    dllToolsManager->addToManager(&ToolManager);
    if (appData->systemSettings->debugMode >= 0) printf("Инструменты загрузились\n");
    

	ToolsPalette *toolsPallette = new ToolsPalette(appData, {.pos = {0, 100}, .finishPos = {50, (double)ToolManager.currentLength * 50 + appData->systemSettings->HANDLEHEIGHT + 100}}, ToolManager.currentLength);
    manager->addWindow(toolsPallette);

    ToolMenu* toolMenu = new ToolMenu(appData, canvasManager, &LoadManager);
    manager->addWindow(toolMenu);   


	ColorMenu *menu = new ColorMenu(appData, {appData->systemSettings->SizeOfScreen.x - 712, 300}, true);
	manager->addWindow (menu);

    DLLFiltersManager* dllManager = new DLLFiltersManager(appData, "Settings\\DLLPathList.txt");
    dllManager->loadLibs ();
    dllManager->addToManager(manager);
    if (appData->systemSettings->debugMode >= 0) printf("Фильтры загрузились\n");


    LaysMenu* laysMenu = new LaysMenu (appData, {.pos = {0, 500}, .finishPos = {appData->systemSettings->BUTTONWIDTH, 800}}, canvasManager);
    manager->addWindow(laysMenu);

    //Curves *curves = new Curves ({.pos = {500, 500}, .finishPos = {500 + 443, 500 + 360}}, LoadManager.loadImage("Brightness.bmp"));
    //manager->addWindow(curves);

    mainhandle;

		CloseButton* closeButton = new CloseButton(appData, LoadManager.loadImage("CloseButton4.bmp"));
		mainhandle->addWindowToBack(closeButton);

        ResizeButton* resizeButton = new ResizeButton(appData);
        mainhandle->addWindowToBack(resizeButton);

        MinimizeWindow* minimizeButton = new MinimizeWindow(appData);
        mainhandle->addWindowToBack(minimizeButton);

        AddCanvasButton* addNewCanvas = new AddCanvasButton(appData, LoadManager.loadImage ("AddNewCanvas2.bmp"), canvasManager);
		mainhandle->addWindowToStart(addNewCanvas);

        OpenManager* openWindowsManager = new OpenManager(appData, {}, TX_WHITE, NULL, LoadManager.loadImage("OpenWindows.bmp"));
        mainhandle->addWindowToStart(openWindowsManager);
        List* openWindows = new List(appData, { openWindowsManager->rect.pos.x, openWindowsManager->rect.finishPos.y }, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT }, 5);
        openWindowsManager->setOpeningManager(openWindows);
    manager->addWindow(openWindows);
        
        openWindows->addNewItem (menu, NULL, "Цвет");
        openWindows->addNewItem (toolsPallette, NULL, "Инструменты");
        openWindows->addNewItem (laysMenu, NULL, "Слои");
        openWindows->addNewItem (toolMenu, NULL, "Инструментальные слои");
        List* filters = openWindows->addSubList("Фильтры");
    manager->addWindow (filters);
            //filters->addNewItem (curves, NULL, "Кривые");
            for (int i = 0; i < dllManager->currLoadWindowNum; i++)
            {
                filters->addNewItem(dllManager->dllWindows[i], NULL, dllManager->dllWindows[i]->name);
            }

        OpenManager* openSystemList = new OpenManager(appData, {}, TX_WHITE, NULL, LoadManager.loadImage("SettingsIcon.bmp"));
        mainhandle->addWindowToStart(openSystemList);
        List* systemList = new List(appData, { openSystemList->rect.pos.x, openSystemList->rect.finishPos.y }, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT }, 1);
        openSystemList->setOpeningManager(systemList);
    manager->addWindow(systemList);
            SaveImages* saveImages = new SaveImages(appData, canvasManager);
            systemList->addNewItem(saveImages, NULL, "Сохранить изображение");
        
        
            
	

    if (appData->systemSettings->debugMode == 6) _getch();



	txBegin ();
	Engine (manager);
    saveSystemSettings(appData->systemSettings, "Settings\\Settings.txt");
    appData->systemSettings->save("Settings\\FullSettings.settings");
    delete manager;
	txEnd ();
    txDisableAutoPause();
    
	return 0;
}
     



LRESULT CALLBACK CtrlWindowFunc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_SIZING)
    {
        
    }
    return 0;
}



/*
void StatusBar::draw()
{
	app->setColor(color, finalDC);
	app->rectangle(0, 0, rect.getSize().x, rect.getSize().y, finalDC);

	if (timeButton)timeButton->draw();
	
	progressBar->draw();

	int currentNum = NULL;
	if (progressBar->currentNum)
	{
		currentNum = *progressBar->currentNum;
	}

	if (currentNum)app->bitBlt(finalDC, 0, 0, rect.getSize().x, rect.getSize().y, progressBar->finalDC);

	char text[32] = {};
	app->setColor(TX_WHITE, finalDC);
	app->selectFont("Arial", app->systemSettings->MainFont, finalDC);
	if (currentNum != 0)
	{
		sprintf(text, "Статус: применяется фильтр");
	}

	if (currentNum == 0)
	{
		sprintf(text, "Статус: фоновых задач нет");
	}
	app->setAlign(TA_LEFT, finalDC);
	app->drawText(10, 0, getSize().x, getSize().y, text, finalDC, DT_LEFT | DT_VCENTER);


	if (timeButton) app->bitBlt (finalDC, timeButton->rect.pos.x, timeButton->rect.pos.y, timeButton->getSize().x, timeButton->getSize().y, timeButton->finalDC);

}
*/




void txSelectFontDC(const char* text, int sizey, HDC &dc, int sizex/* = -1*/)
{
	txSelectFont(text, sizey, sizex, FW_DONTCARE, false, false, false, 0, dc);
}

/*
void StringButton::draw ()
{
	$s;
	bool switched = false;
	bool isShifted = false;
	bool deleteChar = false;


	if (manager->getActiveWindow () == this)
	{
		if (checkDeltaTime (lastTimeClicked))
		{
			if (app->getAsyncKeyState(VK_LEFT) && cursorPosition >= 0)
			{
				cursorMovement (VK_LEFT);
				//printf ("left");
				switched = true;
			}
			if (app->getAsyncKeyState(VK_RIGHT) && cursorPosition <= textLen - 2)
			{
				cursorMovement (VK_RIGHT);
				switched = true;
			}

			if (app->getAsyncKeyState (VK_BACK))
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
	}

	//txSetTextAlign (TA_LEFT);
	//app->setColor (TX_WHITE);
	
	//txTextOut (rect.pos.x, rect.pos.y, inText);

	if (manager->getActiveWindow () == this) shiftArrBack (&inText[cursorPosition + 1], 10);

	if (onlyNums)
	{
		double num;
		if (!strcmp ("", inText)) num = 0;

		sscanf  (inText, "%lf", &num);
		if (num > MaxNum) 
		{
			if ( ((int) log10 (num)) - ((int) log10 (MaxNum)) >= 1) 
			{
				cursorPosition--;
			}
			sprintf (inText, "%d", (int) MaxNum);
		}
	}

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
    const int DELTACLOCKTIME = 100;
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

    /*
	if (validMovement && clock () - lastTimeClicked > _app->systemSettings->DELTACLOCKTIME)
	{
		lastTimeClicked = clock();
		//shiftArrBack(&inText[cursorPosition + 1], textLen - cursorPosition);
		if (side == VK_RIGHT) cursorPosition++;
		if (side == VK_LEFT) cursorPosition--;
		shiftArrForward(&inText[cursorPosition + 1], textLen - cursorPosition);
	}
    
}
*/

void shiftArrForward (char arr[], int length)
{
    assert (arr);
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

    assert (arr);
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


void Engine (MainManager *manager)
{
    assert (manager); 
    AbstractAppData* app = manager->app;
    assert(app);

    bool wasResizedInLastFrame = false;

	for (;;)
	{
        txClearConsole();
        if (app->systemSettings->debugMode == -1 || app->systemSettings->debugMode > 0) printf ("\nFPS: %d\n", (int)txGetFPS());

		app->setColor (app->systemSettings->BackgroundColor, txDC());
		app->rectangle (0, 0, app->systemSettings->FullSizeOfScreen.x, app->systemSettings->FullSizeOfScreen.y, txDC());

        Vector mp = {txMouseX (), txMouseY ()};
        manager->mousePos = mp;
        if (app->systemSettings->debugMode > 0) printf("Engine getMBCondition(): %d\n", txMouseButtons());
        if (app->systemSettings->debugMode > 0) printf("Engine mp: {%lf, %lf}\n", mp.x, mp.y);

		manager->draw ();
		if (manager->finalDC) app->bitBlt (txDC(), manager->rect.pos.x, manager->rect.pos.x, 0, 0, manager->finalDC);

        if (wasResizedInLastFrame) app->setResized(false);
        wasResizedInLastFrame = app->wasResized();


		if (txMouseButtons ())
		{
            manager->clicked = txMouseButtons();
			manager->onClick (mp);
			if (!app->IsRunning) break;
		}
        else
        {
            manager->clicked = 0;
        }
		txSleep (0);
	}

    ShowWindow(app->systemSettings->MAINWINDOW, SW_HIDE);

    app->loadManager->deleteAllImages();
}


bool swapDC(HDC dest, int xDest, int yDest, int wDest, int hDest,
    HDC src, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rOp)
{
    return txBitBlt(dest, 0, 0, wDest, hDest, src);
}



void RECTangle (const Rect rect, HDC dc /* = txDc ()*/)
{
    assert (dc);
	//app->rectangle (rect.pos.x, rect.pos.y, rect.finishPos.x, rect.finishPos.y, dc);
}

/*
void TimeButton::draw ()
{
	if (manager)app->setColor (manager->color, finalDC);
	app->rectangle(0, 0, getSize().x, getSize().y, finalDC);
	time_t t = time (NULL);
	t = t % (24 * 3600);

	char newStr[50] = {};
	int hours =	t / 3600;
	int minutes = t / 60 - hours * 60;
	int second  = t - hours * 3600 - minutes * 60;

	sprintf (newStr, "%d:%02d:%02d", hours + 3, minutes, second);

	app->setAlign (TA_LEFT, finalDC);
	app->setColor (color, finalDC);
    app->selectFont("Arial", font, finalDC);
	app->drawText (0, 0, getSize().x, getSize().y, newStr, finalDC);
	//app->drawOnScreen(finalDC);

}
*/


void ProgressBar::setProgress(double* total, double* current)
{	
	totalNum = total;
	currentNum = current;

};


void ProgressBar::draw()
{
	$s
	if(manager) app->setColor(manager->color, finalDC);
	app->rectangle (0, 0, rect.getSize().x, rect.getSize().y, finalDC);

	app->setColor(color, finalDC);
	if (totalNum && currentNum)
	{
		app->rectangle(0, 0, rect.getSize().x * (*currentNum / *totalNum), rect.getSize().y, finalDC);
	}
	//app->drawOnScreen(finalDC);
} 

int min(int a, int b)
{
    if (a > b) return b;
    else return a;
}

void bitBlt (RGBQUAD *dest, int x, int y, int sizeX, int sizeY, RGBQUAD *source, int originalSizeX, int originalSizeY, int sourceSizeX, int sourceSizeY)
{
	for (int i = 0; i < min (sizeX, sourceSizeX); i++)
	{
		for (int j = 0; j < min (sizeY, sourceSizeY); j++)
		{	
			if (j + y < originalSizeY && i + x < originalSizeX && j + y >= 0 && i + x >= 0)
			{
				//if ( j == 249) _getch ();
				RGBQUAD *destCopy = &dest[i + (originalSizeY - (j + y) - 1) * originalSizeX + x];
				RGBQUAD *sourceCopy = &source[i + (sourceSizeY - j - 1) * sourceSizeX];
				
				sourceCopy->rgbReserved= 255;

				//if (sourceCopy->rgbReserved != 255) _getch ();

				destCopy->rgbRed = sourceCopy->rgbRed * ((float) sourceCopy->rgbReserved / 255.0);
				destCopy->rgbGreen = sourceCopy->rgbGreen * ((float)sourceCopy->rgbReserved / 255.0);
				destCopy->rgbBlue = sourceCopy->rgbBlue * ((float)sourceCopy->rgbReserved / 255.0);
				destCopy->rgbReserved = 255;

				//txSetPixel (i, j, RGB (sourceCopy->rgbRed, sourceCopy->rgbGreen, sourceCopy->rgbBlue));
				//txSetPixel (i, j, RGB (dest[i + (sizeY - (j + y) - 1) * sizeX + x].rgbRed, dest[i + (sizeY - (j + y) - 1) * sizeX + x].rgbGreen, dest[i + (sizeY - (j + y) - 1) * sizeX + x].rgbBlue));

				continue;
			}

		}
	}
}