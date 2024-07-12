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

// buttons
// define the cymbols on the buttons of the keypads
char keys[ROWS][COLS] = {
    {'P', 'H', 'O', 'G'},
    {'M', 'F', 'N', 'E'},
    {'L', 'C', 'K', 'D'},
    {'J', 'A', 'I', 'B'}};
byte rowPins[ROWS] = {37, 36, 35, 34}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {41, 38, 39, 40}; // connect to the column pinouts of the keypad
Adafruit_Keypad kpd = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
bool buttonPressed[NUM_BUTTONS];
unsigned long buttonPressStartTime[NUM_BUTTONS] = {0};  // Zeitpunkt, zu dem jeder Button gedrückt wurde
const unsigned long longPressDuration = 600;  // 1 Sekunde in Millisekunden

void button_setup(int dly)
{
    kpd.begin();
    Serial.println("Initializing Keypad");
    tft.println("Initializing Keypad");
    tft.updateScreenAsync();
    delay(dly);
}
void readMainButtons()
{
    kpd.tick();
    while (kpd.available())
    {
        if (millis() % 10 == 0)
        {
            keypadEvent key = kpd.read();
            if (key.bit.EVENT == KEY_JUST_PRESSED)
            {
                int pressedKey = (int)(key.bit.KEY - 65);
                buttonPressed[pressedKey] = true;
                buttonPressStartTime[pressedKey] = millis();
                Serial.printf("pressed Key: %d\n", pressedKey);
            }

            if (key.bit.EVENT == KEY_JUST_RELEASED)
            {
                int pressedKey = (int)(key.bit.KEY - 65);
                buttonPressed[pressedKey] = false;
            }
        }
    }
}

// encoder
Encoder Enc1(31, 32);
Encoder Enc2(27, 28);
Encoder Enc3(24, 25);
Encoder Enc4(2, 3);
Encoder *allEncoders[NUM_ENCODERS]{&Enc1, &Enc2, &Enc3, &Enc4};
bool enc_moved[NUM_ENCODERS]{0, 0, 0, 0};
int encoded[NUM_ENCODERS];
bool enc_button[NUM_ENCODERS];
long oldEnc[4] = {-999, -999, -999, -999};
// Encoder Buttons
const uint8_t BUTTON_PINS[NUM_ENCODERS] = {30, 29, 26, 4};
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
            //Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
        if (newEnc[i] < oldEnc[i])
        {
            enc_moved[i] = true;
            encoded[i] = -encMultiplier[i];
            oldEnc[i] = newEnc[i];
            //Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
    }
}

// midi
void midi_setup(byte dly)
{
    usbMIDI.setHandleNoteOff(myNoteOff);
    usbMIDI.setHandleNoteOn(myNoteOn);
    usbMIDI.begin(); // Launch MIDI and listen to channel 4
    Serial.println("Initializing MIDI");
    tft.println("Initializing MIDI");
    tft.updateScreenAsync();
    delay(dly);
}
void readMIDI()
{
    usbMIDI.read();
}

