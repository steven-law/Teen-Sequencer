#ifndef TFT_CLASS_H
#define TFT_CLASS_H
#include "hw_stuff.h"
#include "project_functions.h"
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "Track.h"
class Track;
class PluginControll;
// Display

// Startscreen
#define POSITION_ARR_BUTTON 18
#define POSITION_BPM_BUTTON 11
#define POSITION_SCALE_BUTTON 16
#define POSITION_LOAD_BUTTON 15
#define POSITION_SAVE_BUTTON 13
#define POSITION_STOP_BUTTON 10
#define POSITION_PLAY_BUTTON 8
#define POSITION_RECORD_BUTTON 7
#define STARTUPSCREEN 0
#define STEP_SEQUENCER_VIEW 1
#define ARRANGMENT_VIEW 2

#define POSITION_BAR_BUTTON 5
#define POSITION_START_LOOP_BUTTON 16
#define POSITION_END_LOOP_BUTTON 18

#define GRID_LENGTH_HOR 256
#define GRID_LENGTH_VERT 192

#define SONG_POSITION_POINTER_Y 228
#define BAR_POSITION_POINTER_Y 232
#define STEP_POSITION_POINTER_Y 236
#define POSITION_POINTER_THICKNESS 3

#define INFO_BOX_WIDTH 200
#define INFO_BOX_HEIGTH 120
#define INFOBOX_OFFSET 60
#define INFOBOX_TEXT_OFFSET 80

#define OCTAVE_CHANGE_LEFTMOST 18
#define OCTAVE_CHANGE_RIGHTMOST 20
#define OCTAVE_CHANGE_UP_TOPMOST 2
#define OCTAVE_CHANGE_UP_BOTTOMMOST 3
#define OCTAVE_CHANGE_DOWN_TOPMOST 4
#define OCTAVE_CHANGE_DOWN_BOTTOMMOST 5

#define OCTAVE_CHANGE_TEXT 3
#define NO_VALUE 6789
#define NO_NAME "NO_NAME"
class tftClass
{

public:
    void tft_setup(int dly);
    tftClass(ILI9341_t3n *display);
    ~tftClass();

    void tftUpdate(int _pixelOnX, int _pixelOnY);
    void tftUpdateClock(byte _currentTick, byte _currentBar, byte _startLoop, byte _endLoop);

    void show();

    void clearWorkSpace();
    void drawPot(int XPos, byte YPos, int dvalue, const char *dname);
    void drawEnvelope(byte YPos, byte attack, byte decay, byte sustain, byte release);
    void draw_value_box(byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int _value, const char *name, int color, byte _size, bool drawRect, bool drawFilling);
    void show_active_page_info(const char *_pagename, byte _pagenumber); // shows the actual page on the the top left
    void startUpScreen();
    // infobox
    void get_infobox_background();
    void set_infobox_background(int _DisplayTime);
    void reset_infobox_background();
    void set_infobox_next_line(byte _lineNumber); //_lineNumber must be bigger 0
    void draw_sequencer_arranger_parameter(byte _track, byte _encoder, const char *_name, int _value, const char *_valuedName);
    // cursor
    void tftUpdateCursor(int X, int Y, int dX, int dY);
    void moveCursor();
    // clock
    void drawstepPosition();
    void drawbarPosition();

    // songmode
    void gridSongMode(int songpageNumber);
    void drawsongmodepageselector();
    void draw_arranger_parameters(byte lastProw);
    void draw_arrangment_line(byte _trackNr, byte _bar); // b= 0-255; which bar
    void draw_arrangment_lines(byte _track, byte _page);
    void draw_clipNr_arranger(byte _trackNr, byte _bar);
    void draw_offset_arranger(byte _trackNr, byte _bar);

    // stepsequencer
    void drawStepSequencerStatic();
    void draw_stepSequencer_parameters(byte row);
    void draw_Notenames();
    void drawOctaveTriangle();
    void draw_Clipselector();
    void draw_note_on_tick(byte _note, byte _when);
    void draw_notes_in_grid();

    void draw_MIDI_CC_screen();
    void draw_MIDI_CC(byte XPos, byte YPos);
    void draw_stepSequencer_parameter_value(byte lastPRow, byte XPos, byte YPos, byte value, const char *name);
    void draw_stepSequencer_parameter_text(byte lastPRow, byte XPos, byte YPos, const char *text, const char *name);
    void draw_edit_presetNr_ccChannel(byte n, byte lastProw);
    void draw_edit_presetNr_ccValue(byte n, byte lastProw);

    // mixer
    void draw_mixer();
    void draw_mixer_FX_page1();
    void draw_mixer_FX_page2();
    bool updateClock;

private:
    ILI9341_t3n *tft; // Pointer to the display object
    int pixelOnX;
    int pixelOnY;
    uint16_t tftRAM[16][16];
    byte cursorDeltaX;
    byte cursorDeltaY;

    byte cursorOnTick;
    byte cursorOnNote;
    byte cursorOnBar;
    byte cursorOnTrack;
    const int encoder_colour[NUM_ENCODERS] = {ILI9341_BLUE, ILI9341_RED, ILI9341_GREEN, ILI9341_WHITE};
    unsigned long infoboxTimeAtCall = 0;
    unsigned long infoboxTimeAtPress = 0;
    int infoboxWaitingTime = 1000;
    bool infoboxShow = false;
    bool infoboxClear = false;
    int phraseSegmentLength = 16;

    byte currentTick;
    byte currentBar;
    byte loopStart;
    byte loopEnd;
    /* data */
};
extern tftClass *mytft;
#endif // TFT_CLASS_H
