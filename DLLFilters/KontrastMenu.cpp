#pragma once
#include "KontrastMenu.h"


void KontrastMenu::useAlgorithm()
{
    for (int x = 0; x < activeLay->laySize.x; x++)
    {
        for (int y = 0; y < activeLay->laySize.y; y++)
        {
            int pixelPos = (int)(activeLay->laySize.y - y) * ((int)(activeLay->laySize.x - 1)) + x;
            RGBQUAD pixel = ((activeLay->layBuf))[pixelPos];
            double kOfBrightness = firstVal + 1; //коэффициент
            double bOfBrightness = secondVal;	// b графика
            //printf("%lf %lf\n", kOfBrightness, bOfBrightness);


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

            ((activeLay->tempBuf))[pixelPos] = pixel;
        }
    }
}


void KontrastMenu::apply ()
{
    txAlphaBlend(activeLay->lay, 0, 0, 0, 0, activeLay->tempLay);
}