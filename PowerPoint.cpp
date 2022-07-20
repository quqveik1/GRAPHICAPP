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

int main (int argc, int *argv[])
{
    appData = new PowerPoint;
    
    MainManager* manager = new MainManager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen }, 21);

    ToolSave toolSave = {};

    CanvasManager* canvasManager = new CanvasManager(appData);
    appData->canvasManager = canvasManager;
	manager->addWindow (canvasManager);

    Handle* mainhandle = new Handle(appData, { .pos = {0, 0}, .finishPos = {appData->systemSettings->FullSizeOfScreen.x, appData->systemSettings->HANDLEHEIGHT} });
    manager->addWindow(mainhandle);


    if (appData->systemSettings->debugMode >= 0) printf("����������� ������ �����������\n");
    DLLToolsManager* dllToolsManager = new DLLToolsManager(appData, "Settings\\DLLPathList.txt");
    dllToolsManager->loadLibs();
    dllToolsManager->addToManager(appData->toolManager);
    if (appData->systemSettings->debugMode >= 0) printf("����������� �����������\n");
    

	ToolsPalette *toolsPallette = new ToolsPalette(appData, {.pos = {5, 100}, .finishPos = {appData->systemSettings->BUTTONWIDTH + 5, (double)appData->toolManager->currentLength * 50 + appData->systemSettings->HANDLEHEIGHT + 100}}, appData->toolManager->currentLength);
    manager->addWindow(toolsPallette);

    ToolMenu* toolMenu = new ToolMenu(appData, canvasManager);
    manager->addWindow(toolMenu);   

    LaysMenu* laysMenu = new LaysMenu(appData, { .pos = {5, 500}, .finishPos = {appData->systemSettings->BUTTONWIDTH + 5, 800} }, canvasManager);
    manager->addWindow(laysMenu);

	ColorMenu* menu = new ColorMenu(appData, {appData->systemSettings->SizeOfScreen.x - 712, 300}, "Settings\\ColorHistory.history", true);
    menu->devName = "ColorMenu";
	manager->addWindow (menu);

    ThicknessMenu* thicknessButton = new ThicknessMenu(appData, { 300, 300 }, false);
    manager->addWindow(thicknessButton);

    /*
    DLLFiltersManager* dllManager = new DLLFiltersManager(appData, "Settings\\DLLPathList.txt");
    dllManager->loadLibs ();
    dllManager->addToManager(manager);
    if (appData->systemSettings->debugMode >= 0) printf("������� �����������\n");
    */


    mainhandle;

		CloseButton* closeButton = new CloseButton(appData, appData->loadManager->loadImage("CloseButton4.bmp"));
		mainhandle->addWindowToBack(closeButton);

        ResizeButton* resizeButton = new ResizeButton(appData);
        mainhandle->addWindowToBack(resizeButton);

        MinimizeWindow* minimizeButton = new MinimizeWindow(appData);
        mainhandle->addWindowToBack(minimizeButton);

        AddCanvasButton* addNewCanvas = new AddCanvasButton(appData, appData->loadManager->loadImage ("AddNewCanvas2.bmp"), canvasManager);
		mainhandle->addWindowToStart(addNewCanvas);
        SetCanvasButton* setCanvasButton = addNewCanvas->getSetCanvasButton();
    manager->addWindow(setCanvasButton);

        OpenHandleMenuManager* openWindowsManager = new OpenHandleMenuManager(appData, appData->loadManager->loadImage("OpenWindows.bmp"));
        mainhandle->addWindowToStart(openWindowsManager);
        List* openWindows = new List(appData, { openWindowsManager->rect.pos.x, openWindowsManager->rect.finishPos.y }, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT }, 6);
        openWindowsManager->setOpeningManager(openWindows);
    manager->addWindow(openWindows);
        
            openWindows->addNewItem (menu, NULL, "�����");
            openWindows->addNewItem(thicknessButton, NULL, "�������");
            openWindows->addNewItem (toolsPallette, NULL, "�����������");
            openWindows->addNewItem (laysMenu, NULL, "����");
            openWindows->addNewItem (toolMenu, NULL, "����������� �� ����");
            /*
            List* filters = openWindows->addSubList("�������", dllManager->currLoadWindowNum);
        manager->addWindow (filters);
                for (int i = 0; i < dllManager->currLoadWindowNum; i++)
                {
                    filters->addNewItem(dllManager->dllWindows[i], NULL, dllManager->dllWindows[i]->name);
                }
                */

        OpenHandleMenuManager* openSystemList = new OpenHandleMenuManager(appData, appData->loadManager->loadImage("SettingsIcon.bmp"));
        mainhandle->addWindowToStart(openSystemList);
        List* systemList = new List(appData, { openSystemList->rect.pos.x, openSystemList->rect.finishPos.y }, { appData->systemSettings->BUTTONWIDTH * 5, appData->systemSettings->HANDLEHEIGHT }, 1);
        openSystemList->setOpeningManager(systemList);
    manager->addWindow(systemList);
            SaveImages* saveImages = new SaveImages(appData, canvasManager);
            systemList->addNewItem(saveImages, NULL, "��������� �����������");

	txBegin ();
	Engine (manager);

    menu->saveMenu();
    delete manager;
    delete appData;
	txEnd ();
    txDisableAutoPause();
    
	return 0;
}


void Engine (MainManager *manager)
{
    assert (manager); 
    PowerPoint* app = (PowerPoint*)manager->app;
    assert(app);
    RECT programmRect = {};
    bool deltaBetween2UnhideSuggestions = 1000;
    double lastTimeChekedWindowCondition = 0;

    bool wasResizedInLastFrame = false;

    for (;;)
    {
        app->controlApp();
        if (app->systemSettings->debugMode == -1 || app->systemSettings->debugMode > 0) printf("\nFPS: %d\n", (int)txGetFPS());

        app->setColor(app->systemSettings->BackgroundColor, txDC());
        app->rectangle(0, 0, app->systemSettings->FullSizeOfScreen.x, app->systemSettings->FullSizeOfScreen.y, txDC());

        Vector mp = { txMouseX(), txMouseY() };
        manager->mousePos = mp;
        if (app->systemSettings->debugMode > 0) printf("Engine getMBCondition(): %d\n", txMouseButtons());
        if (app->systemSettings->debugMode > 0) printf("Engine mp: {%lf, %lf}\n", mp.x, mp.y);

        if (!app->isShowing)
        {
            GetWindowRect(app->MAINWINDOW, &programmRect);
            if (isBigger(programmRect.right, 0))
            {
                app->isShowing = true;
            }
        }
        if (app->isShowing)
        {

            manager->draw();
            if (manager->finalDC) app->bitBlt(txDC(), manager->rect.pos.x, manager->rect.pos.x, 0, 0, manager->finalDC);

            manager->clicked = txMouseButtons();
            if (txMouseButtons())
            {
                manager->onClick(mp);
            }
            txSleep(0);
        }

        if (!app->IsRunning)
        {
            break;
        }
	}

    ShowWindow(app->MAINWINDOW, SW_HIDE);
}






