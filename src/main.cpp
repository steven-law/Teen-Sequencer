/*ideas:
mixer on trellis
cv/gate outputs

half done: move variables into flashmem/ram2
done: add usb-host for midi tx & rx
done: add mousecontrol
add real polyphony

done: add midi thru for channels 9-16
done: add volume per bar in songmode, thickness of line represents volume
done: add 2nd parameter to edit per tick
done: choose midi output
done: show active songpage
done: look what takes so long to load the song screen
*/
// library includes
#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <SPI.h>
#include <Wire.h>
#include <MIDI.h>
#include <USBHost_t36.h>
#include <Adafruit_MCP4728.h>
#include "ownLibs/Adafruit_Trellis.h"
#include "hardware/trellis_main.h"

#include "Adafruit_NeoTrellis.h"
// #include <Adafruit_NeoTrellis.h>
//  #include "neoTrellis.h"
#include "hw_stuff.h"
#include "project_variables.h"
#include "ownLibs/cursor.h"
// #include "ownLibs/clock.h"
#include "ownLibs/myClock.h"
#include "Track.h"
#include <Plugins/plugin_List.h>
#include <FX/fx_List.h>
#include "FX/Output.h"

#define NEOTRELLIS_INT_PIN 39
#define TRELLIS_INT_PIN 41
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP 0xFC
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();
Adafruit_Trellis matrix4 = Adafruit_Trellis();
Adafruit_Trellis matrix5 = Adafruit_Trellis();
Adafruit_Trellis matrix6 = Adafruit_Trellis();
Adafruit_Trellis matrix7 = Adafruit_Trellis();

Adafruit_TrellisSet trellis = Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7);

#define NUMTRELLIS 8

#define numKeys (NUMTRELLIS * 16)
// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x46, &Wire1)},
    {Adafruit_NeoTrellis(0x36, &Wire1)},
    {Adafruit_NeoTrellis(0x2E, &Wire1)}

};

Adafruit_MultiTrellis neotrellis((Adafruit_NeoTrellis *)t_array, Y_DIM / 4, X_DIM / 4);
// Adafruit_NeoTrellis control(0x46, &Wire1);
//  Adafruit_MultiTrellis control((Adafruit_NeoTrellis *)_control, 1, 1);
Adafruit_MCP4728 mcp;
Adafruit_MCP4728 mcp2;
Plugin_1 plugin_1("Strg", 1);
Plugin_2 plugin_2("1OSC", 2);
Plugin_3 plugin_3("2FM", 3);
Plugin_4 plugin_4("mDrm", 4);
Plugin_5 plugin_5("Drum", 5);
Plugin_6 plugin_6("Adtv", 6);
Plugin_7 plugin_7("Boom", 7);
Plugin_8 plugin_8("dTune", 8);
Plugin_9 plugin_9("rDrm", 9);
Plugin_10 plugin_10("SF2", 9);
Plugin_11 plugin_11("Ext", 9);

PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9, &plugin_10, &plugin_11};
// PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9};
FX_1 fx_1("Rev", 1);
FX_2 fx_2("Bit", 2);
FX_3 fx_3("Nix", 3);
Output MasterOut(3);
MyClock myClock(&tft);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice usbMidi1(myusb);
USBHIDParser hid1(myusb);
// USBHIDParser hid2(myusb);
MouseController mouse1(myusb);
TrellisForMainGrid trellisMain(&trellis);
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long trellisRestartPreviousMillis = 0; // will store last time LED was updated
unsigned long trellisReadPreviousMillis = 0;    // will store last time LED was updated

// constants won't change:
const long trellisReadInterval = 30;       // interval at which to blink (milliseconds)
const long trellisRestartInterval = 60000; // interval at which to blink (milliseconds)
//  put function declarations here:

void draw_mixer();
void set_mixer(byte row);
void set_mixer_gain(byte XPos, byte YPos, const char *name, byte trackn);
void draw_mixer_FX_page1();
void draw_mixer_FX_page2();
void set_mixer_FX_page1(byte row);
void set_mixer_FX_page2(byte row);
void set_mixer_dry(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX1(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX2(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX3(byte XPos, byte YPos, const char *name, byte trackn);

void sendNoteOn_CV_Gate();
void sendNoteOff_CV_Gate();
void sendNoteOn(byte Note, byte Velo, byte Channel);
void sendNoteOff(byte Note, byte Velo, byte Channel);
void sendControlChange(byte control, byte value, byte Channel);
void sendClock();
void mouse(byte deltaX, byte maxY);
void get_infobox_background();
void set_infobox_background(int _DisplayTime);
void reset_infobox_background();
void set_infobox_next_line(byte _lineNumber); //_lineNumber must be bigger 0
void detect_USB_device();
void detect_mouse();
TrellisCallback blink2(keyEvent evt);
TrellisCallback blink(keyEvent evt);
void neotrellis_static();
void neotrellis_setup();
void neo_trellis_set_brightness();
void neotrellis_set_control_buffer(int _x, int _y, int _color);
int neotrellis_get_control_buffer(int _x, int _y);
void neotrellis_recall_control_buffer();
void trellis_set_main_buffer(int _page, int _x, int _y, int color);


void neotrellis_start_clock();
void neotrellis_stop_clock();
void neo_trellis_save_load();
void neotrellis_set_potRow();
void neotrellis_SetCursor(byte maxY);
void neotrellis_show();

void neotrellis_show_tft_plugin();
void neotrellis_show_tft_seqMode();

void trellis_show_arranger();

void trellis_show_tft_mixer();
void neotrellis_show_piano();
void neotrellis_set_fast_record();
void neotrellis_set_mute();
void neotrellis_set_solo();
void neotrellis_perform_set_active();
void set_performCC(byte XPos, byte YPos, const char *name);
void neo_trellis_select_trackClips();

void setup()
{
  // while (!Serial)
  {
  }
  //   put your setup code here, to run once:
  tft_setup(100);
  encoder_setup(100);
  delay(1500);
  midi_setup(100);
  MIDI1.begin();
  myusb.begin();
  pinMode(NEOTRELLIS_INT_PIN, INPUT);
  pinMode(TRELLIS_INT_PIN, INPUT);
  digitalWrite(TRELLIS_INT_PIN, HIGH);
  Serial.println("DAWsketch started");
  Wire1.begin();
  if (!neotrellis.begin())
  {
    Serial.println("DAWfailed to begin trellis");
    while (1)
      delay(1);
  }

  Serial.println("DAWtrellis started");

  // Try to initialize!
  if (!mcp.begin(0x64, &Wire1))
  {
    Serial.println("Failed to find MCP4728 chip");
    while (1)
    {
      delay(10);
    }
  }
  // Try to initialize!
  if (!mcp2.begin(0x67, &Wire1))
  {
    Serial.println("Failed to find MCP4728 chip2");
    while (1)
    {
      delay(10);
    }
  }

  Serial.println("DAC´s started");
  /*
   trellisMainGridBuffer = new int **[TRELLIS_MAX_PAGES];
   for (int i = 0; i < TRELLIS_MAX_PAGES; i++)
   {
     trellisMainGridBuffer[i] = new int *[TRELLIS_PADS_X_DIM];
     for (int j = 0; j < TRELLIS_PADS_X_DIM; j++)
     {
       trellisMainGridBuffer[i][j] = new int[TRELLIS_PADS_Y_DIM];
     }
   }
   // fill the array
   for (int i = 0; i < TRELLIS_MAX_PAGES; i++)
   {
     for (int j = 0; j < TRELLIS_PADS_X_DIM; j++)
     {
       for (int k = 0; k < TRELLIS_PADS_Y_DIM; k++)
       {
         trellisMainGridBuffer[i][j][k] = TRELLIS_BLACK;
       }
     }
   }

   trellisPressed = new bool[X_DIM * Y_DIM];
   for (int i = 0; i < X_DIM * Y_DIM; i++)
   {
     trellisPressed[i] = false;
   }

   trellisControllBuffer = new int *[TRELLIS_CONTROL_X_DIM];
   for (int j = 0; j < TRELLIS_CONTROL_X_DIM; j++)
   {
     trellisControllBuffer[j] = new int[TRELLIS_CONTROL_Y_DIM];
   }
   for (int j = 0; j < TRELLIS_CONTROL_X_DIM; j++)
   {
     for (int k = 0; k < TRELLIS_CONTROL_Y_DIM; k++)
     {
       trellisControllBuffer[j][k] = TRELLIS_BLACK;
     }
   }



*/
  gate_setup();
  myClock.setup();
  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  usbMidi1.setHandleNoteOn(myNoteOn);
  usbMidi1.setHandleNoteOff(myNoteOff);
  AudioMemory(320);
  MasterOut.setup();
  Serial.println("Audio & MIDI Setup done");
  note_frequency = new float[128];
  for (int r = 0; r < 128; r++)
  {
    note_frequency[r] = pow(2.0, ((double)(r - SAMPLE_ROOT) / 12.0));
  }
  // MIDI1.setHandleControlChange(myControlChange);
  startUpScreen();
  tft.updateScreenAsync();
  trellisScreen = TRELLIS_SCREEN_SEQUENCER_CLIP_0;
  neotrellis_setup();
  for (int x = 0; x < X_DIM / 4; x++)
  {
    for (int y = 0; y < Y_DIM / 4; y++)
    {
      t_array[y][x].pixels.setBrightness(5);
    }
  }
  trellis.setBrightness(0);
  // light up all the LEDs in order
  for (uint8_t i = 0; i < numKeys; i++)
  {
    trellis.setLED(i);
    trellis.writeDisplay();
    delay(1);
  }
  // then turn them off
  for (uint8_t i = 0; i < numKeys; i++)
  {
    trellis.clrLED(i);
    trellis.writeDisplay();
    delay(1);
  }
}

void loop()
{
  unsigned long loopStartTime = millis();
  unsigned long trellisRestartMillis = millis();
  // Serial.println("hello");
  //  put your main code here, to run repeatedly:
  readEncoders();
  // readMainButtons();
  readMIDI();

  MIDI1.read();
  myusb.Task();
  usbMidi1.read();

  sendNoteOn_CV_Gate();
  sendNoteOff_CV_Gate();
  detect_USB_device();
  if (!digitalRead(NEOTRELLIS_INT_PIN))
  {
    // Serial.println("reading trellis");
    neotrellis.read(false);
    neotrellis.setPixelColor(3, 1, TRELLIS_AQUA);
    neotrellis.show();

    delay(0);
  }
  neotrellis_set_control_buffer(3, 1, TRELLIS_BLACK);
  // trellis.setPixelColor(23, 1, TRELLIS_BLACK);
  // trellis.show();

  // trellis_setStepsequencer();
  trellis_show_arranger();
  neotrellis_perform_set_active();
  neotrellis_show_tft_seqMode();
  neotrellis_show_tft_plugin();
  neotrellis_set_mute();
  neotrellis_set_solo();
  neotrellis_set_fast_record();
  neotrellis_show_piano();
  trellisMain.trellis_perform();
  trellisMain.trellis_play_mixer();
  trellis_show_tft_mixer();

  neo_trellis_select_trackClips();
  neo_trellis_set_brightness();
  neo_trellis_save_load();

  input_behaviour();
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->update(pixelTouchX, gridTouchY);
    // if (trellisShowClockPixel[i])
  }
  get_infobox_background();
  if (updateTFTScreen)
  {
    tft.fillRect(70, lastPotRow * 4, 10, 3, ILI9341_ORANGE);
    tft.updateScreenAsync();
    updateTFTScreen = false;
    // trellis.writeDisplay();
  }

  unsigned long loopEndTime = millis();
  unsigned long trellisCurrentMillis = millis();

  if (trellisCurrentMillis - trellisReadPreviousMillis >= trellisReadInterval)
  {
    trellisReadPreviousMillis = trellisCurrentMillis;
    trellis_read();
    trellis.writeDisplay();
    neotrellis_recall_control_buffer();
    neotrellis_show();

  if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
  {
    mouse(2, 14);
    cursor.update(pixelTouchX, gridTouchY, 1, TRACK_FRAME_H);
  }
  else
  {
    mouse(2, 14);
    cursor.update(pixelTouchX, gridTouchY, 1, STEP_FRAME_H);
  }

  if (loopEndTime - loopStartTime > 500 /*|| trellisCurrentMillis - trellisRestartPreviousMillis >= trellisRestartInterval*/)
  {
    trellisRestartPreviousMillis = trellisRestartMillis;
    // trellis.trellisReset();
    Serial.println("restart trellis");
    neotrellis.begin();
    neotrellis_setup();
  }
}

// Mixer
void draw_mixer()
{

  if (change_plugin_row)
  {
    change_plugin_row = false;
    drawPot(0, 0, allTracks[0]->mixGainPot, "Tr D");
    drawActiveRect(3, 5, 1, 1, allTracks[0]->muted, "M", ILI9341_RED);
    drawActiveRect(4, 5, 1, 1, allTracks[0]->soloed, "S", ILI9341_WHITE);
    drawPot(1, 0, allTracks[1]->mixGainPot, "Tr 2");
    drawActiveRect(7, 5, 1, 1, allTracks[1]->muted, "M", ILI9341_RED);
    drawActiveRect(8, 5, 1, 1, allTracks[1]->soloed, "S", ILI9341_WHITE);
    drawPot(2, 0, allTracks[2]->mixGainPot, "Tr 3");
    drawActiveRect(11, 5, 1, 1, allTracks[2]->muted, "M", ILI9341_RED);
    drawActiveRect(12, 5, 1, 1, allTracks[2]->soloed, "S", ILI9341_WHITE);
    drawPot(3, 0, allTracks[3]->mixGainPot, "Tr 4");
    drawActiveRect(15, 5, 1, 1, allTracks[3]->muted, "M", ILI9341_RED);
    drawActiveRect(16, 5, 1, 1, allTracks[3]->soloed, "S", ILI9341_WHITE);

    drawPot(0, 2, allTracks[4]->mixGainPot, "Tr 5");
    drawActiveRect(3, 11, 1, 1, allTracks[4]->muted, "M", ILI9341_RED);
    drawActiveRect(4, 11, 1, 1, allTracks[4]->soloed, "S", ILI9341_WHITE);
    drawPot(1, 2, allTracks[5]->mixGainPot, "Tr 6");
    drawActiveRect(7, 11, 1, 1, allTracks[5]->muted, "M", ILI9341_RED);
    drawActiveRect(8, 11, 1, 1, allTracks[5]->soloed, "S", ILI9341_WHITE);
    drawPot(2, 2, allTracks[6]->mixGainPot, "Tr 7");
    drawActiveRect(11, 11, 1, 1, allTracks[6]->muted, "M", ILI9341_RED);
    drawActiveRect(12, 11, 1, 1, allTracks[6]->soloed, "S", ILI9341_WHITE);
    drawPot(3, 2, allTracks[7]->mixGainPot, "Tr 8");
    drawActiveRect(15, 11, 1, 1, allTracks[7]->muted, "M", ILI9341_RED);
    drawActiveRect(16, 11, 1, 1, allTracks[7]->soloed, "S", ILI9341_WHITE);
  }
}

void set_mixer(byte row)
{
  draw_mixer();
  if (row == 0)
  {
    set_mixer_gain(0, 0, "Tr D", 0);
    set_mixer_gain(1, 0, "Tr 2", 1);
    set_mixer_gain(2, 0, "Tr 3", 2);
    set_mixer_gain(3, 0, "Tr 4", 3);
  }

  if (row == 1)
  {
  }

  if (row == 2)
  {
    set_mixer_gain(0, 2, "Tr 5", 4);
    set_mixer_gain(1, 2, "Tr 6", 5);
    set_mixer_gain(2, 2, "Tr 7", 6);
    set_mixer_gain(3, 2, "Tr 8", 7);
  }

  if (row == 3)
  {
  }
}
void set_mixer_gain(byte XPos, byte YPos, const char *name, byte trackn)
{

  if (enc_moved[XPos])
  {

    allTracks[trackn]->mixGainPot = constrain(allTracks[trackn]->mixGainPot + encoded[XPos], 0, MIDI_CC_RANGE);
    allTracks[trackn]->mixGain = (float)(allTracks[trackn]->mixGainPot / MIDI_CC_RANGE_FLOAT);
    /*for (int i = 0; i < NUM_PLUGINS; i++)
   {
     if (allTracks[trackn]->MIDI_channel_out == i+17)
       allPlugins[i]->MixGain.gain(allTracks[trackn]->mixGain);
   }*/
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1)
      plugin_1.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_2)
      plugin_2.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_3)
      plugin_3.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_4)
      plugin_4.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_5)
      plugin_5.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_6)
      plugin_6.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_7)
      plugin_7.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_8)
      plugin_8.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_9)
      plugin_9.MixGain.gain(allTracks[trackn]->mixGain);
    drawPot(XPos, YPos, allTracks[trackn]->mixGainPot, name);
  }
}

void draw_mixer_FX_page1()
{
  if (change_plugin_row)
  {
    change_plugin_row = false;
    drawPot(0, 0, allTracks[0]->mixDryPot, "Dry D");
    drawPot(1, 0, allTracks[1]->mixDryPot, "Dry 2");
    drawPot(2, 0, allTracks[2]->mixDryPot, "Dry 3");
    drawPot(3, 0, allTracks[3]->mixDryPot, "Dry 4");

    drawPot(0, 1, allTracks[0]->mixFX1Pot, "FX1 D");
    drawPot(1, 1, allTracks[1]->mixFX1Pot, "FX1 2");
    drawPot(2, 1, allTracks[2]->mixFX1Pot, "FX1 3");
    drawPot(3, 1, allTracks[3]->mixFX1Pot, "FX1 4");

    drawPot(0, 2, allTracks[0]->mixFX2Pot, "FX2 D");
    drawPot(1, 2, allTracks[1]->mixFX2Pot, "FX2 2");
    drawPot(2, 2, allTracks[2]->mixFX2Pot, "FX2 3");
    drawPot(3, 2, allTracks[3]->mixFX2Pot, "FX2 4");

    drawPot(0, 3, allTracks[0]->mixFX3Pot, "FX3 D");
    drawPot(1, 3, allTracks[1]->mixFX3Pot, "FX3 2");
    drawPot(2, 3, allTracks[2]->mixFX3Pot, "FX3 3");
    drawPot(3, 3, allTracks[3]->mixFX3Pot, "FX3 4");
  }
}
void draw_mixer_FX_page2()
{

  if (change_plugin_row)
  {
    change_plugin_row = false;
    drawPot(0, 0, allTracks[4]->mixDryPot, "Dry 5");
    drawPot(1, 0, allTracks[5]->mixDryPot, "Dry 6");
    drawPot(2, 0, allTracks[6]->mixDryPot, "Dry 7");
    drawPot(3, 0, allTracks[7]->mixDryPot, "Dry 8");

    drawPot(0, 1, allTracks[4]->mixFX1Pot, "FX1 5");
    drawPot(1, 1, allTracks[5]->mixFX1Pot, "FX1 6");
    drawPot(2, 1, allTracks[6]->mixFX1Pot, "FX1 7");
    drawPot(3, 1, allTracks[7]->mixFX1Pot, "FX1 8");

    drawPot(0, 2, allTracks[4]->mixFX2Pot, "FX1 5");
    drawPot(1, 2, allTracks[5]->mixFX2Pot, "FX1 6");
    drawPot(2, 2, allTracks[6]->mixFX2Pot, "FX1 7");
    drawPot(3, 2, allTracks[7]->mixFX2Pot, "FX1 8");

    drawPot(0, 3, allTracks[4]->mixFX3Pot, "FX1 5");
    drawPot(1, 3, allTracks[5]->mixFX3Pot, "FX1 6");
    drawPot(2, 3, allTracks[6]->mixFX3Pot, "FX1 7");
    drawPot(3, 3, allTracks[7]->mixFX3Pot, "FX1 8");
  }
}
void set_mixer_FX_page1(byte row)
{
  draw_mixer_FX_page1();
  if (row == 0)
  {

    set_mixer_dry(0, 0, "Dry D", 0);
    set_mixer_dry(1, 0, "Dry 2", 1);
    set_mixer_dry(2, 0, "Dry 3", 2);
    set_mixer_dry(3, 0, "Dry 4", 3);
  }

  if (row == 1)
  {
    set_mixer_FX1(0, 1, "FX1 D", 0);
    set_mixer_FX1(1, 1, "FX1 2", 1);
    set_mixer_FX1(2, 1, "FX1 3", 2);
    set_mixer_FX1(3, 1, "FX1 4", 3);
  }

  if (row == 2)
  {
    set_mixer_FX2(0, 2, "FX2 D", 0);
    set_mixer_FX2(1, 2, "FX2 2", 1);
    set_mixer_FX2(2, 2, "FX2 3", 2);
    set_mixer_FX2(3, 2, "FX2 4", 3);
  }

  if (row == 3)
  {
    set_mixer_FX3(0, 3, "FX2 D", 0);
    set_mixer_FX3(1, 3, "FX2 2", 1);
    set_mixer_FX3(2, 3, "FX2 3", 2);
    set_mixer_FX3(3, 3, "FX2 4", 3);
  }
}
void set_mixer_FX_page2(byte row)
{
  draw_mixer_FX_page1();
  if (row == 0)
  {

    set_mixer_dry(0, 0, "Dry 5", 4);
    set_mixer_dry(1, 0, "Dry 6", 5);
    set_mixer_dry(2, 0, "Dry 7", 6);
    set_mixer_dry(3, 0, "Dry 8", 7);
  }

  if (row == 1)
  {
    set_mixer_FX1(0, 1, "FX1 5", 4);
    set_mixer_FX1(1, 1, "FX1 6", 5);
    set_mixer_FX1(2, 1, "FX1 7", 6);
    set_mixer_FX1(3, 1, "FX1 8", 7);
  }

  if (row == 2)
  {
    set_mixer_FX2(0, 2, "FX2 5", 4);
    set_mixer_FX2(1, 2, "FX2 6", 5);
    set_mixer_FX2(2, 2, "FX2 7", 6);
    set_mixer_FX2(3, 2, "FX2 8", 7);
  }

  if (row == 3)
  {
    set_mixer_FX3(0, 3, "FX3 5", 4);
    set_mixer_FX3(1, 3, "FX3 6", 5);
    set_mixer_FX3(2, 3, "FX3 7", 6);
    set_mixer_FX3(3, 3, "FX3 8", 7);
  }
}

void set_mixer_dry(byte XPos, byte YPos, const char *name, byte trackn)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      allTracks[trackn]->mixDryPot = constrain(allTracks[trackn]->mixDryPot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixDry = (float)(allTracks[trackn]->mixDryPot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
          MasterOut.fx_section.dry[i].gain(allTracks[trackn]->mixDry);
      }
      drawPot(XPos, YPos, allTracks[trackn]->mixDryPot, name);
    }
  }
}
void set_mixer_FX1(byte XPos, byte YPos, const char *name, byte trackn)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      allTracks[trackn]->mixFX1Pot = constrain(allTracks[trackn]->mixFX1Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX1 = (float)(allTracks[trackn]->mixFX1Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
          fx_1.pl[i].gain(allTracks[trackn]->mixFX1);
      }

      drawPot(XPos, YPos, allTracks[trackn]->mixFX1Pot, name);
    }
  }
}
void set_mixer_FX2(byte XPos, byte YPos, const char *name, byte trackn)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      allTracks[trackn]->mixFX2Pot = constrain(allTracks[trackn]->mixFX2Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX2 = (float)(allTracks[trackn]->mixFX2Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
          fx_2.pl[i].gain(allTracks[trackn]->mixFX2);
      }
      drawPot(XPos, YPos, allTracks[trackn]->mixFX2Pot, name);
    }
  }
}
void set_mixer_FX3(byte XPos, byte YPos, const char *name, byte trackn)
{

  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {

      allTracks[trackn]->mixFX3Pot = constrain(allTracks[trackn]->mixFX3Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX3 = (float)(allTracks[trackn]->mixFX3Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
          fx_3.pl[i].gain(allTracks[trackn]->mixFX3);
      }
      drawPot(XPos, YPos, allTracks[trackn]->mixFX3Pot, name);
    }
  }
}

// put function definitions here:
void sendNoteOn(byte Note, byte Velo, byte Channel)
{
  if (Note < NO_NOTE)
  {
    if (Channel > 0 && Channel <= 16)
      MIDI1.sendNoteOn(Note, Velo, Channel);
    if (Channel > 16 && Channel <= 32)
      usbMIDI.sendNoteOn(Note, Velo, Channel - 16);
    if (Channel > 32 && Channel <= 48)
      usbMidi1.sendNoteOn(Note, Velo, Channel - 32);
    if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
      MasterOut.noteOn(Note, Velo, Channel - (48 + 1), Note % 12);
    // Serial.printf("Note ON: channel:%d, Note: %d, Velo: %d\n", Channel, Note, Velo);
  }
}
void sendNoteOff(byte Note, byte Velo, byte Channel)
{

  if (Channel > 0 && Channel <= 16)
    MIDI1.sendNoteOff(Note, Velo, Channel);
  if (Channel > 16 && Channel <= 32)
    usbMIDI.sendNoteOff(Note, Velo, Channel - 16);
  if (Channel > 32 && Channel <= 48)
    usbMidi1.sendNoteOff(Note, Velo, Channel - 32);
  if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
    MasterOut.noteOff(Note, Velo, Channel - (48 + 1), 0);
}

void sendNoteOn_CV_Gate()
{
  if (allTracks[0]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[0]->cvNoteOn)
    {
      allTracks[0]->cvNoteOn = false;
      // byte _cvNoteValue=4095 / 64 * allTracks[0]->cvNote;
      mcp.setChannelValue(MCP4728_CHANNEL_A, 4095 / 64 * allTracks[0]->cvNote);
      digitalWrite(gateOutputPin[0], true);
      Serial.printf("cv Note On: %d\n", allTracks[0]->cvNote);
    }
  }

  if (allTracks[1]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[1]->cvNoteOn)
    {
      allTracks[1]->cvNoteOn = false;
      mcp.setChannelValue(MCP4728_CHANNEL_B, 4095 / 64 * allTracks[1]->cvNote);
      digitalWrite(gateOutputPin[1], true);
    }
  }
  if (allTracks[2]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[2]->cvNoteOn)
    {
      allTracks[2]->cvNoteOn = false;
      mcp.setChannelValue(MCP4728_CHANNEL_C, 4095 / 64 * allTracks[2]->cvNote);
      digitalWrite(gateOutputPin[2], true);
    }
  }
  if (allTracks[3]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[3]->cvNoteOn)
    {
      allTracks[3]->cvNoteOn = false;
      mcp.setChannelValue(MCP4728_CHANNEL_D, 4095 / 64 * allTracks[3]->cvNote);
      digitalWrite(gateOutputPin[3], true);
    }
  }
  if (allTracks[4]->parameter[SET_MIDICH_OUT] == 0)
  {

    if (allTracks[4]->cvNoteOn)
    {
      allTracks[4]->cvNoteOn = false;
      mcp2.setChannelValue(MCP4728_CHANNEL_A, 4095 / 64 * allTracks[4]->cvNote);
      digitalWrite(gateOutputPin[4], true);
    }
  }
  if (allTracks[5]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[5]->cvNoteOn)
    {
      allTracks[5]->cvNoteOn = false;
      mcp2.setChannelValue(MCP4728_CHANNEL_B, 4095 / 64 * allTracks[5]->cvNote);
      digitalWrite(gateOutputPin[5], true);
    }
  }
  if (allTracks[6]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[6]->cvNoteOn)
    {
      allTracks[6]->cvNoteOn = false;
      mcp2.setChannelValue(MCP4728_CHANNEL_C, 4095 / 64 * allTracks[6]->cvNote);
      digitalWrite(gateOutputPin[6], true);
    }
  }
  if (allTracks[7]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[7]->cvNoteOn)
    {
      allTracks[7]->cvNoteOn = false;
      mcp2.setChannelValue(MCP4728_CHANNEL_D, 4095 / 64 * allTracks[7]->cvNote);
      digitalWrite(gateOutputPin[7], true);
    }
  }
}
void sendNoteOff_CV_Gate()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->parameter[SET_MIDICH_OUT] == 0)
    {
      if (allTracks[i]->cvNoteOff)
      {
        allTracks[i]->cvNoteOff = false;
        digitalWrite(gateOutputPin[i], false);
        Serial.println("cv Note Off");
      }
    }
  }
}

void sendControlChange(byte control, byte value, byte Channel)
{
  if (value < 128 && control < 128)
  {
    Serial.printf("send cc Channel: %d, Control: %d, value: %d\n", Channel, control, value);
    if (Channel > 0 && Channel <= 16)
      MIDI1.sendControlChange(control, value, Channel);
    if (Channel > 16 && Channel <= 32)
      usbMIDI.sendControlChange(control, value, Channel - 16);
    if (Channel > 32 && Channel <= 48)
      usbMidi1.sendControlChange(control, value, Channel - 32);
  }
}
void sendClock()
{
  MIDI1.sendClock();
  usbMIDI.sendRealTime(usbMIDI.Clock);
  usbMidi1.sendRealTime(usbMIDI.Clock);
}

void myNoteOn(byte channel, byte note, byte velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
  if (channel >= 9)
    sendNoteOn(note, velocity, channel);
  Serial.printf("note: %d, velo: %d, channel: %d\n ", note, velocity, channel);
}
void myNoteOff(byte channel, byte note, byte velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
  if (channel >= 9)
    sendNoteOff(note, velocity, channel);
}

void get_infobox_background()
{  infoboxTimeAtPress = millis();
  if (infoboxShow&&infoboxTimeAtPress- infoboxTimeAtCall >= infoboxWaitingTime)
  {
    infoboxClear=true;
    infoboxShow=false;
    //infoboxTimeAtPress=0;
  }
}
void set_infobox_background(int _DisplayTime)
{
  infoboxWaitingTime = _DisplayTime;
  tft.fillRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_BLACK);
  tft.drawRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_WHITE);
  tft.setFont(Arial_10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET);
   infoboxTimeAtCall = millis();
   infoboxShow = true;
  
}
void set_infobox_next_line(byte _lineNumber)
{
  tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET + (20 * _lineNumber));
}
void reset_infobox_background()
{
 // tft.updateScreenAsync();
  if (infoboxClear)
  {
    infoboxClear = false;

    clearWorkSpace();
    change_plugin_row = true;
  }
}

void mouse(byte deltaX, byte maxY)
{

  if (mouse1.available())
  {
    if (mouse1.getButtons() == 4) // left = 1, right = 2, scroll = 4
      neotrellisPressed[TRELLIS_POTROW] = true;

    // moving
    pixelTouchX = constrain(pixelTouchX + mouse1.getMouseX() * deltaX, 0, 304);
    static int countmouseY;
    int mouseY;
    countmouseY = (countmouseY + mouse1.getMouseY());
    if (countmouseY % 4 == 0)
      mouseY = mouse1.getMouseY();
    else
      mouseY = 0;
    gridTouchY = constrain((gridTouchY + mouseY), 0, maxY);
    // input for sequencer
    if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
      if (pixelTouchX >= 18 * STEP_FRAME_W)
      {
        if (gridTouchY == 6 && mouse1.getWheel() != 0)
        {
          enc_moved[0] = true;
          encoded[0] = mouse1.getWheel();
        }
        if (gridTouchY == 8 && mouse1.getWheel() != 0)
        {
          enc_moved[1] = true;
          encoded[1] = mouse1.getWheel();
        }
        if (gridTouchY == 10 && mouse1.getWheel() != 0)
        {
          enc_moved[2] = true;
          encoded[2] = mouse1.getWheel();
        }
        if (gridTouchY == 12 && mouse1.getWheel() != 0)
        {
          enc_moved[3] = true;
          encoded[3] = mouse1.getWheel();
        }
      }
      // buttons
      if (mouse1.getButtons() == 1) // left = 1, right = 2, scroll = 4
        neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
    }
    // inputs for arranger
    if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
    {
      if (pixelTouchX >= 18 * STEP_FRAME_W)
      {
        if (gridTouchY == 6 && mouse1.getWheel() != 0)
        {
          enc_moved[0] = true;
          encoded[0] = mouse1.getWheel();
        }
        if (gridTouchY == 8 && mouse1.getWheel() != 0)
        {
          enc_moved[1] = true;
          encoded[1] = mouse1.getWheel();
        }
        if (gridTouchY == 10 && mouse1.getWheel() != 0)
        {
          enc_moved[2] = true;
          encoded[2] = mouse1.getWheel();
        }
        if (gridTouchY == 12 && mouse1.getWheel() != 0)
        {
          enc_moved[3] = true;
          encoded[3] = mouse1.getWheel();
        }
      }
      else
      {

        if (mouse1.getWheel() != 0)
        {
          enc_moved[2] = true;
          encoded[2] = mouse1.getWheel();
        }
        if (mouse1.getButtons() == 1)
        {
          enc_moved[2] = true;
          encoded[2] = 0;
        }
        if (mouse1.getButtons() == 2)
        {
          enc_moved[2] = true;
          encoded[2] = MAX_CLIPS - 1;
        }
      }
    }
    mouse1.mouseDataClear();
  }

  // show Mouse Infobox
}
void detect_USB_device()
{
  static bool deviceConnected = false;
  // if connected
  if (usbMidi1.idProduct() != 0 && !deviceConnected)
  {
    set_infobox_background(750);

    deviceConnected = true;
    tft.print("USB Device connected: ");
    set_infobox_next_line(1);
    tft.printf("%s: %s", usbMidi1.manufacturer(), usbMidi1.product());
  }
  // if disconnected
  else if (usbMidi1.idProduct() == 0 && deviceConnected)
  {
    set_infobox_background(750);
    tft.println("USB Device disconnected");
    deviceConnected = false;

    // remove infobox
  }

  reset_infobox_background();
}
void detect_mouse()
{
  static bool deviceConnected = false;
  if (mouse1.available() && !deviceConnected)
  {
    set_infobox_background(750);

    deviceConnected = true;
    tft.print("Mouse connected: ");
  }
  // if disconnected
  else if (!mouse1.available() && deviceConnected)
  {
    deviceConnected = false;
    set_infobox_background(750);
    tft.println("Mouse disconnected");
  }
  reset_infobox_background();
}

// define a callback for key presses
TrellisCallback blink(keyEvent evt)
{

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    neotrellisPressed[evt.bit.NUM] = true;
    updateTFTScreen = true;
    // change_plugin_row = true;
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    neotrellisPressed[evt.bit.NUM] = false;
  }
  neotrellis_show();
  return 0;
}

void neotrellis_setup()
{
  for (int y = 0; y < Y_DIM; y++)
  {
    for (int x = 0; x < X_DIM; x++)
    {
      // activate rising and falling edges on all keys
      neotrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      neotrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      neotrellis.registerCallback(x, y, blink);
    }
  }
  // INT pin requires a pullup
  // pinMode(NEOTRELLIS_INT_PIN, INPUT);
  // digitalWrite(NEOTRELLIS_INT_PIN, HIGH);

  trellisMain.setup();
  neotrellis_assign_start_buffer();
}
void neotrellis_assign_start_buffer()
{

  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->muted)
      neotrellis_set_control_buffer(0, i + 4, TRELLIS_BLACK);
    else
      neotrellis_set_control_buffer(0, i + 4, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {

    if (allTracks[i]->soloed)
      neotrellis_set_control_buffer(1, i + 4, TRELLIS_ORANGE);
    else
      neotrellis_set_control_buffer(1, i + 4, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->recordState)
      neotrellis_set_control_buffer(2, i + 4, TRELLIS_RED);
    else
      // trellis.setPixelColor(nr, TRELLIS_1); // off falling
      neotrellis_set_control_buffer(2, i + 4, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->performIsActive)
      neotrellis_set_control_buffer(3, i + 4, TRELLIS_WHITE);
    else
      neotrellis_set_control_buffer(3, i + 4, trellisTrackColor[i]);
  }
  neotrellis_set_control_buffer(0, 0, TRELLIS_ORANGE);
  neotrellis_set_control_buffer(1, 0, TRELLIS_RED);
  neotrellis_set_control_buffer(2, 0, TRELLIS_GREEN);
  neotrellis_set_control_buffer(3, 0, TRELLIS_OLIVE);
  neotrellis_set_control_buffer(0, 3, TRELLIS_WHITE);
  neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);

  trellisMain.assign_start_buffer(lastPotRow);
  /*
  for (int i = 0; i < NUM_TRACKS; i++)
 {
   for (int s = 0; s < NUM_STEPS; s++)
   {
      byte nr = s + (i*NUM_STEPS);
     if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
       neotrellis.setPixelColor(allTracks[i]->parameter[SET_CLIP2_EDIT], i, trellis_get_main_buffer(trellisScreen, s, i));
     else
       neotrellis.setPixelColor(s, i, trellis_get_main_buffer(trellisScreen, s, i));

     // if (trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
     // trellis.setPixelColor(s, i, trellis_get_arranger_buffer(arrangerpage, s, i));
   }
 }
 */
  neotrellis_show();
}
void trellis_read()
{
  // If a button was just pressed or released...
  if (trellis.readSwitches())
  {
    // go through every button
    for (uint8_t i = 0; i < numKeys; i++)
    {
      // if it was pressed, turn it on
      if (trellis.justPressed(i))
      {
        int _nr = (i % 4) + 4 * (i / 16) + (i / 4 % 4) * 16;
        if (i > 63)
        {
          _nr = _nr + 48;
        }

        updateTFTScreen = true;

        trellisPressed[_nr] = true;
        Serial.print("nr");
        Serial.println(_nr);
        // trellis.setLED(i);
        break;
      }
      // if it was released, turn it off
      if (trellis.justReleased(i))
      {
        int _nr = (i % 4) + 4 * (i / 16) + (i / 4 % 4) * 16;
        if (i > 63)
        {
          _nr = _nr + 48;
        }
        trellisPressed[_nr] = false;
        //  Serial.print("^");
        // Serial.println(i);
        // trellis.clrLED(i);
        // trellis.writeDisplay();
      }
    }
    // tell the trellis to set the LEDs we requested
    // trellis.writeDisplay();
  }
}

void neotrellis_set_control_buffer(int _x, int _y, int _color)
{
  trellisControllBuffer[_x][_y] = _color;
  neotrellis.setPixelColor(_x, _y, _color);
  neotrellis.show();
  // Serial.println("set control buffer");
}
int neotrellis_get_control_buffer(int _x, int _y)
{
  // Serial.println("get control buffer");

  return trellisControllBuffer[_x][_y];
}
void neotrellis_recall_control_buffer()
{

  for (int x = 0; x < TRELLIS_CONTROL_X_DIM; x++)
  {
    for (int y = 0; y < TRELLIS_CONTROL_Y_DIM; y++)
    {
      neotrellis.setPixelColor(x, y, trellisControllBuffer[x][y]);
    }
  }
  // Serial.println("recall controll buffer");

  // neotrellis_show();
}
void trellis_set_main_buffer(int _page, int _x, int _y, int color)
{
  trellisMain.trellis_set_main_buffer(_page, _x, _y, color);
}
void neo_trellis_set_brightness()
{
  // set overall brightness for all pixels
  if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (neotrellisPressed[TRELLIS_BUTTON_LEFT])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(5);
        }
      }
      trellis.setBrightness(0);
    }
    if (neotrellisPressed[TRELLIS_BUTTON_RIGHT])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(20);
        }
      }
      trellis.setBrightness(3);
    }
    if (neotrellisPressed[TRELLIS_BUTTON_UP])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(50);
        }
      }
      trellis.setBrightness(6);
    }
    if (neotrellisPressed[TRELLIS_BUTTON_DOWN])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(220);
        }
      }
      trellis.setBrightness(15);
    }
  }
}

void neotrellis_show()
{
  neotrellis.show();
}

// 1st row
void neotrellis_set_potRow()
{
  if (neotrellisPressed[TRELLIS_POTROW])
  {
    neotrellisPressed[TRELLIS_POTROW] = false;
    change_plugin_row = true;
    lastPotRow++;
    if (lastPotRow >= 4)
    {
      lastPotRow = 0;
    }
    tft.fillRect(70, 0, 10, 16, ILI9341_DARKGREY);
    // neotrellisPressed[TRELLIS_POTROW] = false;
    // Serial.printf("potrwo=%d\n", lastPotRow);
  }
}
void neo_trellis_save_load()
{
  if (neotrellisPressed[TRELLIS_BUTTON_RECORD])
  {
    for (int i = 0; i < MAX_SONGS; i++)
    {
      // neotrellis.setPixelColor(i, 0, TRELLIS_ORANGE);
      // neotrellis.setPixelColor(i, 1, TRELLIS_GREEN);
      // neotrellis_show();
      trellis.setLED(trellisMain.TrellisLED[i]);
      trellis.setLED(trellisMain.TrellisLED[i + TRELLIS_PADS_X_DIM]);
      trellis.writeDisplay();
      if (trellisMain.trellisPressed[i])
      {
        trellis.clrLED(trellisMain.TrellisLED[i]);
        trellis.clrLED(trellisMain.TrellisLED[i + TRELLIS_PADS_X_DIM]);
        trellis.writeDisplay();

        byte _songNr = i;
        set_infobox_background(750);
        tft.printf("Saved song: %s ", songNames[i]);
        reset_infobox_background();
        allTracks[0]->save_track(_songNr);
        allTracks[1]->save_track(_songNr);
        allTracks[2]->save_track(_songNr);
        allTracks[3]->save_track(_songNr);
        allTracks[4]->save_track(_songNr);
        allTracks[5]->save_track(_songNr);
        allTracks[6]->save_track(_songNr);
        allTracks[7]->save_track(_songNr);
        myClock.save_clock(_songNr);
        trellisMain.trellisPressed[i] = false;
        // change_plugin_row = true;
        neotrellisPressed[TRELLIS_BUTTON_RECORD] = false;
        updateTFTScreen = true;
        break;
      }
      if (trellisMain.trellisPressed[i + TRELLIS_PADS_X_DIM])
      {
        byte _songNr = i;
        Serial.printf("load song: %d\n", _songNr);
        set_infobox_background(750);
        tft.printf("loaded song: %s ", songNames[_songNr]);
        reset_infobox_background();
        allTracks[0]->load_track(_songNr);
        allTracks[1]->load_track(_songNr);
        allTracks[2]->load_track(_songNr);
        allTracks[3]->load_track(_songNr);
        allTracks[4]->load_track(_songNr);
        allTracks[5]->load_track(_songNr);
        allTracks[6]->load_track(_songNr);
        myClock.load_clock(_songNr);
        neotrellis_setup();
        trellisMain.trellisPressed[i] = false;
        updateTFTScreen = true;

        neotrellisPressed[TRELLIS_BUTTON_RECORD] = false;

        break;
      }

      // allTracks[7]->load_track();
    }
  }
}
void neotrellis_start_clock()
{
  if (neotrellisPressed[TRELLIS_START_CLOCK] /*|| enc_button[2]*/)
  {
    myClock.set_start();
    // Serial.println("Play");
    neotrellisPressed[TRELLIS_START_CLOCK] = false;
    enc_button[2] = false;
  }
}
void neotrellis_stop_clock()
{
  if (neotrellisPressed[TRELLIS_STOP_CLOCK] /*|| enc_button[3]*/)
  {
    myClock.set_stop();
    // Serial.println("Stop");
    neotrellisPressed[TRELLIS_STOP_CLOCK] = false;
    enc_button[3] = false;
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      allTracks[i]->internal_clock = -1;
      allTracks[i]->internal_clock_bar = -1;
      allTracks[i]->external_clock_bar = -1;
    }
  }
}

// 2nd row
void neotrellis_SetCursor(byte maxY)
{

  if (neotrellisPressed[TRELLIS_BUTTON_LEFT]) //..move the cursor left
  {
    pixelTouchX = constrain(pixelTouchX - 1, 0, 320);
    neotrellisPressed[TRELLIS_BUTTON_LEFT] = false;
  }
  if (neotrellisPressed[TRELLIS_BUTTON_RIGHT]) //..move the cursor right
  {
    pixelTouchX = constrain(pixelTouchX + 1, 0, 320);
    neotrellisPressed[TRELLIS_BUTTON_RIGHT] = false;
  }
  if (neotrellisPressed[TRELLIS_BUTTON_UP]) //..move the cursor up
  {
    gridTouchY = constrain(gridTouchY - 1, 0, maxY);
    neotrellisPressed[TRELLIS_BUTTON_UP] = false;
  }

  if (neotrellisPressed[TRELLIS_BUTTON_DOWN]) //..move the cursor down
  {
    gridTouchY = constrain(gridTouchY + 1, 0, maxY);
    neotrellisPressed[TRELLIS_BUTTON_DOWN] = false;
  }
}

// 3rd row
void neotrellis_show_piano()
{
  if (neotrellisPressed[TRELLIS_BUTTON_PIANO])
  {
    trellisScreen = TRELLIS_SCREEN_PIANO;
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        neotrellisPressed[TRELLIS_BUTTON_PIANO] = false;
        trellisMain.trellisPianoTrack = i;
        int _color = TRELLIS_BLACK;
        for (int x = 0; x < NUM_STEPS; x++)
        {
          for (int y = 0; y < NUM_TRACKS; y++)
          {
            int _nr = x + (y * NUM_STEPS);
            if (_nr % 12 == 0)
            {
              _color = trellisTrackColor[trellisMain.trellisPianoTrack];
              if (allTracks[trellisMain.trellisPianoTrack]->get_recordState())
                _color = TRELLIS_RED;
            }
            else
              _color = TRELLIS_BLACK;
            trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PIANO, x, y, _color);
          }
        }
        break;
      }
    }
  }
}

void neotrellis_show_tft_seqMode()
{
  if (neotrellisPressed[TRELLIS_BUTTON_SEQMODE])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        show_active_track();
        allTracks[active_track]->draw_sequencer_modes(allTracks[active_track]->parameter[SET_SEQ_MODE]);
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER_MODES;
        neotrellis_set_control_buffer(3, 2, trellisTrackColor[active_track]);
        // Serial.println("SeqMode selected");
        // trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, 3, 2, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}

// 4th row
void trellis_show_arranger()
{
  if (neotrellisPressed[TRELLIS_BUTTON_ARRANGER])
  {
    if (trellisScreen != TRELLIS_SCREEN_SONGPAGE_SELECTION)
    {
      trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
      trellis.clear();
      for (int i = 0; i < NUM_STEPS; i++)
      {
        trellis.setLED(trellisMain.TrellisLED[i]);
        trellis.writeDisplay();
      }
    }
    for (int i = 0; i < NUM_STEPS; i++)
    {
      if (trellisMain.trellisPressed[i])
      {
        trellis.clear();
        trellis.writeDisplay();

        trellisMain.trellisPressed[i] = false;
        neotrellisPressed[TRELLIS_BUTTON_ARRANGER] = false;
        trellisScreen = i + TRELLIS_SCREEN_ARRANGER_1;
        arrangerpage = i;

        activeScreen = INPUT_FUNCTIONS_FOR_ARRANGER;
        // trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, i, 0, TRELLIS_BLACK);
        //  neotrellis_show();
        //   Serial.printf("songpage= %d\n", _key);
        gridSongMode(arrangerpage);
        Serial.println("try to recall");
        trellisMain.trellis_recall_main_buffer(trellisScreen);
        break;
      }
    }
  }
}

void trellis_show_tft_mixer()
{
  if (neotrellisPressed[TRELLIS_BUTTON_MIXER])
  {
    if (trellisScreen != TRELLIS_SCREEN_SONGPAGE_SELECTION)
    {
      trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
      trellis.clear();
      for (int i = 0; i < 7; i++)
      {
        trellis.setLED(trellisMain.TrellisLED[i]);
        trellis.writeDisplay();
      }
    }

    if (trellisMain.trellisPressed[0])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[0] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER1;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER1;
      clearWorkSpace();
      draw_mixer();
    }
    if (trellisMain.trellisPressed[1])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[1] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER2;
      clearWorkSpace();
      draw_mixer_FX_page1();

      trellisMain.trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisMain.trellisPressed[2])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[2] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER3;
      clearWorkSpace();
      draw_mixer_FX_page2();

      trellisMain.trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisMain.trellisPressed[3])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[3] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      clearWorkSpace();
      fx_1.draw_plugin();

      trellisMain.trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisMain.trellisPressed[4])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[4] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX2;
      clearWorkSpace();
      fx_2.draw_plugin();

      trellisMain.trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisMain.trellisPressed[5])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[5] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX3;
      clearWorkSpace();
      fx_3.draw_plugin();

      trellisMain.trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisMain.trellisPressed[6])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisMain.trellisPressed[6] = false;
      trellisScreen = TRELLIS_SCREEN_PERFORM;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_PERFORM;

      clearWorkSpace();
      // activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      // clearWorkSpace();
      trellisMain.set_perform_page(lastPotRow);
      trellisMain.trellis_recall_main_buffer(TRELLIS_SCREEN_PERFORM);
    }
  }
}

void neotrellis_perform_set_active()
{
  if (trellisScreen == TRELLIS_SCREEN_PERFORM)
  {

    for (int x = 0; x < NUM_STEPS; x++)
    {
      for (int y = 0; y < NUM_TRACKS; y++)
      {
        trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, x, y, TRELLIS_BLACK);
        // trellis.clrLED(TrellisLED[x + (y * TRELLIS_PADS_X_DIM)]);
        byte _nr = x + (trellisMain.trellisPerformIndex[x] * TRELLIS_PADS_X_DIM);
        // if (trellisPressed[_nr]){
        trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, x, trellisMain.trellisPerformIndex[x], TRELLIS_WHITE);
        // break;}
        //  trellis.setLED(TrellisLED[_nr]);
      }
    }
    trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4, 7, TRELLIS_BLUE);
    trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 1, 7, TRELLIS_RED);
    trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
    trellisMain.trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 3, 7, TRELLIS_WHITE);
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      if (neotrellisPressed[3 + ((t + 4) * X_DIM)])
      {

        if (!allTracks[t]->performIsActive)
        {
          neotrellis_set_control_buffer(3, t + 4, TRELLIS_WHITE);
          allTracks[t]->performIsActive = true;
          neotrellisPressed[3 + ((t + 4) * X_DIM)] = false;
          /*
          for (int i = 0; i < NUM_PLUGINS; i++)
          {
            if (allTracks[t]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
            {
              fx_1.pl[i].gain(1);
              fx_2.pl[i].gain(1);
              fx_3.pl[i].gain(1);
            }
          }
          */
          break;
        }
        else if (allTracks[t]->performIsActive)
        {
          neotrellis_set_control_buffer(3, t + 4, trellisTrackColor[t]);
          allTracks[t]->performIsActive = false;
          neotrellisPressed[3 + ((t + 4) * X_DIM)] = false;
          /*
          for (int i = 0; i < NUM_PLUGINS; i++)
          {
            if (allTracks[t]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
            {
              fx_1.pl[i].gain(0);
              fx_2.pl[i].gain(0);
              fx_3.pl[i].gain(0);
            }
          }
          */
          // Serial.println("set unmute");
          break;
        }
      }
    }
  }
}

void neotrellis_show_tft_plugin()
{
  if (neotrellisPressed[TRELLIS_BUTTON_PLUGIN])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        show_active_track();
        change_plugin_row = true;
        // allTracks[active_track]->draw_MIDI_CC_screen();
        activeScreen = INPUT_FUNCTIONS_FOR_PLUGIN;
        neotrellis_set_control_buffer(2, 3, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}
void neo_trellis_select_trackClips()
{
  if (neotrellisPressed[TRELLIS_BUTTON_SEQUENCER])
  { // byte _activeClip=allTracks[y]->parameter[SET_CLIP2_EDIT];
    trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
    trellis.clear();

    for (int y = 0; y < NUM_TRACKS; y++)
    {
      trellis.setLED(trellisMain.TrellisLED[allTracks[y]->parameter[SET_CLIP2_EDIT] + (y * TRELLIS_PADS_X_DIM)]);
    }
    trellis.writeDisplay();
    // neotrellis_show();
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      for (int x = 0; x <= NUM_USER_CLIPS; x++)
      {
        if (trellisMain.trellisPressed[x + (TRELLIS_PADS_X_DIM * y)])
        {
          trellis.clear();
          trellis.writeDisplay();
          neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
          trellisMain.trellisPressed[x + (TRELLIS_PADS_X_DIM * y)] = false;
          // neotrellis_show();
          active_track = y;
          show_active_track();
          activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
          allTracks[y]->parameter[SET_CLIP2_EDIT] = x;
          updateTFTScreen = true;

          change_plugin_row = true;
          allTracks[active_track]->drawStepSequencerStatic();
          allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
          allTracks[active_track]->clear_notes_in_grid();
          allTracks[active_track]->draw_notes_in_grid();
          neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);

          trellisScreen = allTracks[active_track]->parameter[SET_CLIP2_EDIT];
          trellisMain.trellis_recall_main_buffer(trellisScreen);
          // trellisScreen = 0;
          break;
        }

        else if (neotrellisPressed[3 + (X_DIM * (y + 4))])
        {
          trellis.clear();
          trellis.writeDisplay();
          neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
          neotrellisPressed[3 + (X_DIM * (y + 4))] = false;
          // trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, x, y, TRELLIS_BLACK);
          //  neotrellis_show();
          active_track = y;
          show_active_track();
          updateTFTScreen = true;
          change_plugin_row = true;
          activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
          trellisScreen = allTracks[active_track]->parameter[SET_CLIP2_EDIT];
          // trellisScreen = 0;

          allTracks[active_track]->drawStepSequencerStatic();
          allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
          // allTracks[active_track]->clear_notes_in_grid();
          allTracks[active_track]->draw_notes_in_grid();
          neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);
          trellisMain.trellis_recall_main_buffer(trellisScreen);
          break;
        }
      }
    }
  }
}

// colums
void neotrellis_set_mute()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[((i + 4) * X_DIM)])
    {
      neotrellisPressed[((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->muted)
      {
        neotrellis_set_control_buffer(0, (i + 4), TRELLIS_BLACK);
        allTracks[i]->muted = true;
        Serial.print("set mute");
        Serial.println(i);
        break;
      }
      else if (allTracks[i]->muted)
      {
        neotrellis_set_control_buffer(0, (i + 4), TRELLIS_1);
        allTracks[i]->muted = false;
        Serial.print("set unmute");
        Serial.println(i);
        break;
      }
    }
  }
}
void neotrellis_set_solo()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[1 + ((i + 4) * X_DIM)])
    {
      neotrellisPressed[1 + ((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->soloed)
      {
        neotrellis_set_control_buffer(1, i + 4, TRELLIS_ORANGE);
        allTracks[i]->soloed = true;
        allTracks[i]->muteThruSolo = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {

          if (!allTracks[o]->soloed && o != i)
          {
            allTracks[o]->muteThruSolo = true;
          }
        }
        // Serial.println("set solo");
        break;
      }
      else if (allTracks[i]->soloed)
      {
        neotrellis_set_control_buffer(1, i + 4, TRELLIS_1);
        allTracks[i]->soloed = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (!allTracks[o]->soloed && o != i)
          {
            allTracks[o]->muteThruSolo = false;
          }
        }
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (allTracks[o]->soloed)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              allTracks[s]->muteThruSolo = true;
            }
            allTracks[o]->muteThruSolo = false;
          }
        }
        // Serial.println("set unsolo");
        break;
      }
    }
  }
}
void neotrellis_set_fast_record()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[2 + ((i + 4) * X_DIM)])
    {
      neotrellisPressed[2 + ((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->recordState)
      {
        allTracks[i]->recordState = true;
        neotrellis_set_control_buffer(2, i + 4, TRELLIS_RED);
        // trellis.setPixelColor(18, i, TRELLIS_RED);
        break;
      }

      else if (allTracks[i]->recordState)
      {
        allTracks[i]->recordState = false;
        neotrellis_set_control_buffer(2, (i + 4), TRELLIS_1);
        // trellis.setPixelColor(18, i, TRELLIS_1);
        break;
      }

      // break;
    }
  }
}

void TrellisForMainGrid::trellis_play_mixer()
{
  if (activePage == TRELLIS_SCREEN_MIXER1)
  {
    for (int t = 0; t < NUM_TRACKS; t++)
    {

      for (int s = 0; s < NUM_STEPS; s++)
      {
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER1, s, t, TRELLIS_BLACK);
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER1, allTracks[t]->mixGainPot / 8, t, trackColor[t]);
        byte _nr = s + (t * TRELLIS_PADS_X_DIM);
        if (trellisPressed[_nr])
        {
          byte _gain[NUM_STEPS] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 111, 119, 127};
          trellisPressed[_nr] = false;

          allTracks[t]->mixGainPot = _gain[s];
          break;
        }
      }
    }
  }
  if (activePage == TRELLIS_SCREEN_MIXER)
  {
    float _gain[4] = {0, 0.30, 0.60, 1};

    for (int t = 0; t < NUM_TRACKS; t++)
    {
      // if (allTracks[t]->parameter[SET_MIDICH_OUT] >= (NUM_MIDI_OUTPUTS + 1))
      {
        for (int s = 0; s < NUM_STEPS; s++)
        {

          byte _nr = s + (t * TRELLIS_PADS_X_DIM);

          if (trellisPressed[_nr])
          {
            trellisPressed[_nr] = false;
            for (int c = 0; c < 4; c++)
            {

              if (_nr % TRELLIS_PADS_X_DIM == c)
              {
                Serial.printf("dry channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                MasterOut.fx_section.dry[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixDryPot = (c * 42);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 0, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 1, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 2, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 3, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c, t, TRELLIS_PINK);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("Dry Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }
              if (_nr % TRELLIS_PADS_X_DIM == c + 4)
              {
                Serial.printf("fx1 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_1.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixFX1Pot = (c * 42);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 4, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 5, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 6, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 7, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 4, t, TRELLIS_OLIVE);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("FX1 Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }
              if (_nr % TRELLIS_PADS_X_DIM == c + 8)
              {
                Serial.printf("fx2 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_2.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixFX2Pot = (c * 42);

                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 8, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 9, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 10, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 11, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 8, t, TRELLIS_AQUA);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("FX2 Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }

              if (_nr % TRELLIS_PADS_X_DIM == c + 12)
              {
                Serial.printf("fx3 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_3.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixFX3Pot = (c * 42);

                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 12, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 13, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 14, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 15, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 12, t, TRELLIS_ORANGE);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("FX3 Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }
            }
            // change_plugin_row=true;
          }
        }
      }
    }
  }
}
void TrellisForMainGrid::trellis_perform()
{
  if (activePage == TRELLIS_SCREEN_PERFORM)
  {
    set_perform_page(lastPotRow);
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      for (int i = 0; i < NUM_STEPS; i++)
      {
        int _nr = i + (t * TRELLIS_PADS_X_DIM);
        if (trellisPressed[_nr])

        {
          // change_plugin_row = true;
          trellisPressed[_nr] = false;
          if (_nr % TRELLIS_PADS_X_DIM == 0)
          {

            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->mixGainPot = 127 - (t * 16);
                trellisPerformIndex[0] = t;
                // sendControlChange(performCC[0], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
            }
            set_infobox_background(750);
            tft.printf("Master Vol =  %d ", 127 - (t * 16));
            // set_infobox_next_line(1);
            // tft.printf("send CC%d =  %d ", performCC[0],127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 1)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {

                Serial.printf("dry channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixDryPot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  MasterOut.fx_section.dry[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[1], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[1] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("dry Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[1], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 2)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                Serial.printf("fx1 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX1Pot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  fx_1.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[2], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[2] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("FX1 Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[2], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 3)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                Serial.printf("fx2 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX2Pot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  fx_2.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[3], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[3] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("FX2 Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[3], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 4)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                Serial.printf("fx3 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX3Pot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  fx_3.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[4], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[4] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("FX3 Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[4], 127 - (t * 16));
            reset_infobox_background();
          }

          if (_nr % TRELLIS_PADS_X_DIM == 5)
          {
            fx_1.potentiometer[fx_1.presetNr][0] = 127 - (t * 16);
            fx_1.freeverb.roomsize((float)map(t, 0, 8, 0, 1.00));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[5], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[5] = t;
            set_infobox_background(750);
            tft.printf("FX1 Roomsize =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[5], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 6)
          {

            fx_1.potentiometer[fx_1.presetNr][1] = 127 - (t * 16);
            fx_1.freeverb.damping((float)map(t, 0, 8, 0, 1.00));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[6], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[6] = t;
            set_infobox_background(750);
            tft.printf("FX1 Damping =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[6], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 7)
          {
            fx_2.potentiometer[fx_2.presetNr][0] = 127 - (t * 16);

            fx_2.bitcrusher.bits(map(t, 0, 8, 16, 0));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[7], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[7] = t;
            set_infobox_background(750);
            tft.printf("FX2 BitDepth =  %d ", map(t, 0, 8, 16, 0));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[7], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 8)
          {
            int _rate[Y_DIM] = {44100, 22050, 11025, 5512, 2756, 1378, 1022, 689};
            fx_2.potentiometer[fx_2.presetNr][1] = 127 - (t * 16);
            fx_2.bitcrusher.sampleRate(_rate[t]);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[8], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[8] = t;
            set_infobox_background(750);
            tft.printf("FX2 SmplRate =  %d ", _rate[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[8], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 9)
          {
            fx_3.potentiometer[fx_3.presetNr][0] = 127 - (t * 16);
            fx_3.delay.delay(0, 500 / (t + 1));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[9], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[9] = t;
            set_infobox_background(750);
            tft.printf("FX3 DlyTime =  %d ", 500 / (t + 1));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[9], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 10)
          {
            fx_3.potentiometer[fx_3.presetNr][1] = 127 - (t * 16);
            fx_3.delayMixer.gain(1, t / 8.00);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[10], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[10] = t;
            set_infobox_background(750);
            tft.printf("FX3 Feedback =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[10], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 11)
          {
            int frequency = note_frequency[t * 16] * tuning;
            MasterOut.finalFilter.frequency(frequency);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[11], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[11] = t;
            set_infobox_background(750);
            tft.printf("Fltr Freq =  %d ", frequency);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[11], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 12)
          {
            MasterOut.finalFilter.resonance(map(t, 0, 8, 0, 5.00));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[12], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[12] = t;
            set_infobox_background(750);
            tft.printf("Fltr Reso =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[12], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 13)
          {
            byte _clipLength[Y_DIM]{96, 72, 60, 48, 36, 24, 12, 6};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                {
                  allTracks[s]->parameter[SET_SEQUENCE_LENGTH] = _clipLength[t];
                  if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  {
                    sendControlChange(performCC[13], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                  }
                }
              }

              if (t == 0)
              {
                allTracks[s]->internal_clock = 0;
                allTracks[s]->internal_clock_bar = myClock.barTick;
                allTracks[s]->external_clock_bar = myClock.barTick;
              }
            }
            trellisPerformIndex[13] = t;
            set_infobox_background(750);
            tft.printf("Clip Length =  %d ", _clipLength[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[13], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 14)
          {
            byte _stepDivision[Y_DIM]{1, 2, 3, 4, 6, 8, 12, 16};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->parameter[SET_STEP_DIVIVISION] = _stepDivision[t];
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[14], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
              if (t == 0)
              {
                allTracks[s]->internal_clock = 0;
                allTracks[s]->internal_clock_bar = myClock.barTick;
                allTracks[s]->external_clock_bar = myClock.barTick;
              }
            }
            trellisPerformIndex[14] = t;
            set_infobox_background(750);
            tft.printf("Step Div =  %d ", _stepDivision[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[14], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 15)
          {
            int _offset[Y_DIM]{0, -12, -4, -2, 2, 4, 6, 12};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->performNoteOffset = _offset[t];
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[15], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
            }
            trellisPerformIndex[15] = t;
            set_infobox_background(750);
            tft.printf("Note Transpose =  %d ", _offset[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[15], 127 - (t * 16));
            reset_infobox_background();
          }
        }
      }
    }
  }
}
void draw_perform_page()
{
  if (change_plugin_row)
  {
    change_plugin_row = false;
    for (int i = 0; i < NUM_STEPS; i++)
    {
      drawPot(i % 4, i / 4, performCC[i], "CC");
      trellisRecall = true;
      trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, i, 7, TRELLIS_BLACK);
    }
    trellisRecall = true;
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4, 7, TRELLIS_BLUE);
    trellisRecall = true;
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 1, 7, TRELLIS_RED);
    trellisRecall = true;
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
    trellisRecall = true;
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 3, 7, TRELLIS_WHITE);
  }
}
void set_performCC(byte XPos, byte YPos, const char *name)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      byte _nr = XPos + (YPos * 4);
      performCC[_nr] = constrain(performCC[_nr] + encoded[XPos], 0, 128);
      drawPot(XPos, YPos, performCC[_nr], name);
    }
  }
}
void set_perform_page(byte row)
{
  draw_perform_page();
  if (row == 0)
  {

    set_performCC(0, 0, "CC");
    set_performCC(1, 0, "CC");
    set_performCC(2, 0, "CC");
    set_performCC(3, 0, "CC");
  }

  if (row == 1)
  {
    set_performCC(0, 1, "CC");
    set_performCC(1, 1, "CC");
    set_performCC(2, 1, "CC");
    set_performCC(3, 1, "CC");
  }

  if (row == 2)
  {
    set_performCC(0, 2, "CC");
    set_performCC(1, 2, "CC");
    set_performCC(2, 2, "CC");
    set_performCC(3, 2, "CC");
  }

  if (row == 3)
  {
    set_performCC(0, 3, "CC");
    set_performCC(1, 3, "CC");
    set_performCC(2, 3, "CC");
    set_performCC(3, 3, "CC");
  }
}
void trellis_show_tft_plugin()
{
  if (neotrellisPressed[TRELLIS_BUTTON_PLUGIN])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        trellisRecall = true;
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        show_active_track();
        change_plugin_row = true;
        // allTracks[active_track]->draw_MIDI_CC_screen();
        activeScreen = INPUT_FUNCTIONS_FOR_PLUGIN;
        neotrellis_set_control_buffer(2, 3, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}
void trellis_select_trackClips()
{
  if (neotrellisPressed[TRELLIS_BUTTON_SEQUENCER])
  { // byte _activeClip=allTracks[y]->parameter[SET_CLIP2_EDIT];
    trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
    trellis.clear();

    for (int y = 0; y < NUM_TRACKS; y++)
    {
      trellis.setLED(TrellisLED[allTracks[y]->parameter[SET_CLIP2_EDIT] + (y * TRELLIS_PADS_X_DIM)]);
    }
    trellis.writeDisplay();
    // neotrellis_show();
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      for (int x = 0; x <= NUM_USER_CLIPS; x++)
      {
        if (trellisPressed[x + (TRELLIS_PADS_X_DIM * y)])
        {
          trellis.clear();
          trellis.writeDisplay();
          neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
          trellisPressed[x + (TRELLIS_PADS_X_DIM * y)] = false;
          // neotrellis_show();
          active_track = y;
          show_active_track();
          trellisRecall = true;
          activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
          allTracks[y]->parameter[SET_CLIP2_EDIT] = x;
            updateTFTScreen = true;

          change_plugin_row = true;
          allTracks[active_track]->drawStepSequencerStatic();
          allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
          allTracks[active_track]->clear_notes_in_grid();
          allTracks[active_track]->draw_notes_in_grid();
          neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);

          trellisScreen = allTracks[active_track]->parameter[SET_CLIP2_EDIT];
          trellisRecall = true;
          trellis_recall_main_buffer(trellisScreen);
          // trellisScreen = 0;
          trellis.writeDisplay();
          break;
        }

        else if (neotrellisPressed[3 + (X_DIM * (y + 4))])
        {
          trellis.clear();
          trellis.writeDisplay();
          neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
          neotrellisPressed[3 + (X_DIM * (y + 4))] = false;
          // trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, x, y, TRELLIS_BLACK);
          //  neotrellis_show();
          active_track = y;
          show_active_track();
          updateTFTScreen = true;
          change_plugin_row = true;
          trellisRecall = true;
          activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
          trellisScreen = allTracks[active_track]->parameter[SET_CLIP2_EDIT];
          // trellisScreen = 0;

          allTracks[active_track]->drawStepSequencerStatic();
          allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
          // allTracks[active_track]->clear_notes_in_grid();
          allTracks[active_track]->draw_notes_in_grid();
          neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);
          trellis_recall_main_buffer(trellisScreen);
          break;
        }
      }
    }
  }
}
void trellis_setStepsequencer()
{
  if (trellisScreen < TRELLIS_SCREEN_SEQUENCER_CLIP_8)
  {
    if (!neotrellisPressed[TRELLIS_BUTTON_RECORD])
    {

      for (int x = 0; x < NUM_STEPS; x++)
      {
        for (int y = 0; y < NUM_TRACKS; y++)
        {
          byte _nr = x + (y * TRELLIS_PADS_X_DIM);
          if (trellisPressed[_nr])
          {
            byte trellisNote;
            byte track = _nr / NUM_STEPS;
            byte step = _nr % NUM_STEPS;
            int keyTick = (step * 6);
            // gridTouchY = 1;
            // neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
            if (gridTouchY > 0 && gridTouchY <= 12)
            {

              trellisNote = gridTouchY;
            }
            else
              trellisNote = 1;
            trellisPressed[_nr] = false;
            trellisRecall = true;
            updateTFTScreen = true;
             change_plugin_row = true;
            neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
            allTracks[track]->set_note_on_tick(keyTick, trellisNote);
            Serial.printf("step: %d, tick: %d, track: %D \n", step, keyTick, track);
            /*
            for (int v = 0; v < MAX_VOICES; v++)
            {
              if (allTracks[_key / TRELLIS_PADS_X_DIM]->get_active_note(allTracks[track]->parameter[SET_CLIP2_EDIT], keyTick, v) < NO_NOTE)
              {
                trellis_set_buffer(_key, trellisTrackColor[track]);
                break;
              }
              else if (allTracks[_key / TRELLIS_PADS_X_DIM]->get_active_note(allTracks[_key / TRELLIS_PADS_X_DIM]->parameter[SET_CLIP2_EDIT], keyTick, v) == NO_NOTE)
              {
                trellis_set_buffer(_key, TRELLIS_BLACK);
                break;
              }
            }
*/
          }
        }
      }
    }
  }
}

// colums
void trellis_set_mute()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[((i + 4) * X_DIM)])
    {
      trellisRecall = true;
      neotrellisPressed[((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->muted)
      {
        neotrellis_set_control_buffer(0, (i + 4), TRELLIS_BLACK);
        allTracks[i]->muted = true;
        Serial.print("set mute");
        Serial.println(i);
        break;
      }
      else if (allTracks[i]->muted)
      {
        neotrellis_set_control_buffer(0, (i + 4), TRELLIS_1);
        allTracks[i]->muted = false;
        Serial.print("set unmute");
        Serial.println(i);
        break;
      }
    }
  }
}
void trellis_set_solo()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[1 + ((i + 4) * X_DIM)])
    {
      trellisRecall = true;
      neotrellisPressed[1 + ((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->soloed)
      {
        neotrellis_set_control_buffer(1, i + 4, TRELLIS_ORANGE);
        allTracks[i]->soloed = true;
        allTracks[i]->muteThruSolo = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {

          if (!allTracks[o]->soloed && o != i)
          {
            allTracks[o]->muteThruSolo = true;
          }
        }
        // Serial.println("set solo");
        break;
      }
      else if (allTracks[i]->soloed)
      {
        neotrellis_set_control_buffer(1, i + 4, TRELLIS_1);
        allTracks[i]->soloed = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (!allTracks[o]->soloed && o != i)
          {
            allTracks[o]->muteThruSolo = false;
          }
        }
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (allTracks[o]->soloed)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              allTracks[s]->muteThruSolo = true;
            }
            allTracks[o]->muteThruSolo = false;
          }
        }
        // Serial.println("set unsolo");
        break;
      }
    }
  }
}
void trellis_set_fast_record()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[2 + ((i + 4) * X_DIM)])
    {
      neotrellisPressed[2 + ((i + 4) * X_DIM)] = false;
      trellisRecall = true;
      if (!allTracks[i]->recordState)
      {
        allTracks[i]->recordState = true;
        neotrellis_set_control_buffer(2, i + 4, TRELLIS_RED);
        // trellis.setPixelColor(18, i, TRELLIS_RED);
        break;
      }

      else if (allTracks[i]->recordState)
      {
        allTracks[i]->recordState = false;
        neotrellis_set_control_buffer(2, (i + 4), TRELLIS_1);
        // trellis.setPixelColor(18, i, TRELLIS_1);
        break;
      }

      // break;
    }
  }
}

// to add:
