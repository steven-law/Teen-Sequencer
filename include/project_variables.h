#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include <Arduino.h>
#include "hw_stuff.h"
#include "SD.h"
extern File myFile;

extern elapsedMicros msecsclock;
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

#define STEP_FRAME_W 16
#define STEP_FRAME_H 16
#define TRACK_FRAME_H 24
#define GRID_LENGTH_HOR 256
#define GRID_LENGTH_VERT 192

#define SONG_POSITION_POINTER_Y 228
#define BAR_POSITION_POINTER_Y 232
#define STEP_POSITION_POINTER_Y 236
#define POSITION_POINTER_THICKNESS 3

#define NUM_STEPS 16
#define MAX_TICKS 96
#define MAX_VOICES 12
#define MAX_SONGS 9
// active Tracks
#define ACTIVE_TRACK_1 0
#define ACTIVE_TRACK_2 1
#define ACTIVE_TRACK_3 2
#define ACTIVE_TRACK_4 3
#define ACTIVE_TRACK_5 4
#define ACTIVE_TRACK_6 5
#define ACTIVE_TRACK_7 6
#define ACTIVE_TRACK_8 7
#define NUM_TRACKS 8
// pages
#define TRACK_1_PAGE 0
#define TRACK_2_PAGE 1
#define TRACK_3_PAGE 2
#define TRACK_4_PAGE 3
#define TRACK_5_PAGE 4
#define TRACK_6_PAGE 5
#define TRACK_7_PAGE 6
#define TRACK_8_PAGE 7
#define STARTUPSCREEN_PAGE 8
// #define EMPTY 9
#define SONGMODE_PAGE_1 0
#define SONGMODE_PAGE_2 1
#define SONGMODE_PAGE_3 2
#define SONGMODE_PAGE_4 3
#define SONGMODE_PAGE_5 4
#define SONGMODE_PAGE_6 5
#define SONGMODE_PAGE_7 6
#define SONGMODE_PAGE_8 7
#define SONGMODE_PAGE_9 8
#define SONGMODE_PAGE_10 9
#define SONGMODE_PAGE_11 10
#define SONGMODE_PAGE_12 11
#define SONGMODE_PAGE_13 12
#define SONGMODE_PAGE_14 13
#define SONGMODE_PAGE_15 14
#define SONGMODE_PAGE_16 15

// encoder functions
#define INPUT_FUNCTIONS_FOR_CURSOR 0
#define INPUT_FUNCTIONS_FOR_SEQUENCER 1
#define INPUT_FUNCTIONS_FOR_SEQUENCER_ALT 2
#define INPUT_FUNCTIONS_FOR_ARRANGER 3
#define INPUT_FUNCTIONS_FOR_ARRANGER_ALT 4
#define INPUT_FUNCTIONS_FOR_PLUGIN 5
#define INPUT_FUNCTIONS_FOR_SEQUENCER_MODES 6
#define INPUT_FUNCTIONS_FOR_MIXER1 7
#define INPUT_FUNCTIONS_FOR_MIXER2 8
#define INPUT_FUNCTIONS_FOR_MIXER3 9
#define INPUT_FUNCTIONS_FOR_FX1 10
#define INPUT_FUNCTIONS_FOR_FX2 11
#define INPUT_FUNCTIONS_FOR_FX3 12
#define INPUT_FUNCTIONS_FOR_PERFORM 13

#define INFO_BOX_WIDTH 200
#define INFO_BOX_HEIGTH 120
#define INFOBOX_OFFSET 60
#define INFOBOX_TEXT_OFFSET 80

#define NUM_MIDI_OUTPUTS 48
// plugins
#define CH_PLUGIN_1 NUM_MIDI_OUTPUTS+1
#define CH_PLUGIN_2 NUM_MIDI_OUTPUTS+2
#define CH_PLUGIN_3 NUM_MIDI_OUTPUTS+3
#define CH_PLUGIN_4 NUM_MIDI_OUTPUTS+4
#define CH_PLUGIN_5 NUM_MIDI_OUTPUTS+5
#define CH_PLUGIN_6 NUM_MIDI_OUTPUTS+6
#define CH_PLUGIN_7 NUM_MIDI_OUTPUTS+7
#define CH_PLUGIN_8 NUM_MIDI_OUTPUTS+8
#define CH_PLUGIN_9 NUM_MIDI_OUTPUTS+9

#define NUM_PLUGINS 11
#define MIDI_CC_RANGE 127
#define MIDI_CC_RANGE_FLOAT 127.00
#define NUM_PLUGIN_PRESETS 8
#define SAMPLE_ROOT 69
#define MAX_RESONANCE 5.00
#define MAX_FILTERSWEEP 7.00
#define SEQUENCER_OPTIONS_VERY_RIGHT 18
#define PRESET_ENCODER 3

#define MAX_OUTPUTS NUM_MIDI_OUTPUTS + NUM_PLUGINS
// trellis


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


#define TRELLIS_MAX_PAGES TRELLIS_SCREEN_ARRANGER_1+SONGMODE_PAGE_16+1
#define TRELLIS_CONTROL_X_DIM 4
#define TRELLIS_CONTROL_Y_DIM 12




extern int pixelTouchX;
extern int gridTouchY;
extern byte lastPotRow;
extern byte activeScreen;
extern bool change_plugin_row;
extern int tftRamInfoBox[INFO_BOX_WIDTH][INFO_BOX_HEIGTH];
//extern int **tftRamInfoBox;
extern unsigned long currentTime;
extern bool showBox;

extern byte active_track;
extern byte arrangerpage;
extern bool otherCtrlButtons;
extern const char FLASHMEM *songNames[MAX_SONGS];

extern int phraseSegmentLength;
extern const char FLASHMEM *playstate[3];
extern byte trellisScreen;
extern int controlColors[16];
extern int trackColor[9];
extern int trellisTrackColor[9];
extern int trellisControllBuffer[TRELLIS_CONTROL_X_DIM][TRELLIS_CONTROL_Y_DIM];
extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
//extern int ***trellisMainGridBuffer;
//extern int **trellisControllBuffer;

extern bool trellisRecall;
extern bool trellisShowClockPixel[Y_DIM];
extern byte trellisPianoTrack;
extern byte trellisPerformIndex[NUM_STEPS];

extern byte trellisMixerIndex[NUM_TRACKS];
extern byte performCC[16];
extern char _trackname[20];
extern const char FLASHMEM *CCnames[129];

extern const char FLASHMEM *channelOutNames[MAX_OUTPUTS + 1];
extern const char FLASHMEM *noteNames[12];
extern const char FLASHMEM *seqModname[5];

// plugins
extern float *note_frequency;
extern int tuning;
extern const char *filterName[4];
#endif // GLOBAL_STUFF_H