#ifndef TRELLIS_MAIN_CLASS>
#define TRELLIS_MAIN_CLASS


#include "ownLibs/Adafruit_Trellis.h"
#include "hw_stuff.h"
#include "Track.h"
#include <ownLibs/myClock.h>
//#include <FX/fx_List.h>
//#include "FX/Output.h"
extern bool change_plugin_row;
extern bool updateTFTScreen;
extern void get_infobox_background();
extern void set_infobox_background(int _DisplayTime);
extern void reset_infobox_background();
extern void set_infobox_next_line(byte _lineNumber); //_lineNumber must be bigger 0
void neotrellis_set_control_buffer(int _x, int _y, int _color);
#define TRELLIS_PADS_X_DIM 16
#define TRELLIS_PADS_Y_DIM 8
#define TRELLIS_INT_PIN 41
#define NUMTRELLIS 8
#define numKeys (NUMTRELLIS * 16)

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
#define TRELLIS_SONGMODE_PAGE_16 TRELLIS_SCREEN_ARRANGER_1 + 16

#define TRELLIS_SCREEN_STARTUP 99
#define TRELLIS_MAX_PAGES TRELLIS_SONGMODE_PAGE_16 + 1

class TrellisForMainGrid
{
    Adafruit_TrellisSet *trellis;

public:
    TrellisForMainGrid(Adafruit_TrellisSet *_grid)
    {
        trellis = _grid;
    }
    void setup();
    void assign_start_buffer(byte _lastPotRow);
    void update(byte _activePage, byte _lastpotrow);
    void read();

    byte _lastPotRow;
    byte activePage;
   const byte TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM]{0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51,
                                                         4, 5, 6, 7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55,
                                                         8, 9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59,
                                                         12, 13, 14, 15, 28, 29, 30, 31, 44, 45, 46, 47, 60, 61, 62, 63,
                                                         64, 65, 66, 67, 80, 81, 82, 83, 96, 97, 98, 99, 112, 113, 114, 115,
                                                         68, 69, 70, 71, 84, 85, 86, 87, 100, 101, 102, 103, 116, 117, 118, 119,
                                                         72, 73, 74, 75, 88, 89, 90, 91, 104, 105, 106, 107, 120, 121, 122, 123,
                                                         76, 77, 78, 79, 92, 93, 94, 95, 108, 109, 110, 111, 124, 125, 126, 127};

    bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
    int trellisTrackColor[9];

    int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
    bool trellisShowClockPixel[Y_DIM];
    byte trellisPianoTrack;
    byte trellisPerformIndex[TRELLIS_PADS_X_DIM];

    byte performCC[TRELLIS_PADS_X_DIM];

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

    void trellis_show_clockbar();

    void trellis_play_piano();
    void trellis_setStepsequencer();
    void trellis_writeDisplay();

    void trellis_set_arranger();

    void trellis_play_mixer();

    void trellis_perform();
    void draw_perform_page();
    void set_perform_page(byte row);
    void set_performCC(byte XPos, byte YPos, const char *name);
};

//extern TrellisForMainGrid *trellisMain;
#endif //TRELLIS_MAIN_CLASS