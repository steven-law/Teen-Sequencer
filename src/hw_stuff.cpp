#include <Arduino.h>
#include "hw_stuff.h"

// Display
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO); // initiate TFT-Srceen
DMAMEM uint16_t tft_frame_buffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];
void tft_setup(int dly)
{
    // initialize the TFT- and Touchscreen

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setFrameBuffer(tft_frame_buffer);
    tft.useFrameBuffer(true);
    tft.initDMASettings();
    tft.updateScreenAsync();
    delay(dly);
    // tft.updateChangedAreasOnly(true);
    tft.setTextColor(ILI9341_WHITE);
    tft.setFont(Arial_8);
    tft.setCursor(0, 3);
    Serial.println("Initializing Screen");
    tft.println("Initializing Screen");
    tft.updateScreenAsync();
    delay(dly);
}


const unsigned long longPressDuration = 600;           // 1 Sekunde in Millisekunden
bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
byte TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM]{0 ,1, 2, 3,      16, 17, 18, 19,  32, 33, 34, 35,  48, 49, 50, 51, 
                                                         4 ,5, 6, 7,      20, 21, 22, 23,  36, 37, 38, 39,  52, 53, 54, 55,
                                                         8, 9, 10, 11,    24, 25, 26, 27,  40, 41, 42, 43,  56, 57, 58, 59,
                                                         12, 13, 14, 15,  28, 29, 30, 31,  44, 45, 46, 47,  60, 61, 62, 63, 
                                                         64, 65, 66, 67,  80, 81, 82, 83,  96, 97, 98, 99,  112,113,114,115,
                                                         68, 69, 70, 71,  84, 85, 86, 87,  100,101,102,103, 116,117,118,119,
                                                         72, 73, 74, 75,  88, 89, 90, 91,  104,105,106,107, 120,121,122,123,
                                                         76, 77, 78, 79,  92, 93, 94, 95,  108,109,110,111, 124,125,126,127};
const byte gateOutputPin[8]{22,40,38,37,36,35,34,33};
void gate_setup(){
    for (int i=0;i<8;i++){
        pinMode(gateOutputPin[i], OUTPUT);
    }
}

// encoder
Encoder Enc1(4, 3);
Encoder Enc2(29, 28);
Encoder Enc3(26, 25);
Encoder Enc4(31, 32);
Encoder *allEncoders[NUM_ENCODERS]{&Enc1, &Enc2, &Enc3, &Enc4};
bool enc_moved[NUM_ENCODERS]{0, 0, 0, 0};
int encoded[NUM_ENCODERS];
bool enc_button[NUM_ENCODERS];
long oldEnc[4] = {-999, -999, -999, -999};
// Encoder Buttons
const uint8_t BUTTON_PINS[NUM_ENCODERS] = {2, 27, 24, 30};
Bounce *encButtons = new Bounce[NUM_ENCODERS];
void encoder_setup(int dly)
{

    for (int i = 0; i < NUM_ENCODERS; i++)
    {
        encButtons[i].attach(BUTTON_PINS[i], INPUT_PULLUP); // setup the bounce instance for the current button
        encButtons[i].interval(25);                         // interval in ms
    }
    Serial.println("Initializing Encoders");
    tft.println("Initializing Encoders");
    tft.updateScreenAsync();
    delay(dly);
}
void readEncoders()
{
    long newEnc[NUM_ENCODERS];
    byte encMultiplier[NUM_ENCODERS];
    for (int i = 0; i < NUM_ENCODERS; i++)
    {
        encoded[i] = 0;
        enc_moved[i] = 0;
        newEnc[i] = allEncoders[i]->read() / 4;
        encMultiplier[i] = 1;

        // Update the Bounce instance :
        encButtons[i].update();
        // If it fell, flag the need to toggle the LED
        if (encButtons[i].fell())
        {
            enc_button[i] = true;
            Serial.printf("EncButton: %d\n", i);
        }

        if (!encButtons[i].read())
            encMultiplier[i] = 8;

        if (newEnc[i] > oldEnc[i])
        {
            enc_moved[i] = true;
            encoded[i] = encMultiplier[i];
            oldEnc[i] = newEnc[i];
            // Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
        if (newEnc[i] < oldEnc[i])
        {
            enc_moved[i] = true;
            encoded[i] = -encMultiplier[i];
            oldEnc[i] = newEnc[i];
            // Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
    }
    if (encButtons[0].fell())
    {
        neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
        // Serial.printf("EncButton: %d\n", i);
    }
    if (encButtons[0].rose())
    {
        neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        // Serial.printf("EncButton: %d\n", i);
    }
}

// midi
void midi_setup(byte dly)
{
    usbMIDI.setHandleNoteOff(myNoteOff);
    usbMIDI.setHandleNoteOn(myNoteOn);
    usbMIDI.begin(); // Launch MIDI
    Serial.println("Initializing MIDI");
    tft.println("Initializing MIDI");
    tft.updateScreenAsync();
    delay(dly);
}
void readMIDI()
{
    usbMIDI.read();
}
