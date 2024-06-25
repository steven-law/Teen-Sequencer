

#include <Arduino.h>
#include "project_variables.h"


elapsedMicros msecsclock;

//extern Clock Masterclock;
int pixelTouchX = 0;
int gridTouchY = 0;
byte lastPotRow = 0;
byte encoder_function = 0;
bool change_plugin_row = false;

byte active_track;
byte arrangerpage;
bool otherCtrlButtons = true;

int trackColor[9]{6150246, 8256638, 1095334, 12643941, 2583100, 9365295, 12943157, 5678954, ILI9341_WHITE};
int encoder_colour[NUM_ENCODERS] = {ILI9341_BLUE, ILI9341_RED, ILI9341_GREEN, ILI9341_WHITE};
// songmode
int phraseSegmentLength = 16;
// mixer
const char *playstate[3] = {"Mute", "Play", "Solo"};
// cursor



