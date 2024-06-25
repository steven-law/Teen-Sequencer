#include "cursor.h"

void Cursor::update(int X, int Y, int dX, int dY)
{
    xPos = X;
    yPos = Y;
    deltaX = dX;
    deltaY = dY;
    showCoordinates();
    move();
    // tft->updateScreenAsync();
}
void Cursor::move()
{
    if (xPos != last_xPos || yPos != last_yPos)
    {
        int arranger_offset = 0;
        if (deltaY == TRACK_FRAME_H)
            if (yPos >= 1)
                arranger_offset = 4;
        for (int pixel = 0; pixel < 16; pixel++)
        {
            tft->drawPixel(pixel + (deltaX * last_xPos), (deltaY * last_yPos) + 1 + arranger_offset, tftRAM[0][pixel]);  // draw upper line X1
            tft->drawPixel(pixel + (deltaX * last_xPos), (deltaY * last_yPos) + 15 + arranger_offset, tftRAM[1][pixel]); // draw bottom line X2
            tft->drawPixel((deltaX * last_xPos) + 1, pixel + (deltaY * last_yPos) + arranger_offset, tftRAM[2][pixel]);  // draw left line Y1
            tft->drawPixel((deltaX * last_xPos) + 15, pixel + (deltaY * last_yPos) + arranger_offset, tftRAM[3][pixel]); // draw right line Y2
        }
        for (int pixel = 0; pixel < 16; pixel++)
        {
            tftRAM[0][pixel] = tft->readPixel(pixel + (deltaX * xPos), (deltaY * yPos) + 1 + arranger_offset);  // save upper line
            tftRAM[1][pixel] = tft->readPixel(pixel + (deltaX * xPos), (deltaY * yPos) + 15 + arranger_offset); // save bottom line
            tftRAM[2][pixel] = tft->readPixel((deltaX * xPos) + 1, pixel + (deltaY * yPos) + arranger_offset);  // save left line
            tftRAM[3][pixel] = tft->readPixel((deltaX * xPos) + 15, pixel + (deltaY * yPos) + arranger_offset); // save right line
        }

        tft->drawRect((deltaX * xPos) + 1, (deltaY * yPos) + 1 + arranger_offset, STEP_FRAME_W - 1, STEP_FRAME_H - 1, ILI9341_WHITE);

        last_xPos = xPos;
        last_yPos = yPos;
    }
}
void Cursor::showCoordinates()
{
    tft->fillRect(20, 0, 60, 13, ILI9341_DARKGREY); // Xmin, Ymin, Xlength, Ylength, color
    tft->setTextColor(ILI9341_GREEN);
    tft->setFont(Arial_10);
    tft->setCursor(20, 3);
    tft->print("X");
    tft->print(xPos);
    tft->setCursor(55, 3);
    tft->print("Y");
    tft->print(yPos);
}
Cursor cursor(&tft);
