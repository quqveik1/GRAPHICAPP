#pragma once

#include "..\DrawBibliothek.h"


struct ContrastMenu : Manager
{
	Slider upSlider;
	Slider downSlider;
	double brightness = FirstFilterValue;
	double contrast = SecondFilterValue;

	Window confirmButton;
	RGBQUAD(*algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue);

	ContrastMenu(Rect _rect, Vector firstDomain, Vector secondDomain, RGBQUAD(*_algorithm)(RGBQUAD pixel, double FirstValue, double SecondValue))	:
		Manager (_rect, 3, false, NULL, { .pos = {0, 0}, .finishPos = {getSize().x, HANDLEHEIGHT} }),
		upSlider({ .pos = {10, 65}, .finishPos = {165, 80}}, &brightness, 0.3, firstDomain.x, firstDomain.y, true, true),
		downSlider({ .pos = {10, 125}, .finishPos = {165, 140} }, &contrast, 0.3, secondDomain.x, secondDomain.y, true, true),
		confirmButton({ .pos = {240, 40}, .finishPos = {325, 60} }),
		algorithm (_algorithm)
	{
		addWindow(&upSlider);
		addWindow(&downSlider);
		addWindow(&confirmButton);
		compressImage(dc, getSize(), LoadManager.loadImage("ContrastMenu.bmp"), {335, 179});
	}

	virtual void draw() override;
	virtual void onClick() override;
};