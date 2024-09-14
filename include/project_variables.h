#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include <Arduino.h>
#include "hw_stuff.h"
#include "SD.h"
extern File myFile;


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



//#define TRELLIS_MAX_PAGES TRELLIS_SCREEN_ARRANGER_1+SONGMODE_PAGE_16+1
#define TRELLIS_CONTROL_X_DIM 4
#define TRELLIS_CONTROL_Y_DIM 12




extern int pixelTouchX;
extern int gridTouchY;
extern byte lastPotRow;
extern byte activeScreen;
extern bool change_plugin_row;

extern unsigned long infoboxTimeAtCall;
extern unsigned long infoboxTimeAtPress;
extern int infoboxWaitingTime;
extern bool infoboxShow;
extern bool infoboxClear;


extern byte active_track;
extern byte arrangerpage;
extern const char FLASHMEM *songNames[MAX_SONGS];

extern int phraseSegmentLength;
extern byte trellisScreen;
extern int controlColors[16];
extern int trackColor[9];
extern int trellisControllBuffer[TRELLIS_CONTROL_X_DIM][TRELLIS_CONTROL_Y_DIM];
//extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
//extern int ***trellisMainGridBuffer;
//extern int **trellisControllBuffer;

extern bool trellisShowClockPixel[Y_DIM];
//extern byte trellisPianoTrack;
//extern byte trellisPerformIndex[NUM_STEPS];

extern byte performCC[16];
extern char _trackname[20];

extern const char FLASHMEM *channelOutNames[MAX_OUTPUTS + 1];
extern const char FLASHMEM *noteNames[12];
extern const char FLASHMEM *seqModname[5];

// plugins
extern float *note_frequency;
extern int tuning;
extern const char *filterName[4];






#endif // GLOBAL_STUFF_H