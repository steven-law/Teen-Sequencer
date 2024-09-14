#include <Arduino.h>
#include "hw_stuff.h"

bool change_plugin_row = false;
bool updateTFTScreen = false;
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
int trellisTrackColor[9]{TRELLIS_RED, TRELLIS_PURPLE, TRELLIS_OLIVE, TRELLIS_YELLOW, TRELLIS_BLUE, 9365295, TRELLIS_AQUA, TRELLIS_GREEN, 900909};

//bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
const byte gateOutputPin[8]{22, 40, 38, 37, 36, 35, 34, 33};
void gate_setup()
{
    for (int i = 0; i < 8; i++)
    {
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
            updateTFTScreen = true;
            enc_button[i] = true;
            Serial.printf("EncButton: %d\n", i);
        }

        if (!encButtons[i].read())
            encMultiplier[i] = 8;

        if (newEnc[i] > oldEnc[i])
        {
            updateTFTScreen = true;
            enc_moved[i] = true;
            encoded[i] = encMultiplier[i];
            oldEnc[i] = newEnc[i];
            // Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
        if (newEnc[i] < oldEnc[i])
        {
            updateTFTScreen = true;
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
