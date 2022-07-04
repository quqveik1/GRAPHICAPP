#pragma once

#include "BrightnessFilter.h"

void BrightnessMenu::useAlgorithm()
{
    Vector laySize = activeCanvas->getActiveLay()->getLaySize();
    activeLay = activeCanvas->getActiveLay();

    for (int x = 0; x < laySize.x; x++)
    {
        for (int y = 0; y < laySize.y; y++)
        {
            int pixelPos = (int)(laySize.y - y) * ((int)(laySize.x - 1)) + x;
            RGBQUAD pixel = ((activeLay->getPermanentBuf()))[pixelPos];

            int copyOfFirstVal = firstVal;
            firstVal += 100;
            firstVal /= 100;
            //FirstValue += 100;
            //FirstValue /= 100;

            int red = (pixel.rgbRed) * firstVal;
            pixel.rgbRed = red;
            if (red < 0) pixel.rgbRed = 0;
            if (red > 255) pixel.rgbRed = 255;

            int green = (pixel.rgbGreen) * firstVal;
            pixel.rgbGreen = green;
            if (green < 0) pixel.rgbGreen = 0;
            if (green > 255) pixel.rgbGreen = 255;

            int blue = (pixel.rgbBlue) * firstVal;
            pixel.rgbBlue = blue;
            if (blue < 0) pixel.rgbBlue = 0;
            if (blue > 255) pixel.rgbBlue = 255;


            ((activeLay->getOutputBuf()))[pixelPos] = pixel;
            firstVal = copyOfFirstVal;
        }
    }
}

void BrightnessMenu::apply ()
{
    txBitBlt(activeLay->getPermanentDC(), 0, 0, 0, 0, activeLay->getOutputDC());
}