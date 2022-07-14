#pragma once
#include "DrawBibliothek.h"
#include "InputButton.cpp"
#include "StringButton2.cpp"


struct SetCanvasButton : Manager
{
    Vector* controlSize = NULL;
    Vector size = { 300, 200 };
    Vector oneLineSize = { 250, 25 };
    Vector inputButtonSize = { 50, 25 };
    Rect sizeXText = {};
    Rect sizeYText = {};
    double deltaYFromHandle = 10;
    double deltaBetweenSizes = 5;
    int sizeX = 0;
    int sizeY = 0;
    int minSize = 0;
    int maxSize = 1000;
    double downSectionPosY = 0;
    Vector buttonPos = {};
    Vector buttonSize = { app->systemSettings->BUTTONWIDTH + 20, 25 };
    double deltaBetweenButtons = 10;
    Rect confirmButton = {};
    Rect cancelButton = {};

    InputButton inputX;
    InputButton inputY;
    char canvasName[MAX_PATH] = {};
    StringButton2 inputName;
    int enterStatus = false;

    SetCanvasButton(AbstractAppData* _app, Vector _pos, Vector* _controlSize) :
        Manager(_app, { .pos = _pos, .finishPos = {_pos.x + 300, _pos.y + 200} }, 3, false, NULL, {}, _app->systemSettings->MenuColor),
        controlSize(_controlSize),
        inputX(app, { .pos = {}, .finishPos = inputButtonSize }, &sizeX, &minSize, &maxSize, 0, app->systemSettings->TRANSPARENTCOLOR),
        inputY(app, { .pos = {}, .finishPos = inputButtonSize }, &sizeY, &minSize, &maxSize, 0, app->systemSettings->TRANSPARENTCOLOR),
        inputName(app, { .pos = {}, .finishPos = {100, inputButtonSize.y} }, canvasName, MAX_PATH, app->systemSettings->TRANSPARENTCOLOR)
    {
        assert (app);
        assert(controlSize);
        handle.resize({ .pos = {}, .finishPos = {getSize().x, app->systemSettings->HANDLEHEIGHT } });
        handle.text = "Создать холст";
        handle.font = 30;

        Vector centralizedPos = app->getCentrizedPos(oneLineSize, size);
        sizeXText = { .pos = {centralizedPos.x, handle.rect.finishPos.y + deltaYFromHandle}, .finishPos =  {centralizedPos.x + (oneLineSize.x - inputButtonSize.x), handle.rect.finishPos.y + deltaYFromHandle  + oneLineSize.y} };
        sizeYText = { .pos = {centralizedPos.x, sizeXText.finishPos.y   + deltaBetweenSizes}, .finishPos = {centralizedPos.x + (oneLineSize.x - inputButtonSize.x), sizeXText.finishPos.y   + deltaBetweenSizes + oneLineSize.y} };

        inputX.MoveWindowTo({ sizeXText.finishPos.x,  sizeXText.pos.y });
        inputY.MoveWindowTo({ sizeYText.finishPos.x,  sizeYText.pos.y });
        inputName.MoveWindow({ 10,  sizeYText.pos.y + 10 });
        //addWindow(&inputName);
        addWindow(&inputX);
        addWindow(&inputY);

        downSectionPosY = getSize().y - 40;
        buttonPos.y = downSectionPosY + (((getSize().y - downSectionPosY) - buttonSize.y)) * 0.5;
        confirmButton = { .pos = {(getSize().x - deltaBetweenButtons) * 0.5 - buttonSize.x, buttonPos.y}, .finishPos = {(getSize().x - deltaBetweenButtons) * 0.5, buttonPos.y + buttonSize.y} };
        cancelButton = { .pos = {(getSize().x + deltaBetweenButtons) * 0.5, buttonPos.y}, .finishPos = {(getSize().x + deltaBetweenButtons) * 0.5 + buttonSize.x, buttonPos.y + buttonSize.y} };
    }


    void confirmEnter();
    void cancelEnter();

    virtual int isResultEntered();

    virtual void show();

    virtual void draw() override;
    virtual void onClick(Vector mp) override;
   
};