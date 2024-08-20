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


//trellis
#define Y_DIM 8  // number of rows of key
#define X_DIM 24 // number of columns of keys
#define TRELLIS_POTROW 20
#define TRELLIS_BUTTON_RECORD 21
#define TRELLIS_START_CLOCK 22
#define TRELLIS_STOP_CLOCK 23

#define TRELLIS_BUTTON_LEFT 44
#define TRELLIS_BUTTON_RIGHT 45
#define TRELLIS_BUTTON_UP 46
#define TRELLIS_BUTTON_DOWN 47

#define TRELLIS_BUTTON_ENTER 68
#define TRELLIS_BUTTON_SHIFT 69
#define TRELLIS_BUTTON_PIANO 70
#define TRELLIS_BUTTON_SEQMODE 71

#define TRELLIS_BUTTON_ARRANGER 92
#define TRELLIS_BUTTON_MIXER 93
#define TRELLIS_BUTTON_PLUGIN 94
#define TRELLIS_BUTTON_SEQUENCER 95

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
#define TRELLIS_SCREEN_PERFORM 13

#define TRELLIS_SCREEN_ARRANGER_1 15
#define TRELLIS_SCREEN_ARRANGER_2 16

#define TRELLIS_SCREEN_STARTUP 99

//extern bool trellisPressed[X_DIM * Y_DIM];
extern bool *trellisPressed;
#define BUTTON_LEFT 0
#define BUTTON_RIGHT 1
#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_ROW 4
#define BUTTON_REC 5
#define BUTTON_PLAY 6
#define BUTTON_STOP 7
#define BUTTON_TRACK 8
#define BUTTON_PLUGIN 9
#define BUTTON_SONG 10
#define BUTTON_MIXER 11
#define BUTTON_FX 12
#define BUTTON_SMP 13
#define BUTTON_SHIFT 14
#define BUTTON_ENTER 15
#define BUTTONS_PER_ROW 8
#define NUM_BUTTONS 16
extern const unsigned long longPressDuration;  // 1 Sekunde in Millisekunden

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