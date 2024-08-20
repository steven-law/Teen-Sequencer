#ifndef CURSOR_H
#define CURSOR_H

#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "project_variables.h"

class Cursor
{
public:
    ILI9341_t3n *tft; // Pointer to the display object
    int xPos = 0;
    int yPos = 0;
    int last_xPos = 0;
    int last_yPos = 0;
    int last_button_X = 0;
    int last_button_Y = 0;
    int deltaY = STEP_FRAME_H;
    int deltaX = STEP_FRAME_W;

    uint16_t tftRAM[16][16];
    Cursor(ILI9341_t3n *display)
    {
        tft = display;
    }
    void update(int X, int Y, int dX, int dY);
    void move();
    void showCoordinates();
};
extern Cursor cursor;


#endif