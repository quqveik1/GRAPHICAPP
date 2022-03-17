#pragma once

#define standartDraw                                                                    \
{                                                                                          \
    $s                                                                                     \
	if (finalDC) txSetAllColors (color, finalDC);                                           \
	if (finalDC) txRectangle (0, 0, rect.getSize ().x, rect.getSize ().y, finalDC);        \
                                                                                           \
                                                                                           \
	if (finalDC) txSetAllColors (TextColor, finalDC);                                      \
	txSetTextAlign (TA_CENTER, finalDC);                                                    \
                                                                                           \
                                                                                           \
	if (dc)                                                                                \
	{                                                                                           \
		compressDraw (finalDC, {0, 0}, rect.getSize (), dc, originalRect.getSize (), __LINE__);\
	}                                                                                          \
}                                                                                              \