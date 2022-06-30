#pragma once
#include "ColorComponentChanger.cpp" 

struct ColorMenu : Manager
{
    ColorComponentChanger* redChanger = NULL;
    ColorComponentChanger* greenChanger = NULL;
    ColorComponentChanger* blueChanger = NULL;
    int redComponent = 0;
    int greenComponent = 0;
    int blueComponent = 0;
    const int HistoryLength = 11;
    COLORREF colorHistory[11] = {};
    Rect colorRect[11] = {};
    int currentPos = 0;
    int currHistoryLen = 0;
    
    Vector sizeOfColorMenu = {412, 227};

    ColorMenu(AbstractAppData* _app, Vector _pos, bool _advancedMode = false) :
        sizeOfColorMenu ({ 412, 227 }),
        Manager(_app, { .pos = _pos, .finishPos = _pos + sizeOfColorMenu }, 3, _advancedMode, _app->loadManager->loadImage("ColorsMenu-2.bmp"), { .pos = {0, 0}, .finishPos = { 412, 50 } })
    {
        needTransparencyOutput = true;

        confirmColor();

        assert(app);
        assert(app->systemSettings);
        Rect newRect = { .pos = _pos, .finishPos = _pos + sizeOfColorMenu };
        resize(newRect);

        redComponent = app->getColorComponent (app->systemSettings->DrawColor, TX_RED);
        redChanger = new ColorComponentChanger(app, { .pos = {75, 60}, .finishPos = {380, 80} }, &redComponent);
        addWindow(redChanger);

        greenComponent = app->getColorComponent(app->systemSettings->DrawColor, TX_GREEN);
        greenChanger = new ColorComponentChanger(app, { .pos = {75, 85}, .finishPos = {380, 105} }, &greenComponent);
        addWindow(greenChanger);

        blueComponent = app->getColorComponent(app->systemSettings->DrawColor, TX_BLUE);
        blueChanger = new ColorComponentChanger(app, { .pos = {75, 110}, .finishPos = {380, 130} }, &blueComponent);
        addWindow(blueChanger);
    }


    void setColorRects();
    void moveHistory(int clickedNumOFColorRect);
    virtual void confirmColor();
    
    virtual void draw() override;
    virtual void onClick(Vector mp) override;

};