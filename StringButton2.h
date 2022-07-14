#pragma once
#include "WindowsLib.cpp"

struct StringButton2 : Manager
{
    char* text = NULL;
    int currentTextSize = 0;
    int maxTextSize = 0;
    char* textBeforeRedacting = NULL;

    int cursorPos = 0;
    int lastTimeClicked = 0;
    int delta = 300;
    int lastTimeCursorConditionChanged = 0;
    bool wasClicked = false;
    bool shouldShowCursor = false;
    double fontSizeX = 0;
    double spaceBetween2Symbols = 0;

    int inputMode = 0;

    HCURSOR cursor = NULL;

    double deltaAfterCadre = 4;
    COLORREF cadreColor = NULL;
    COLORREF cursorColor = NULL;

    StringButton2(AbstractAppData* _app, Rect _rect, char* _text, int _maxTextSize, COLORREF _mainColor, COLORREF _cadreColor = RGB(144, 144, 144), COLORREF _cursorColor = RGB(200, 200, 200)) :
        Manager(_app, _rect, 1, true, NULL, {}, _mainColor),
        text(_text),
        maxTextSize(_maxTextSize),
        cadreColor (_cadreColor),
        cursorColor (_cursorColor)
    {
        
        needTransparencyOutput = true;

        font = rect.getSize().y - 5;
        fontSizeX = font / 3;
        spaceBetween2Symbols = fontSizeX * 0.4;

        cursor = LoadCursor(NULL, IDC_IBEAM);
    }

    void checkKeyboard();
    void backSpace();
    void moveCursorLeft();
    void moveCursorRight();
    void drawCursor();
    bool isAnyNormalKeyButtonClicked();
    int findClickedKey();

    void shiftTextForward(char* _text, int startPos, int finishPos);
    void shiftTextBack(char* _text, int startPos, int finishPos);

    int getTextSize(char* _text);
    int& getInputMode() { return inputMode; };

    virtual bool isSymbolAllowed(int symbol);
    virtual void modifyOutput(char* outputStr, char* originalStr);
    virtual void confirmEnter() {};
    virtual void doBeforeMainBlock() {};
    virtual void doAfterMainBlock() {};



    virtual void draw() override;
    virtual void onClick(Vector mp) override;


};