#ifndef HW_STUFF_H
#define HW_STUFF_H

#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <Bounce2.h>
#include <Encoder.h>

extern ILI9341_t3n tft;

// Display
// #define TIRQ_PIN 15 // alternate Pins: any digital pin
#define CS_PIN 14   // alternate Pins: any digital pin
#define TFT_DC 9    // alternate Pins 9, 10, 20, 21
#define TFT_CS 10   // alternate Pins 9, 15, 20, 21
#define TFT_RST 255 // 255 = unused, connect to 3.3V
#define TFT_MOSI 11 // shareable
#define TFT_SCK 13  // shareable
#define TFT_MISO 12 // shareable
extern uint16_t tft_frame_buffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];
#define ENABLE_ILI9341_FRAMEBUFFER
extern void tft_setup(int dly);

#define TRELLIS_PADS_X_DIM 16
#define TRELLIS_PADS_Y_DIM 8
//trellis
#define Y_DIM 12  // number of rows of key
#define X_DIM 4 // number of columns of keys
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

#define TRELLIS_SCREEN_STARTUP 99

extern bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern bool neotrellisPressed[X_DIM * Y_DIM];
extern byte TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];

extern bool change_plugin_row;
extern bool updateTFTScreen;

extern const byte gateOutputPin[8];
void gate_setup();

// encoder
#define NUM_ENCODERS 4
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
#endif // HW_STUFF_H