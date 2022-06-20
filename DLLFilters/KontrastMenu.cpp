#pragma once
#include "KontrastMenu.h"


void KontrastMenu::useAlgorithm()
{
    Vector laySize = activeCanvas->getActiveLay()->getLaySize();
    activeLay = activeCanvas->getActiveLay();
    for (int x = 0; x < laySize.x; x++)
    {
        for (int y = 0; y < laySize.y; y++)
        {
            int pixelPos = (int)(laySize.y - y) * ((int)(laySize.x - 1)) + x;
            RGBQUAD pixel = ((activeLay->getPermanentBuf()))[pixelPos];

            if (   pixel.rgbRed == txExtractColor(systemSettings->TRANSPARENTCOLOR, TX_RED)
                && pixel.rgbGreen == txExtractColor(systemSettings->TRANSPARENTCOLOR, TX_GREEN)
                && pixel.rgbBlue == txExtractColor(systemSettings->TRANSPARENTCOLOR, TX_BLUE))
            {
                pixel = { 0, 0, 0, 0 };
            }

            double kOfBrightness = firstVal + 1; //коэффициент
            double bOfBrightness = secondVal;	// b графика
            


            int red = (pixel.rgbRed - 128) * kOfBrightness + bOfBrightness + 128;
            pixel.rgbRed = red;
            if (red < 0) pixel.rgbRed = 0;
            if (red > 255) pixel.rgbRed = 255;

            int green = (pixel.rgbGreen - 128) * kOfBrightness + bOfBrightness + 128;
            pixel.rgbGreen = green;
            if (green < 0) pixel.rgbGreen = 0;
            if (green > 255) pixel.rgbGreen = 255;

            int blue = (pixel.rgbBlue - 128) * kOfBrightness + bOfBrightness + 128;
            pixel.rgbBlue = blue;
            if (blue < 0) pixel.rgbBlue = 0;
            if (blue > 255) pixel.rgbBlue = 255;

            ((activeLay->getOutputBuf()))[pixelPos] = pixel;
        }
    }
}


void KontrastMenu::apply ()
{
    txBitBlt(activeLay->getPermanentDC(), 0, 0, 0, 0, activeLay->getOutputDC());
    app->drawOnScreen(activeLay->getPermanentDC());
    _getch();
}