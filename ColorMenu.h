#pragma once
#include "ColorComponentChanger.cpp" 

struct ColorHistory
{
    int HistoryLength = 11;
    COLORREF colorHistory[11] = {};
    Rect colorRect[11] = {};
    int currentPos = 0;
    int currHistoryLen = 0;

    int getByteSize();
};

struct ColorMenu : Manager
{
    ColorComponentChanger* redChanger = NULL;
    ColorComponentChanger* greenChanger = NULL;
    ColorComponentChanger* blueChanger = NULL;
    int redComponent = 0;
    int greenComponent = 0;
    int blueComponent = 0;

    
    //saveable part
    ColorHistory colorHistory;
    //saveable part
    char pathToSaveHistory[MAX_PATH] = {};
    
    Vector sizeOfColorMenu = {412, 227};
    bool confirmedColor = false;

    void loadHistory();


    ColorMenu(AbstractAppData* _app, Vector _pos, const char* _pathToHistory , bool _advancedMode = false) :
        sizeOfColorMenu ({ 412, 227 }),
        Manager(_app, { .pos = _pos, .finishPos = _pos + sizeOfColorMenu }, 3, _advancedMode, _app->loadManager->loadImage("ColorsMenu-2.bmp"), { .pos = {0, 0}, .finishPos = { 412, 50 } })
    {
        assert(app);
        assert(app->systemSettings);

        needTransparencyOutput = true;

        strcpy(pathToSaveHistory, _pathToHistory);
        loadHistory();

        if (colorHistory.currentPos - 1 < 0)
        {
            if (colorHistory.currHistoryLen == colorHistory.HistoryLength && colorHistory.colorHistory[colorHistory.HistoryLength - 1] != app->systemSettings->DrawColor)
            {
                confirmColor();
            }
            if (colorHistory.currHistoryLen == 0)
            {
                confirmColor();
            }
        }
        else if (colorHistory.colorHistory[colorHistory.currentPos - 1] != app->systemSettings->DrawColor)
        {
            confirmColor();
        }

        Rect newRect = { .pos = _pos, .finishPos = _pos + sizeOfColorMenu };
        resize(newRect);

        setColorComponents();

        redChanger = new ColorComponentChanger(app, { .pos = {75, 60}, .finishPos = {380, 80} }, &redComponent, &confirmedColor);
        addWindow(redChanger);

        greenChanger = new ColorComponentChanger(app, { .pos = {75, 85}, .finishPos = {380, 105} }, &greenComponent, &confirmedColor);
        addWindow(greenChanger);

        blueChanger = new ColorComponentChanger(app, { .pos = {75, 110}, .finishPos = {380, 130} }, &blueComponent, &confirmedColor);
        addWindow(blueChanger);
    }


    virtual void saveMenu();
    void setColorRects();
    void moveHistory(int clickedNumOFColorRect);
    virtual void confirmColor();
    void setColorComponents();
    
    
    virtual void draw() override;
    virtual void onClick(Vector mp) override;

};