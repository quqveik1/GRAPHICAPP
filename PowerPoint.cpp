#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SystemSettings.cpp"
#include "DrawBibliothek.cpp"
#include "WindowsLib.cpp"
#include "Q_Buttons.h"
#include <cmath>
#include "StandartFunctions.h"
#include "Canvas.cpp"
#include "DLLFiltersManager.cpp"
#include "Tool.h"
#include "DLLToolsManager.cpp"
#include "CLay.cpp"
#include "MainTools.cpp"
#include "CanvasManager.cpp"
#include "ProgressBar.h"
#include "LoadManager.cpp"
#include "Lay.cpp"
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
#include "FileSavings.cpp"
#include "LaysMenu.cpp"
#include "Thickness.cpp"
#include "OpenHandleMenuManager.cpp"
#include "winuser.h"
#include "WindowsLibApi.cpp"



void Engine (MainManager* manager);
LRESULT CALLBACK CtrlWindowFunc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
void printfDCS (const char *str = "");
bool swapDC(HDC dest, int xDest, int yDest, int wDest, int hDest,
    HDC src, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rOp);
bool checkVersionCompability(PowerPoint* app);
void writeVersion(PowerPoint* app);


PowerPoint* appData = new PowerPoint;


int main (int argc, int *argv[])
{

    appData->appVersion = "v0.1.7.4";

    CSystemSettings SystemSettings;
    appData->systemSettings = &SystemSettings;

    CToolManager ToolManager;
    appData->toolManager = &ToolManager;

    CLoadManager LoadManager(appData);
    appData->loadManager = &LoadManager;

    WindowsLibApi windowsLib;
    appData->windowsLibApi = &windowsLib;

    CFileSavings FileSavings;
    FileSavings.add("Settings\\FullSettings.settings");
    FileSavings.add("Settings\\ColorHistory.history");
    appData->fileSavings = &FileSavings;
    
    appData->currColor = &SystemSettings.DrawColor;

    setDefaultSystemSettings(appData->systemSettings);

    bool needLoadFiles = checkVersionCompability(appData);
    if (!needLoadFiles) appData->fileSavings->deleteAllFiles();

    
    appData->systemSettings->read("Settings\\FullSettings.settings");
    setSystemSettings(appData->systemSettings, "Settings\\Settings.txt");  
    setDynamicSystemSettings(appData->systemSettings);


    _txWindowStyle = appData->systemSettings->WindowStyle;
    _txSwapBuffers = swapDC;
    txSetWindowsHook(CtrlWindowFunc);
    appData->systemSettings->MAINWINDOW = txCreateWindow (appData->systemSettings->FullSizeOfScreen.x, appData->systemSettings->FullSizeOfScreen.y);

    /*
    HDC testPhoto = txLoadImage("Files\\TestPhoto.bmp");

    RGBQUAD* tempBuf = NULL;

    HDC tempDC = appData->createDIBSection(appData->systemSettings->FullSizeOfScreen, &tempBuf);

    appData->bitBlt(tempDC, {}, {}, testPhoto);

    appData->horizontalReflect(tempDC, tempBuf, { 1200, 727 }, appData->systemSettings->FullSizeOfScreen);
    
    appData->drawOnScreen(tempDC);
    _getch();
    */

    appData->changeWindow(appData->systemSettings->SizeOfScreen, appData->systemSettings->ScreenPos);;
    
    MainManager* manager = new MainManager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen }, 21);

    ToolSave toolSave = {};

    CanvasManager* canvasManager = new CanvasManager(appData);
    appData->canvasManager = canvasManager;
	manager->addWindow (canvasManager);

    Handle* mainhandle = new Handle(appData, { .pos = {0, 0}, .finishPos = {appData->systemSettings->FullSizeOfScreen.x, appData->systemSettings->HANDLEHEIGHT} });
    manager->addWindow(mainhandle);


    if (appData->systemSettings->debugMode >= 0) printf("Инструменты начали загружаться\n");
    DLLToolsManager* dllToolsManager = new DLLToolsManager(appData, "Settings\\DLLPathList.txt");
    dllToolsManager->loadLibs();
    dllToolsManager->addToManager(&ToolManager);
    if (appData->systemSettings->debugMode >= 0) printf("Инструменты загрузились\n");
    

	ToolsPalette *toolsPallette = new ToolsPalette(appData, {.pos = {5, 100}, .finishPos = {appData->systemSettings->BUTTONWIDTH + 5, (double)ToolManager.currentLength * 50 + appData->systemSettings->HANDLEHEIGHT + 100}}, ToolManager.currentLength);
    manager->addWindow(toolsPallette);

    ToolMenu* toolMenu = new ToolMenu(appData, canvasManager, &LoadManager);
    manager->addWindow(toolMenu);   

    LaysMenu* laysMenu = new LaysMenu(appData, { .pos = {5, 500}, .finishPos = {appData->systemSettings->BUTTONWIDTH + 5, 800} }, canvasManager);
    manager->addWindow(laysMenu);

	ColorMenu* menu = new ColorMenu(appData, {appData->systemSettings->SizeOfScreen.x - 712, 300}, "Settings\\ColorHistory.history", true);
    menu->devName = "ColorMenu";
	manager->addWindow (menu);

    ThicknessMenu* thicknessButton = new ThicknessMenu(appData, { 300, 300 }, false);
    manager->addWindow(thicknessButton);

    DLLFiltersManager* dllManager = new DLLFiltersManager(appData, "Settings\\DLLPathList.txt");
    dllManager->loadLibs ();
    dllManager->addToManager(manager);
    if (appData->systemSettings->debugMode >= 0) printf("Фильтры загрузились\n");


    mainhandle;

		CloseButton* closeButton = new CloseButton(appData, LoadManager.loadImage("CloseButton4.bmp"));
		mainhandle->addWindowToBack(closeButton);

        ResizeButton* resizeButton = new ResizeButton(appData);
        mainhandle->addWindowToBack(resizeButton);

        MinimizeWindow* minimizeButton = new MinimizeWindow(appData);
        mainhandle->addWindowToBack(minimizeButton);

        AddCanvasButton* addNewCanvas = new AddCanvasButton(appData, LoadManager.loadImage ("AddNewCanvas2.bmp"), canvasManager);
		mainhandle->addWindowToStart(addNewCanvas);
        SetCanvasButton* setCanvasButton = addNewCanvas->getSetCanvasButton();
    manager->addWindow(setCanvasButton);

        OpenHandleMenuManager* openWindowsManager = new OpenHandleMenuManager(appData, LoadManager.loadImage("OpenWindows.bmp"));
        mainhandle->addWindowToStart(openWindowsManager);
        List* openWindows = new List(appData, { openWindowsManager->rect.pos.x, openWindowsManager->rect.finishPos.y }, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT }, 6);
        openWindowsManager->setOpeningManager(openWindows);
    manager->addWindow(openWindows);
        
            openWindows->addNewItem (menu, NULL, "Цвета");
            openWindows->addNewItem(thicknessButton, NULL, "Толщина");
            openWindows->addNewItem (toolsPallette, NULL, "Инструменты");
            openWindows->addNewItem (laysMenu, NULL, "Слои");
            openWindows->addNewItem (toolMenu, NULL, "Инструменты на слое");
            List* filters = openWindows->addSubList("Фильтры", dllManager->currLoadWindowNum);
        manager->addWindow (filters);
                for (int i = 0; i < dllManager->currLoadWindowNum; i++)
                {
                    filters->addNewItem(dllManager->dllWindows[i], NULL, dllManager->dllWindows[i]->name);
                }

        OpenHandleMenuManager* openSystemList = new OpenHandleMenuManager(appData, LoadManager.loadImage("SettingsIcon.bmp"));
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
    menu->saveMenu();
    writeVersion(appData);
    delete manager;
	txEnd ();
    txDisableAutoPause();
    
	return 0;
}
 

void writeVersion(PowerPoint* app)
{
    assert(app);
    FILE* versionFile = fopen("Settings\\Version.txt", "w");

    if (versionFile)
    {
        fprintf(versionFile, "%s", app->appVersion);
    }
    if (versionFile)fclose(versionFile);
}


bool checkVersionCompability(PowerPoint* app)
{
    assert(app);
    bool needLoadSaves = false;
    FILE* versionFile = fopen("Settings\\Version.txt", "r");
   
    if (versionFile)
    {
        char versionName[MAX_PATH] = {};
        fscanf(versionFile, "%s", versionName);
        int result = strcmp(app->appVersion, versionName);
        if (result == 0) needLoadSaves = true;
    }

    if (versionFile) fclose(versionFile);
    return needLoadSaves;
}


LRESULT CALLBACK CtrlWindowFunc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_SETCURSOR)
    {
        if (appData->activeCursor)
        {
            SetClassLongPtr(window, GCLP_HCURSOR, (LONG_PTR)appData->activeCursor);
        }
    }
    return 0;
}


void Engine (MainManager *manager)
{
    assert (manager); 
    PowerPoint* app = (PowerPoint*)manager->app;
    assert(app);

    bool wasResizedInLastFrame = false;

	for (;;)
	{
        app->controlApp();
        if (app->systemSettings->debugMode == -1 || app->systemSettings->debugMode > 0) printf ("\nFPS: %d\n", (int)txGetFPS());

		app->setColor (app->systemSettings->BackgroundColor, txDC());
		app->rectangle (0, 0, app->systemSettings->FullSizeOfScreen.x, app->systemSettings->FullSizeOfScreen.y, txDC());

        Vector mp = {txMouseX (), txMouseY ()};
        manager->mousePos = mp;
        if (app->systemSettings->debugMode > 0) printf("Engine getMBCondition(): %d\n", txMouseButtons());
        if (app->systemSettings->debugMode > 0) printf("Engine mp: {%lf, %lf}\n", mp.x, mp.y);

		manager->draw ();
		if (manager->finalDC) app->bitBlt (txDC(), manager->rect.pos.x, manager->rect.pos.x, 0, 0, manager->finalDC);

        manager->clicked = txMouseButtons();
		if (txMouseButtons ())
        {
			manager->onClick (mp);
			if (!app->IsRunning) break;
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



