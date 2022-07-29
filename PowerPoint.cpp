#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TXLib.cpp" 
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
#include "SetCanvasButton.cpp"
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
#include "ImportImage.cpp"
#include "OpenImage.cpp"



void Engine (MainManager* manager);



int main (int argc, int *argv[])
{
    appData = new PowerPoint;

     
    /*
    if (OpenClipboard(NULL))//открываем буфер обмена
    {
        //извлекаем данные из буфера обмена
        HBITMAP handle = (HBITMAP)GetClipboardData(CF_BITMAP);
        if (handle)
        {
            HDC _dc = appData->createDIBSection(appData->getHBITMAPSize(handle));
            HGDIOBJ result = SelectObject(_dc, handle);

            CloseClipboard();//закрываем буфер обмена
            appData->drawOnScreen(_dc);
        }
    }
    */
    /*
    RGBQUAD* quad = NULL;
    HDC dc = appData->getBufferDC(&quad);
    _getch();
    */
    /*

    
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    int w = 500;
    int h = 500;
    HDC dc = appData->createDIBSection(500, 500);
    HBITMAP hBitmap = CreateCompatibleBitmap(dc, w, h);
    HGDIOBJ old_obj = SelectObject(dc, hBitmap);

    Gdiplus::Bitmap bitmap(hBitmap, NULL);
    CLSID clsid;


    _getch();
    

    HDC _dc = appData->createDIBSection(500, 500);       
    HBITMAP _bitmap = (HBITMAP)GetCurrentObject(_dc, OBJ_BITMAP);
    appData->setColor(TX_RED, _dc);
    appData->rectangle(0, 0, 1000, 1000, _dc);
    appData->drawOnScreen(_dc);
    _getch();
    */

    /*
    HMODULE _lib = LoadLibrary("SaveImage.dll");
    HDC (*loadImage)(const char* path, Vector & size) = (HDC(*) (const char* path, Vector & size)) GetProcAddress(_lib, "loadImage");
    Vector size = {};
    HDC _dc = loadImage("Холст.bmp", size);
    appData->drawOnScreen(_dc);
    _getch();
    HDC dc = appData->createDIBSection(size);
    appData->bitBlt(dc, 0, 0, 0, 0, _dc);
    appData->drawOnScreen(dc);
    _getch();
    */

    //saveImage = (int (*) (HDC dc, const char* path))GetProcAddress(_lib, "saveImage");

    Handle* mainhandle = new Handle(appData, { .pos = {0, 0}, .finishPos = {appData->systemSettings->FullSizeOfScreen.x, appData->systemSettings->HANDLEHEIGHT} });

    MainManager* manager = new MainManager(appData, { .pos = {0, 0}, .finishPos = appData->systemSettings->FullSizeOfScreen }, 21, mainhandle);

    manager->addWindow(mainhandle);

    CanvasManager* canvasManager = new CanvasManager(appData, {0, mainhandle->rect.finishPos.y});
    appData->canvasManager = canvasManager;
	manager->addWindow (canvasManager);


    if (appData->systemSettings->debugMode >= 0) printf("Инструменты начали загружаться\n");
    DLLToolsManager* dllToolsManager = new DLLToolsManager(appData, "Settings\\DLLPathList.txt");
    dllToolsManager->loadLibs();
    dllToolsManager->addToManager(appData->toolManager);
    if (appData->systemSettings->debugMode >= 0) printf("Инструменты загрузились\n");
    

	ToolsPalette *toolsPallette = new ToolsPalette(appData, {.pos = {5, 100}, .finishPos = {appData->systemSettings->BUTTONWIDTH + 5, (double)appData->toolManager->currentLength * 50 + appData->systemSettings->HANDLEHEIGHT + 100}}, appData->toolManager->currentLength);
    manager->addWindow(toolsPallette);

    ToolMenu* toolMenu = new ToolMenu(appData, canvasManager);
    manager->addWindow(toolMenu);   

    LaysMenu* laysMenu = new LaysMenu(appData, { .pos = {5, 600}, .finishPos = {appData->systemSettings->BUTTONWIDTH * 2, 800} }, canvasManager);
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
    if (appData->systemSettings->debugMode >= 0) printf("Фильтры загрузились\n");
    */


    mainhandle;

		CloseButton* closeButton = new CloseButton(appData);
		mainhandle->addWindowToBack(closeButton);

        ResizeButton* resizeButton = new ResizeButton(appData);
        mainhandle->addWindowToBack(resizeButton);

        MinimizeWindow* minimizeButton = new MinimizeWindow(appData);
        mainhandle->addWindowToBack(minimizeButton);

        List* createList = mainhandle->createMenuOption("Создать", NULL, true);
    
        SetCanvasButton setCanvasButton(appData, canvasManager);
        createList->addNewItem(&setCanvasButton, NULL, "Создать холст", NULL, 'N');
        createList->addNewItem(NULL, NULL, "Создать слой", &(laysMenu->needToCreateLay), 'Q');
        manager->addWindow(&setCanvasButton);
    manager->addWindow(createList);

        List* openWindows = mainhandle->createMenuOption("Окна", NULL);;
    
            openWindows->addNewItem (menu, NULL, "Цвета");
            openWindows->addNewItem(thicknessButton, NULL, "Толщина");
            openWindows->addNewItem (toolsPallette, NULL, "Инструменты");
            openWindows->addNewItem (laysMenu, NULL, "Слои");
            openWindows->addNewItem (toolMenu, NULL, "Инструменты на слое");
        manager->addWindow(openWindows);
            /*
            List* filters = openWindows->addSubList("Фильтры", dllManager->currLoadWindowNum);
        manager->addWindow (filters);
                for (int i = 0; i < dllManager->currLoadWindowNum; i++)
                {
                    filters->addNewItem(dllManager->dllWindows[i], NULL, dllManager->dllWindows[i]->name);
                }
                */

        List* importList = mainhandle->createMenuOption("Импорт/Экспорт", NULL, true);
            SaveImages saveImages(appData, canvasManager);
            importList->addNewItem(&saveImages, NULL, "Сохранить изображение", NULL, 'S');
            ImportImage importImage(appData);
            importList->addNewItem(&importImage, NULL, "Загрузить изображение", NULL, 'I');
            OpenImage openImage(appData);
            importList->addNewItem(&openImage, NULL, "Открыть изображение", NULL, 'O');
        manager->addWindow(importList);

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
            if (manager->clicked)
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






