/*#ifndef TRELLIS_CLASS>
#define TRELLIS_CLASS
#include "ownLibs/Adafruit_Trellis.h"
#include "hw_stuff.h"

#define TRELLIS_PADS_X_DIM 16
#define TRELLIS_PADS_Y_DIM 8
// See https://www.w3schools.com/colors/colors_picker.asp
#define TRELLIS_RED 0xFF0000
#define TRELLIS_ORANGE 0xB34700
#define TRELLIS_YELLOW 0xFFFF00
#define TRELLIS_OLIVE 0x66DD00
#define TRELLIS_GREEN 0x008000
#define TRELLIS_AQUA 0x00FF66
#define TRELLIS_TEAL 0x00BFFF
#define TRELLIS_BLUE 0x0080FF
#define TRELLIS_NAVY 0x000080
#define TRELLIS_MAROON 0x800000
#define TRELLIS_PURPLE 0x800080
#define TRELLIS_PINK 0xFF66B3
#define TRELLIS_WHITE 0xFFFFFF
#define TRELLIS_BLACK 0x000000
//#define TRELLIS_LIGHTBLUE #ADD8E6

#define TRELLIS_1 0x000066
#define TRELLIS_2 579

#define TRELLIS_POTROW 0
#define TRELLIS_BUTTON_RECORD 1
#define TRELLIS_START_CLOCK 2
#define TRELLIS_STOP_CLOCK 3

#define TRELLIS_BUTTON_LEFT 4
#define TRELLIS_BUTTON_RIGHT 5
#define TRELLIS_BUTTON_UP 6
#define TRELLIS_BUTTON_DOWN 7

#define TRELLIS_BUTTON_ENTER 8
#define TRELLIS_BUTTON_SHIFT 9
#define TRELLIS_BUTTON_PIANO 10
#define TRELLIS_BUTTON_SEQMODE 11

#define TRELLIS_BUTTON_ARRANGER 12
#define TRELLIS_BUTTON_MIXER 13
#define TRELLIS_BUTTON_PLUGIN 14
#define TRELLIS_BUTTON_SEQUENCER 15

#define TRELLIS_SCREEN_SEQUENCER_CLIP_0 0
#define TRELLIS_SCREEN_SEQUENCER_CLIP_1 1
#define TRELLIS_SCREEN_SEQUENCER_CLIP_2 2
#define TRELLIS_SCREEN_SEQUENCER_CLIP_3 3
#define TRELLIS_SCREEN_SEQUENCER_CLIP_4 4
#define TRELLIS_SCREEN_SEQUENCER_CLIP_5 5
#define TRELLIS_SCREEN_SEQUENCER_CLIP_6 6
#define TRELLIS_SCREEN_SEQUENCER_CLIP_7 7
#define TRELLIS_SCREEN_SEQUENCER_CLIP_8 8

#define TRELLIS_SCREEN_PIANO 10
#define TRELLIS_SCREEN_SONGPAGE_SELECTION 11
#define TRELLIS_SCREEN_MIXER 12
#define TRELLIS_SCREEN_MIXER1 13
#define TRELLIS_SCREEN_PERFORM 14

#define TRELLIS_SCREEN_ARRANGER_1 15
#define TRELLIS_SCREEN_ARRANGER_2 16
#define TRELLIS_SONGMODE_PAGE_16 TRELLIS_SCREEN_ARRANGER_1+16

#define TRELLIS_SCREEN_STARTUP 99
#define TRELLIS_MAX_PAGES TRELLIS_SONGMODE_PAGE_16+1

class TrellisForMainGrid
{

public:
 byte TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
 bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
int trellisTrackColor[9];

int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
bool trellisRecall;
bool trellisShowClockPixel[Y_DIM];
byte trellisPianoTrack;
byte trellisPerformIndex[NUM_STEPS];

byte performCC[16];

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long trellisRestartPreviousMillis = 0; // will store last time LED was updated
unsigned long trellisReadPreviousMillis = 0;    // will store last time LED was updated

// constants won't change:
const long trellisReadInterval = 30;       // interval at which to blink (milliseconds)
const long trellisRestartInterval = 60000; // interval at which to blink (milliseconds)

void trellis_set_main_buffer(int _page, int _x, int _y, int color);
void trellis_recall_main_buffer(int _page);
int trellis_get_main_buffer(int _page, int _x, int _y);
void trellis_clear_main_buffer(int _page);

void trellis_setStepsequencer();
void trellis_read();
};
#endif*/