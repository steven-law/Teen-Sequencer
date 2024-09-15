#ifndef PL_FUNCTIONS_H
#define PL_FUNCTIONS_H

#include <Arduino.h>
#include "project_variables.h"
#include "Track.h"
#include "hw_stuff.h"

//#include "ownLibs/clock.h"

void encoder_SetCursor(byte deltaX, byte maxY);
void mouse(byte deltaX, byte maxY);


void clock_to_notes(int _tick);
void input_behaviour();

void trellis_set_main_buffer(int _page, int _x, int _y, int color);


#endif