#ifndef HW_STUFF_H
#define HW_STUFF_H

#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <Bounce2.h>
#include <Encoder.h>
#include "SD.h"
extern File myFile;
// display
extern ILI9341_t3n tft;
#define CS_PIN 14   // alternate Pins: any digital pin
#define TFT_DC 9    // alternate Pins 9, 10, 20, 21
#define TFT_CS 10   // alternate Pins 9, 15, 20, 21
#define TFT_RST 255 // 255 = unused, connect to 3.3V
#define TFT_MOSI 11 // shareable
#define TFT_SCK 13  // shareable
#define TFT_MISO 12 // shareable
#define STEP_FRAME_W 16
#define STEP_FRAME_H 16
#define TRACK_FRAME_H 24
// for arranger
#define BARS_PER_PAGE 16
// for sequencer
#define SEQ_GRID_LEFT 30
#define SEQ_GRID_RIGHT (18 * STEP_FRAME_W) - 2
#define SEQ_GRID_TOP 1
#define SEQ_GRID_BOTTOM 12
#define SEQUENCER_OPTIONS_VERY_RIGHT 18
#define SEQUENCER_OPTIONS_RIGHT 16
extern bool updateTFTScreen;

extern uint16_t tft_frame_buffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];
#define ENABLE_ILI9341_FRAMEBUFFER

#define TRELLIS_PADS_X_DIM 16
#define TRELLIS_PADS_Y_DIM 8
// trellis
#define Y_DIM 12 // number of rows of key
#define X_DIM 4  // number of columns of keys
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

#define TRELLIS_SCREEN_PIANO 10
#define TRELLIS_SCREEN_SONGPAGE_SELECTION 11
#define TRELLIS_SCREEN_MIXER 12
#define TRELLIS_SCREEN_MIXER1 13

#define TRELLIS_SCREEN_PERFORM 14
#define TRELLIS_SCREEN_ARRANGER_1 15
#define TRELLIS_SCREEN_ARRANGER_2 16

#define TRELLIS_SCREEN_STARTUP 99

// trellis
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
// #define TRELLIS_LIGHTBLUE #ADD8E6

#define TRELLIS_1 0x000066
#define TRELLIS_2 579

extern bool neotrellisPressed[X_DIM * Y_DIM];
extern bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];

extern const byte TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern const char FLASHMEM *CCnames[129];
extern int trellisTrackColor[9];
extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
extern bool trellisShowClockPixel[Y_DIM];
extern byte trellisPianoTrack;
extern byte trellisPerformIndex[TRELLIS_PADS_X_DIM];
extern byte performCC[TRELLIS_PADS_X_DIM];

extern const byte gateOutputPin[8];
void gate_setup();

// encoder
#define NUM_ENCODERS 4
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
extern Encoder Enc1;
extern Encoder Enc2;
extern Encoder Enc3;
extern Encoder Enc4;
extern Encoder *allEncoders[NUM_ENCODERS];
extern bool enc_moved[NUM_ENCODERS];
extern int encoded[NUM_ENCODERS];
extern bool enc_button[NUM_ENCODERS];
extern long oldEnc[4];
extern const uint8_t BUTTON_PINS[NUM_ENCODERS];
extern Bounce *encButtons;
extern void encoder_setup(int dly);
extern void readEncoders();

// midi
void midi_setup(byte dly);
void readMIDI();
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);

// project
#define NUM_TRACKS 8

extern int pixelTouchX;
extern int gridTouchY;
extern byte lastPotRow;
extern byte activeScreen;
extern bool change_plugin_row;

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

// clips
#define MAX_CLIPS 9
#define NUM_USER_CLIPS 7
#define NUM_PRESETS 8

// colors
extern int trackColor[9];

#endif // HW_STUFF_H