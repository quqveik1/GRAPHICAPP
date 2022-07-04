#pragma once
#include "Menu.cpp"





struct ToolsPalette : Menu
{

    ToolsPalette(AbstractAppData* _app, Rect _rect, int _length) :
        Menu(_app, _rect, { .pos = {0, 0}, .finishPos = {(double)_app->systemSettings->DCMAXSIZE, (double)_app->systemSettings->HANDLEHEIGHT} }, _length, true)
    {
        Window** tools = new Window* [app->toolManager->currentLength];
        for (int i = 0; i < app->toolManager->currentLength; i++)
        {
            tools[i] = new Window(app);
        }

        for (int i = 0; i < app->toolManager->currentLength; i++)
        {
            tools[i]->rect = { .pos = {0, (double)i * 50}, .finishPos = {50, (double)(i + 1) * 50} };
            tools[i]->dc = app->toolManager->tools[i]->getDC();
            tools[i]->finalDC = app->createDIBSection(tools[i]->getSize().x, tools[i]->getSize().y);
            tools[i]->originalRect = tools[i]->rect;
            addWindow(tools[i]);
            currentSize++;
        }

    }

    virtual void drawOneLine(int lineNum);
    virtual int  onClickLine(Vector mp);
};




//Меню, которое будет позволять управлять уже нарисованными на слое инструментами
struct ToolMenu : Menu
{
    struct CanvasManager* canvasManager = NULL;
    HDC emptyToolDC = NULL;

    ToolMenu(AbstractAppData* _app, CanvasManager* _canvasManager, CLoadManager* _loadManager) :
        Menu(_app, { .pos = {_app->systemSettings->SizeOfScreen.x - 300, 300}, .finishPos = {_app->systemSettings->SizeOfScreen.x, _app->systemSettings->ONELAYTOOLSLIMIT * _app->systemSettings->BUTTONHEIGHT} }, { .pos = {0, 0}, .finishPos = {_app->systemSettings->DCVECTORSIZE.x, _app->systemSettings->HANDLEHEIGHT} }, _app->systemSettings->ONELAYTOOLSLIMIT, true),
        canvasManager(_canvasManager)
    {
        loadManager = _loadManager;
        emptyToolDC = loadManager->loadImage("addToolButton2.bmp");

        app->setColor(color, finalDC);
        app->rectangle(0, 0, rect.finishPos.x, rect.finishPos.y, finalDC);
        font = _app->systemSettings->MainFont * 1.5;
        app->selectFont(_app->systemSettings->FONTNAME, font, finalDC);
    }


    virtual void onUpdate();
    virtual void drawOneLine(int lineNum);
    virtual int  onClickLine(Vector mp);

};
