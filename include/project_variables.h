#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include <Arduino.h>
#include "hw_stuff.h"

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
#define POSITION_POINTER_THICKNESS 3
#define NUM_STEPS 16
#define MAX_TICKS 96
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


extern int pixelTouchX;
extern int gridTouchY;
extern byte lastPotRow;
extern byte encoder_function;
extern bool change_plugin_row;

extern byte active_track;
extern byte arrangerpage;
extern bool otherCtrlButtons;

extern int phraseSegmentLength;
extern const char *playstate[3];
extern char _trackname[20];
extern const char *CCnames[128];
extern const char *noteNames[12];
extern  const char *seqModname[5];




#endif // GLOBAL_STUFF_H