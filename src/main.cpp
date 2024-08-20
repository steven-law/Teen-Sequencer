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
#include "Adafruit_NeoTrellis.h"
// #include <Adafruit_NeoTrellis.h>
//  #include "neoTrellis.h"
#include "hw_stuff.h"
#include "project_variables.h"
#include "ownLibs/cursor.h"
#include "ownLibs/clock.h"
#include "ownLibs/myClock.h"
#include "Track.h"
#include <Plugins/plugin_List.h>
#include <FX/fx_List.h>
#include "FX/Output.h"

#define INT_PIN 41
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP 0xFC
// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x3E, &Wire1), Adafruit_NeoTrellis(0x36, &Wire1), Adafruit_NeoTrellis(0x32, &Wire1), Adafruit_NeoTrellis(0x30, &Wire1), Adafruit_NeoTrellis(0x2F, &Wire1), Adafruit_NeoTrellis(0x46, &Wire1)},

    {Adafruit_NeoTrellis(0x3F, &Wire1), Adafruit_NeoTrellis(0x37, &Wire1), Adafruit_NeoTrellis(0x33, &Wire1), Adafruit_NeoTrellis(0x31, &Wire1), Adafruit_NeoTrellis(0x35, &Wire1), Adafruit_NeoTrellis(255, &Wire1)}

};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM / 4, X_DIM / 4);
// Adafruit_NeoTrellis control(0x46, &Wire1);
//  Adafruit_MultiTrellis control((Adafruit_NeoTrellis *)_control, 1, 1);
Adafruit_MCP4728 mcp;
Adafruit_MCP4728 mcp2;
Plugin_1 plugin_1("Strg", 18);
Plugin_2 plugin_2("1OSC", 18);
Plugin_3 plugin_3("2FM", 19);
Plugin_4 plugin_4("mDrm", 20);
Plugin_5 plugin_5("Drum", 21);
Plugin_6 plugin_6("Adtv", 22);
Plugin_7 plugin_7("Boom", 23);
Plugin_8 plugin_8("dTune", 24);
Plugin_9 plugin_9("dTune", 24);
PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9};
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
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long trellisPreviousMillis = 0; // will store last time LED was updated

// constants won't change:
const long trellisInterval = 100; // interval at which to blink (milliseconds)
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
void set_infobox_background();
void reset_infobox_background(unsigned long int waitingTime);
void set_infobox_next_line(byte _lineNumber); //_lineNumber must be bigger 0
void detect_USB_device();
void detect_mouse();
TrellisCallback blink2(keyEvent evt);
TrellisCallback blink(keyEvent evt);
uint32_t Wheel(byte WheelPos);
void trellis_static();
void trellis_setup();

void trellis_set_control_buffer(int _x, int _y, int _color);
int trellis_get_control_buffer(int _x, int _y);
void trellis_recall_control_buffer();

void trellis_set_panel_buffer(int _x, int _y, int _color);
int trellis_get_panel_buffer(int _x, int _y);
void trellis_recall_panel_buffer();

void trellis_set_main_buffer(int _page, int _x, int _y, int color);
void trellis_recall_main_buffer(int _page);
int trellis_get_main_buffer(int _page, int _x, int _y);

void trellis_setStepsequencer();

void draw_clock_option(byte x, byte v);
void drawstepPosition(int current);
void drawbarPosition(byte _bar);
void trellis_start_clock();
void trellis_stop_clock();
void trellis_set_track_record();
void trellis_set_potRow();
void trellis_SetCursor(byte maxY);
void trellis_show_clockbar(byte trackNr, byte step);
void trellis_show();

void trellis_show_tft_sequencer();
void trellis_show_tft_plugin();
void trellis_show_tft_seqMode();

void trellis_show_arranger();
void trellis_set_arranger(int _key);

void trellis_show_tft_mixer();
void trellis_play_mixer();
void trellis_show_pads();
void trellis_show_piano();
void trellis_play_piano();
void trellis_set_fast_record();
void trellis_set_mute();
void trellis_set_solo();
void trellis_perform();

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
  pinMode(INT_PIN, INPUT);
  Serial.println("DAWsketch started");
  Wire1.begin();
  if (!trellis.begin())
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

  trellisPanelBuffer = new int *[TRELLIS_CONTROL_X_DIM];
  for (int j = 0; j < TRELLIS_CONTROL_X_DIM; j++)
  {
    trellisPanelBuffer[j] = new int[TRELLIS_PANEL_Y_DIM];
  }
  for (int j = 0; j < TRELLIS_CONTROL_X_DIM; j++)
  {
    for (int k = 0; k < TRELLIS_PANEL_Y_DIM; k++)
    {
      trellisPanelBuffer[j][k] = TRELLIS_BLACK;
    }
  }
  
    tftRamInfoBox = new int *[INFO_BOX_WIDTH];
    for (int j = 0; j < INFO_BOX_WIDTH; j++)
    {
      tftRamInfoBox[j] = new int[INFO_BOX_HEIGTH];
    }
    for (int j = 0; j < INFO_BOX_WIDTH; j++)
    {
      for (int k = 0; k < INFO_BOX_HEIGTH; k++)
      {
        tftRamInfoBox[j][k] = TRELLIS_BLACK;
      }
    }
    
  gate_setup();
  myClock.setup();
  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  usbMidi1.setHandleNoteOn(myNoteOn);
  usbMidi1.setHandleNoteOff(myNoteOff);
  AudioMemory(200);
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
  trellis_setup();
}

void loop()
{
  unsigned long loopStartTime = millis();
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
  if (!digitalRead(INT_PIN))
  {
    Serial.println("reading trellis");
    trellis.read(false);
    // trellis_set_control_buffer(1,3,TRELLIS_AQUA);
    trellis.setPixelColor(23, 1, TRELLIS_AQUA);
    trellis.show();
    // delay(1);
  }
  trellis_set_control_buffer(3, 1, TRELLIS_BLACK);
  // trellis.setPixelColor(23, 1, TRELLIS_BLACK);
  // trellis.show();
  for (int i = 0; i < X_DIM * Y_DIM; i++)
  {

    trellis_set_arranger(i);
  }
  trellis_setStepsequencer();
  trellis_show_arranger();
  trellis_show_tft_sequencer();
  trellis_show_tft_seqMode();
  trellis_show_tft_plugin();
  trellis_set_mute();
  trellis_set_solo();
  trellis_set_fast_record();
  trellis_show_piano();
  trellis_play_piano();
  trellis_show_pads();
  trellis_show_tft_mixer();
  trellis_play_mixer();
  trellis_perform();

  //  trellis.read();
  //   control.read();
  if (millis() % 50 == 0)
  {
    // Serial.println(usbMidi1.idProduct());

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

    tft.fillRect(70, lastPotRow * 4, 10, 3, ILI9341_ORANGE);
  }
  input_behaviour();
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->update(pixelTouchX, gridTouchY);
    if (trellisShowClockPixel[i])
      trellis_show_clockbar(i, allTracks[i]->internal_clock / 6);

    // allTracks[i]->save_track();
    // allTracks[i]->load_track();
  }

  // Masterclock.process_MIDItick();
  // clock_to_notes();

  if (millis() % 50 == 0)
  {
    tft.updateScreenAsync();
  }

  unsigned long loopEndTime = millis();
  unsigned long trellisCurrentMillis = millis();
  if (trellisCurrentMillis - trellisPreviousMillis >= trellisInterval)
  {
    trellisPreviousMillis = trellisCurrentMillis;
    // Serial.printf("LoopTime= %d\n", loopEndTime - loopStartTime);
    //   Serial.println(trellisScreen);
  }

  if (loopEndTime - loopStartTime > 500 || millis() % 300000 == 0)
  {
    // trellis.trellisReset();
    Serial.println("restart trellis");
    trellis.begin();
    trellis_setup();
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
  if (!trellisPressed[TRELLIS_BUTTON_SHIFT])
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
  if (!trellisPressed[TRELLIS_BUTTON_SHIFT])
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
  if (!trellisPressed[TRELLIS_BUTTON_SHIFT])
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

  if (!trellisPressed[TRELLIS_BUTTON_SHIFT])
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

void sendNoteOn_CV_Gate()
{
  if (allTracks[0]->parameter[SET_MIDICH_OUT] == 0)
  {
    if (allTracks[0]->cvNoteOn)
    {
      allTracks[0]->cvNoteOn = false;
      //byte _cvNoteValue=4095 / 64 * allTracks[0]->cvNote;
      mcp.setChannelValue(MCP4728_CHANNEL_A, 4095 / 64 * allTracks[0]->cvNote);
      digitalWrite(gateOutputPin[0], true);
      Serial.printf("cv Note On: %d\n",allTracks[0]->cvNote );
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

// put function definitions here:
void sendNoteOn(byte Note, byte Velo, byte Channel)
{
  if (Channel > 0 && Channel <= 16)
    MIDI1.sendNoteOn(Note, Velo, Channel);
  if (Channel > 16 && Channel <= 32)
    usbMIDI.sendNoteOn(Note, Velo, Channel - 16);
  if (Channel > 32 && Channel <= 48)
    usbMidi1.sendNoteOn(Note, Velo, Channel - 32);
  if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
    MasterOut.noteOn(Note, Velo, Channel - (48 + 1), Note % 12);
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
void sendControlChange(byte control, byte value, byte Channel)
{
  if (control != 128)
  {
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
void mouse(byte deltaX, byte maxY)
{

  if (mouse1.available())
  {
    if (mouse1.getButtons() == 4) // left = 1, right = 2, scroll = 4
      trellisPressed[TRELLIS_POTROW] = true;

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
        trellisPressed[TRELLIS_BUTTON_ENTER] = true;
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

void get_infobox_background()
{
  currentTime = millis();
  for (int w = 0; w < INFO_BOX_WIDTH; w++)
  {
    for (int h = 0; h < INFO_BOX_HEIGTH; h++)
    {
      tftRamInfoBox[w][h] = tft.readPixel(w + INFOBOX_OFFSET, h + INFOBOX_OFFSET);
    }
  }
}
void set_infobox_background()
{
  showBox = true;
  tft.fillRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_BLACK);
  tft.drawRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_WHITE);
  tft.setFont(Arial_10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET);
}

void set_infobox_next_line(byte _lineNumber)
{
  tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET + (20 * _lineNumber));
}
void reset_infobox_background(unsigned long int waitingTime)
{
  if (showBox && millis() - currentTime >= waitingTime)
  {
    showBox = false;
    for (int w = 0; w < INFO_BOX_WIDTH; w++)
    {
      for (int h = 0; h < INFO_BOX_HEIGTH; h++)
      {
        tft.drawPixel(w + INFOBOX_OFFSET, h + INFOBOX_OFFSET, tftRamInfoBox[w][h]);
      }
    }
  }
}
void detect_USB_device()
{
  static bool deviceConnected = false;
  // if connected
  if (usbMidi1.idProduct() != 0 && !deviceConnected)
  {
    get_infobox_background();
    set_infobox_background();

    deviceConnected = true;
    tft.print("USB Device connected: ");
    set_infobox_next_line(1);
    tft.printf("%s: %s", usbMidi1.manufacturer(), usbMidi1.product());
  }
  // if disconnected
  else if (usbMidi1.idProduct() == 0 && deviceConnected)
  {
    get_infobox_background();
    set_infobox_background();
    tft.println("USB Device disconnected");
    deviceConnected = false;

    // remove infobox
  }

  reset_infobox_background(2000);
}
void detect_mouse()
{
  static bool deviceConnected = false;
  if (mouse1.available() && !deviceConnected)
  {
    get_infobox_background();
    set_infobox_background();

    deviceConnected = true;
    tft.print("Mouse connected: ");
  }
  // if disconnected
  else if (!mouse1.available() && deviceConnected)
  {
    deviceConnected = false;
    get_infobox_background();
    set_infobox_background();
    tft.println("Mouse disconnected");
  }
  reset_infobox_background(2000);
}

// define a callback for key presses
TrellisCallback blink(keyEvent evt)
{

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    trellisPressed[evt.bit.NUM] = true;
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    trellisPressed[evt.bit.NUM] = false;
  }
  trellis_show();
  return 0;
}

// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

void trellis_set_control_buffer(int _x, int _y, int _color)
{
  trellisControllBuffer[_x][_y] = _color;
  trellis.setPixelColor(_x + 20, _y, trellisControllBuffer[_x][_y]);
  // trellis.show();
  // Serial.println("set control buffer");
}
int trellis_get_control_buffer(int _x, int _y)
{
  // Serial.println("get control buffer");

  return trellisControllBuffer[_x][_y];
}
void trellis_recall_control_buffer()
{
  if (trellisRecall)
  {
    for (int x = 0; x < TRELLIS_CONTROL_X_DIM; x++)
    {
      for (int y = 0; y < TRELLIS_CONTROL_Y_DIM; y++)
      {
        trellis.setPixelColor(x + 20, y, trellisControllBuffer[x][y]);
      }
    }
    // Serial.println("recall controll buffer");

    // trellis_show();
    trellisRecall = false;
  }
}

void trellis_set_panel_buffer(int _x, int _y, int _color)
{
  trellisPanelBuffer[_x][_y] = _color;
  int _nr = (_x + 16) + (_y * X_DIM);
  trellis.setPixelColor(_nr, trellisPanelBuffer[_x][_y]);
  // trellis_show();
  //  trellis.setPixelColor((_x*X_DIM)+16, _y, trellisPanelBuffer[_x][_y]);
  // Serial.println("set panel buffer");
}
int trellis_get_panel_buffer(int _x, int _y)
{
  // Serial.println("get panel buffer");

  return trellisPanelBuffer[_x][_y];
}
void trellis_recall_panel_buffer()
{
  if (trellisRecall)
  {
    for (int x = 0; x < TRELLIS_CONTROL_X_DIM; x++)
    {
      for (int y = 0; y < TRELLIS_PANEL_Y_DIM; y++)
      {
        trellis.setPixelColor(x + 16, y, trellisPanelBuffer[x][y]);
      }
    }
    // Serial.println("recall panel buffer");

    // trellis_show();
    trellisRecall = false;
  }
}

void trellis_set_main_buffer(int _page, int _x, int _y, int color)
{
  trellisMainGridBuffer[_page][_x][_y] = color;
  trellis.setPixelColor(_x, _y, trellisMainGridBuffer[_page][_x][_y]);
  // Serial.printf("set buffer page: %d, x: %d, y: %d, color: %d\n", _page, _x, _y, color);
  //   trellis.show();
}
void trellis_recall_main_buffer(int _page)
{
  if (trellisRecall)
  {
    for (int i = 0; i < TRELLIS_PADS_X_DIM; i++)
    {
      for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
      {
        trellis.setPixelColor(i, y, trellisMainGridBuffer[_page][i][y]);
      }
    }
    // Serial.println("load main buffer");
  }
  trellisRecall = false;
  // trellis_show();
}
int trellis_get_main_buffer(int _page, int _x, int _y)
{
  // Serial.println("return main buffer");
  return trellisMainGridBuffer[_page][_x][_y];
}

void trellis_setStepsequencer()
{
  if (trellisPressed[TRELLIS_BUTTON_SEQUENCER])
  {
    trellisRecall = true;
    trellisScreen = TRELLIS_SCREEN_SEQUENCER_CLIP_0;
    // trellisPressed[TRELLIS_BUTTON_SEQUENCER]=false;
  }
  if (trellisScreen < TRELLIS_SCREEN_SEQUENCER_CLIP_8)
  {
    for (int x = 0; x < NUM_STEPS; x++)
    {
      for (int y = 0; y < NUM_TRACKS; y++)
      {
        byte _nr = x + (y * X_DIM);
        if (trellisPressed[_nr])
        {
          byte trellisNote;
          byte track = y;
          byte step = x;
          int keyTick = (step * 6);
          // gridTouchY = 1;
          // trellisPressed[TRELLIS_BUTTON_ENTER] = true;
          if (gridTouchY > 0 && gridTouchY <= 12)
          {

            trellisNote = gridTouchY;
          }
          else
            trellisNote = 1;
          trellisPressed[_nr] = false;
          trellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
          allTracks[track]->set_note_on_tick(keyTick, trellisNote);
          Serial.printf("step: %d, tick: %d, track: %D \n", step, keyTick, track);
          /*
          for (int v = 0; v < MAX_VOICES; v++)
          {
            if (allTracks[_key / X_DIM]->get_active_note(allTracks[track]->parameter[SET_CLIP2_EDIT], keyTick, v) < NO_NOTE)
            {
              trellis_set_buffer(_key, trellisTrackColor[track]);
              break;
            }
            else if (allTracks[_key / X_DIM]->get_active_note(allTracks[_key / X_DIM]->parameter[SET_CLIP2_EDIT], keyTick, v) == NO_NOTE)
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

void trellis_show_arranger()
{
  if (trellisPressed[TRELLIS_BUTTON_ARRANGER])
  {
    trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 0, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 1, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 2, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 3, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 4, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 5, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 6, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 7, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 8, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 9, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 10, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 11, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 12, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 13, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 14, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 15, 0, TRELLIS_WHITE);
    trellisRecall = true;
    trellis_show();
    for (int i = 0; i < NUM_STEPS; i++)
    {
      if (trellisPressed[i])
      {
        trellisPressed[i] = false;
        trellisScreen = i + TRELLIS_SCREEN_ARRANGER_1;
        arrangerpage = i;
        trellisRecall = true;
        trellisPressed[TRELLIS_BUTTON_ARRANGER] = false;
        activeScreen = INPUT_FUNCTIONS_FOR_ARRANGER;

        // Serial.printf("songpage= %d\n", _key);
        gridSongMode(arrangerpage);
        break;
      }
    }
  }
}

void trellis_set_arranger(int _key)
{
  if (trellisScreen >= TRELLIS_SCREEN_ARRANGER_1 && !trellisPressed[TRELLIS_BUTTON_ARRANGER])
  {
    byte _clipPos = _key % X_DIM;
    byte _track = _key / X_DIM;
    byte _clipNr = gridTouchY;
    byte _clipTrack = _clipPos + (arrangerpage * 16);
    int _color;
    if (_clipPos < 16)
    {
      if (trellisPressed[_key])
      {
        trellisPressed[_key] = false;
        allTracks[_track]->clip_to_play[_clipTrack] = _clipNr;
        if (activeScreen >= INPUT_FUNCTIONS_FOR_ARRANGER)
        {
          allTracks[_track]->draw_clip_to_play(3, _clipTrack);
          allTracks[_track]->draw_arrangment_line(3, _clipTrack);
        }

        if (_clipNr < MAX_CLIPS)
          _color = trellisTrackColor[_track] + (allTracks[_track]->clip_to_play[_clipTrack] * 20);
        if (_clipNr == MAX_CLIPS - 1)
          _color = TRELLIS_BLACK;
        trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, _clipPos, _track, _color);
      }
    }
  }
}
void trellis_show_tft_sequencer()
{
  if (trellisPressed[TRELLIS_BUTTON_SEQUENCER])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (trellisPressed[19 + (i * X_DIM)])
      {
        trellisPressed[19 + (i * X_DIM)] = false;
        trellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
        active_track = i;
        show_active_track();
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
        change_plugin_row = true;
        allTracks[active_track]->drawStepSequencerStatic();
        allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
        allTracks[active_track]->draw_notes_in_grid();
        trellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}
void trellis_show_tft_plugin()
{
  if (trellisPressed[TRELLIS_BUTTON_PLUGIN])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (trellisPressed[19 + (i * X_DIM)])
      {
        trellisPressed[19 + (i * X_DIM)] = false;
        active_track = i;
        show_active_track();
        change_plugin_row = true;
        // allTracks[active_track]->draw_MIDI_CC_screen();
        activeScreen = INPUT_FUNCTIONS_FOR_PLUGIN;
        trellis_set_control_buffer(2, 3, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}
void trellis_show_tft_seqMode()
{
  if (trellisPressed[TRELLIS_BUTTON_SEQMODE])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (trellisPressed[19 + (i * X_DIM)])
      {
        trellisPressed[19 + (i * X_DIM)] = false;
        active_track = i;
        show_active_track();
        allTracks[active_track]->draw_sequencer_modes(allTracks[active_track]->parameter[SET_SEQ_MODE]);
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER_MODES;
        // Serial.println("SeqMode selected");
        trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, 3, 2, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}

void trellis_show_pads()
{
  trellis_recall_main_buffer(trellisScreen);
}
void trellis_setup()
{
  for (int y = 0; y < Y_DIM; y++)
  {
    for (int x = 0; x < X_DIM; x++)
    {
      // activate rising and falling edges on all keys
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
      trellis.registerCallback(x, y, blink);
    }
  }

  trellis_static();
}
void trellis_static()
{

  for (int i = 0; i < NUM_TRACKS; i++)
  {

    // trellis.setPixelColor(nr, TRELLIS_1); // off falling
    trellis_set_panel_buffer(0, i, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {

    // trellis.setPixelColor(nr, TRELLIS_1); // off falling
    trellis_set_panel_buffer(1, i, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {

    // trellis.setPixelColor(nr, TRELLIS_1); // off falling
    trellis_set_panel_buffer(2, i, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {

    // trellis.setPixelColor(nr, trellisTrackColor[i]); // off falling
    trellis_set_panel_buffer(3, i, trellisTrackColor[i]);
  }
  trellis_set_control_buffer(0, 0, TRELLIS_ORANGE);
  trellis_set_control_buffer(1, 0, TRELLIS_RED);
  trellis_set_control_buffer(2, 0, TRELLIS_GREEN);
  trellis_set_control_buffer(3, 0, TRELLIS_OLIVE);
  trellis_set_control_buffer(0, 3, TRELLIS_WHITE);
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 0, i, TRELLIS_TEAL);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 1, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 2, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 3, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 4, i, TRELLIS_ORANGE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 5, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 6, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 7, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 8, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 9, i, TRELLIS_ORANGE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 10, i, TRELLIS_ORANGE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 11, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 12, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 13, i, TRELLIS_BLUE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 14, i, TRELLIS_RED);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 15, i, TRELLIS_GREEN);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    for (int c = 0; c < 4; c++)
    {
      trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c, i, TRELLIS_PINK);
      trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 4, i, TRELLIS_OLIVE);
      trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 8, i, TRELLIS_AQUA);
      trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 12, i, TRELLIS_ORANGE);
    }
  }
  // trellis_recall_panel_buffer();
  // trellis_recall_control_buffer();
  Serial.println("trellis setup done");
  // trellisRecall=true;
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    for (int s = 0; s < NUM_STEPS; s++)
    {
      // byte nr = s + (i*NUM_STEPS);
      // if (trellisScreen == TRELLIS_SCREEN_SEQUENCER)
      trellis.setPixelColor(s, i, trellis_get_main_buffer(trellisScreen, s, i));
      // if (trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
      // trellis.setPixelColor(s, i, trellis_get_arranger_buffer(arrangerpage, s, i));
    }
  }
  trellis_show();
}

void trellis_show()
{
  trellis.show();
}
void trellis_start_clock()
{
  if (trellisPressed[TRELLIS_START_CLOCK] || enc_button[2])
  {
    myClock.set_start();
    // Serial.println("Play");
    trellisPressed[TRELLIS_START_CLOCK] = false;
    enc_button[2] = false;
  }
}
void trellis_stop_clock()
{
  if (trellisPressed[TRELLIS_STOP_CLOCK] || enc_button[3])
  {
    myClock.set_stop();
    // Serial.println("Stop");
    trellisPressed[TRELLIS_STOP_CLOCK] = false;
    enc_button[3] = false;
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      allTracks[i]->internal_clock = 0;
      allTracks[i]->internal_clock_bar = 0;
    }
  }
}
void trellis_set_potRow()
{
  if (trellisPressed[TRELLIS_POTROW])
  {
    trellisPressed[TRELLIS_POTROW] = false;
    change_plugin_row = true;
    lastPotRow++;
    if (lastPotRow >= 4)
    {
      lastPotRow = 0;
    }
    tft.fillRect(70, 0, 10, 16, ILI9341_DARKGREY);
    // trellisPressed[TRELLIS_POTROW] = false;
    // Serial.printf("potrwo=%d\n", lastPotRow);
  }
}
void trellis_SetCursor(byte maxY)
{

  if (trellisPressed[TRELLIS_BUTTON_LEFT]) //..move the cursor left
  {
    pixelTouchX = constrain(pixelTouchX - 1, 0, 320);
    trellisPressed[TRELLIS_BUTTON_LEFT] = false;
  }
  if (trellisPressed[TRELLIS_BUTTON_RIGHT]) //..move the cursor right
  {
    pixelTouchX = constrain(pixelTouchX + 1, 0, 320);
    trellisPressed[TRELLIS_BUTTON_RIGHT] = false;
  }
  if (trellisPressed[TRELLIS_BUTTON_UP]) //..move the cursor up
  {
    gridTouchY = constrain(gridTouchY - 1, 0, maxY);
    trellisPressed[TRELLIS_BUTTON_UP] = false;
  }

  if (trellisPressed[TRELLIS_BUTTON_DOWN]) //..move the cursor down
  {
    gridTouchY = constrain(gridTouchY + 1, 0, maxY);
    trellisPressed[TRELLIS_BUTTON_DOWN] = false;
  }
}

void trellis_set_track_record()
{
  if (trellisPressed[TRELLIS_BUTTON_RECORD])
  {
    if (!allTracks[active_track]->recordState)
    {
      allTracks[active_track]->recordState = true;
      trellis_set_panel_buffer(2, active_track, TRELLIS_RED);
      trellisRecall = true;
    }

    else if (allTracks[active_track]->recordState)
    {
      allTracks[active_track]->recordState = false;
      trellis_set_panel_buffer(2, active_track, TRELLIS_1);
      trellisRecall = true;
    }
    trellisPressed[TRELLIS_BUTTON_RECORD] = false;
  }
}
void trellis_show_clockbar(byte trackNr, byte step)
{
  if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8 || trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
  {
    trellisShowClockPixel[trackNr] = false;
    int color;

    if (allTracks[trackNr]->get_recordState())
      color = TRELLIS_RED;
    else
      color = TRELLIS_ORANGE;

    trellis.setPixelColor(step, trackNr, color);
    if (step > 0)
      trellis.setPixelColor(step - 1, trackNr, trellis_get_main_buffer(trellisScreen, step - 1, trackNr));
    if (step == 0)
    {
      byte oldNr = (allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1;
      trellis.setPixelColor(oldNr, trackNr, trellis_get_main_buffer(trellisScreen, oldNr, trackNr));
      // Serial.println(oldNr);
    }
    trellis_show();
  }
}

void trellis_show_piano()
{
  if (trellisPressed[TRELLIS_BUTTON_PIANO])
  {
    trellisScreen = TRELLIS_SCREEN_PIANO;
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (trellisPressed[19 + (i * X_DIM)])
      {
        trellisPressed[19 + (i * X_DIM)] = false;
        trellisPressed[TRELLIS_BUTTON_PIANO] = false;
        trellisPianoTrack = i;
        trellisRecall = true;
        int _color = TRELLIS_BLACK;
        for (int x = 0; x < NUM_STEPS; x++)
        {
          for (int y = 0; y < NUM_TRACKS; y++)
          {
            int _nr = x + (y * NUM_STEPS);
            if (_nr % 12 == 0)
            {
              _color = trellisTrackColor[trellisPianoTrack];
              if (allTracks[trellisPianoTrack]->get_recordState())
                _color = TRELLIS_RED;
            }
            else
              _color = TRELLIS_BLACK;
            trellis_set_main_buffer(TRELLIS_SCREEN_PIANO, x, y, _color);
          }
        }
        break;
      }
    }
  }
}
void trellis_play_piano()
{

  if (trellisScreen == TRELLIS_SCREEN_PIANO)
  {
    static bool _holdNote[NUM_STEPS * NUM_TRACKS];
    for (int x = 0; x < NUM_STEPS; x++)
    {
      for (int y = 0; y < NUM_TRACKS; y++)
      {
        byte _keyNr = x + (y * X_DIM);
        byte _keynote = x + (y * NUM_STEPS);
        if (trellisPressed[_keyNr] && !_holdNote[_keynote])
        {
          // Serial.printf("pianoPage _keyNr:%d, _keyNote: %d\n", _keyNr, _keynote);
          _holdNote[_keynote] = true;
          allTracks[trellisPianoTrack]->noteOn(_keynote, 99, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          if (allTracks[trellisPianoTrack]->get_recordState())
            allTracks[trellisPianoTrack]->record_noteOn(_keynote, 99, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          Serial.printf("trellisPiano NoteON key:%d, track:%d\n", _keynote, trellisPianoTrack);
          break;
        }

        else if (!trellisPressed[_keyNr] && _holdNote[_keynote])
        {
          _holdNote[_keynote] = false;
          allTracks[trellisPianoTrack]->noteOff(_keynote, 0, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          if (allTracks[trellisPianoTrack]->get_recordState())
            allTracks[trellisPianoTrack]->record_noteOff(_keynote, 0, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          Serial.printf("trellisPiano NoteOff key:%d, track:%d\n", _keynote, trellisPianoTrack);
          break;
        }
      }
    }
  }
}

void trellis_set_mute()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (trellisPressed[16 + (i * X_DIM)])
    {
      trellisPressed[16 + (i * X_DIM)] = false;
      if (!allTracks[i]->muted)
      {
        trellis_set_panel_buffer(0, i, TRELLIS_BLACK);
        allTracks[i]->muted = true;
        // Serial.println("set mute");
        break;
      }
      else if (allTracks[i]->muted)
      {
        trellis_set_panel_buffer(0, i, TRELLIS_1);
        allTracks[i]->muted = false;
        // Serial.println("set unmute");
        break;
      }
    }
  }
}
void trellis_set_fast_record()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (trellisPressed[18 + (i * X_DIM)])
    {
      trellisPressed[18 + (i * X_DIM)] = false;
      trellisRecall = true;
      if (!allTracks[i]->recordState)
      {
        allTracks[i]->recordState = true;
        trellis_set_panel_buffer(2, i, TRELLIS_RED);
        // trellis.setPixelColor(18, i, TRELLIS_RED);
        break;
      }

      else if (allTracks[i]->recordState)
      {
        allTracks[i]->recordState = false;
        trellis_set_panel_buffer(2, i, TRELLIS_1);
        // trellis.setPixelColor(18, i, TRELLIS_1);
        break;
      }

      // break;
    }
  }
}
void trellis_set_solo()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (trellisPressed[17 + (i * X_DIM)])
    {
      trellisPressed[17 + (i * X_DIM)] = false;
      if (!allTracks[i]->soloed)
      {
        trellis_set_panel_buffer(1, i, TRELLIS_ORANGE);
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
        trellis_set_panel_buffer(1, i, TRELLIS_1);
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

void trellis_show_tft_mixer()
{
  if (trellisPressed[TRELLIS_BUTTON_MIXER])
  {
    trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 0, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 1, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 2, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 3, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 4, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 5, 0, TRELLIS_WHITE);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 6, 0, TRELLIS_WHITE);

    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 7, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 8, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 9, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 10, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 11, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 12, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 13, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 14, 0, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, 15, 0, TRELLIS_BLACK);
    trellisRecall = true;
    trellis_show();
    if (trellisPressed[0])
    {
      trellisPressed[0] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      // trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER1;
      clearWorkSpace();
      draw_mixer();
    }
    if (trellisPressed[1])
    {
      trellisPressed[1] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      // trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER2;
      clearWorkSpace();
      draw_mixer_FX_page1();
    }
    if (trellisPressed[2])
    {
      trellisPressed[2] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      // trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER3;
      clearWorkSpace();
      draw_mixer_FX_page2();
    }
    if (trellisPressed[3])
    {
      trellisPressed[3] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      // trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      clearWorkSpace();
      fx_1.draw_plugin();
    }
    if (trellisPressed[4])
    {
      trellisPressed[4] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      // trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX2;
      clearWorkSpace();
      fx_2.draw_plugin();
    }
    if (trellisPressed[5])
    {
      trellisPressed[5] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      // trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX3;
      clearWorkSpace();
      fx_3.draw_plugin();
    }
    if (trellisPressed[6])
    {
      trellisPressed[6] = false;
      trellisScreen = TRELLIS_SCREEN_PERFORM;
      trellisRecall = true;
      trellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      // activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      // clearWorkSpace();
      // fx_2.draw_plugin();
    }
  }
}
void trellis_play_mixer()
{

  if (trellisScreen == TRELLIS_SCREEN_MIXER)
  {
    float _gain[4] = {0, 0.30, 0.60, 1};

    for (int t = 0; t < NUM_TRACKS; t++)
    {
      if (allTracks[t]->parameter[SET_MIDICH_OUT] >= (NUM_MIDI_OUTPUTS + 1))
      {
        for (int s = 0; s < NUM_STEPS; s++)
        {
          byte _nr = s + (t * X_DIM);
          if (trellisPressed[_nr])
          {
            trellisPressed[_nr] = false;
            for (int c = 0; c < 4; c++)
            {
              if (_nr % X_DIM == c)
              {
                Serial.printf("dry channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                MasterOut.fx_section.dry[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                break;
              }
              if (_nr % X_DIM == c + 4)
              {
                Serial.printf("fx1 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_1.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                break;
              }
              if (_nr % X_DIM == c + 8)
              {
                Serial.printf("fx2 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_2.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                break;
              }

              if (_nr % X_DIM == c + 12)
              {
                Serial.printf("fx3 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_3.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
              }
            }
          }
        }
      }
    }
  }
}

void trellis_perform()
{
  if (trellisScreen == TRELLIS_SCREEN_PERFORM)
  {
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      if (trellisPressed[19 + (t * X_DIM)])
      {

        if (!allTracks[t]->performIsActive)
        {
          trellis_set_panel_buffer(3, t, TRELLIS_WHITE);
          trellisRecall = true;
          allTracks[t]->performIsActive = true;
          trellisPressed[19 + (t * X_DIM)] = false;
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
          trellis_set_panel_buffer(3, t, trellisTrackColor[t]);
          trellisRecall = true;
          allTracks[t]->performIsActive = false;
          trellisPressed[19 + (t * X_DIM)] = false;
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

    for (int t = 0; t < NUM_TRACKS; t++)
    {
      for (int i = 0; i < NUM_STEPS; i++)
      {
        int _nr = i + (t * X_DIM);
        if (trellisPressed[_nr])
        {
          // change_plugin_row = true;
          trellisPressed[_nr] = false;
          if (_nr % X_DIM == 0)
          {

            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->mixGainPot = 127 - (t * 16);
              }
            }
          }
          if (_nr % X_DIM == 1)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
              {
                Serial.printf("dry channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixDryPot = 127 - (t * 16);
                MasterOut.fx_section.dry[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);

                // break;
              }
            }
          }
          if (_nr % X_DIM == 2)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
              {
                Serial.printf("fx1 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX1Pot = 127 - (t * 16);
                fx_1.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);

                // break;
              }
            }
          }
          if (_nr % X_DIM == 3)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
              {
                Serial.printf("fx2 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX2Pot = 127 - (t * 16);
                fx_2.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);

                // break;
              }
            }
          }
          if (_nr % X_DIM == 4)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
              {
                Serial.printf("fx3 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX3Pot = 127 - (t * 16);
                fx_3.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);

                // break;
              }
            }
          }

          if (_nr % X_DIM == 5)
          {
            fx_1.potentiometer[fx_1.presetNr][0] = 127 - (t * 16);
            fx_1.freeverb.roomsize((float)map(t, 0, 8, 0, 1.00));
          }
          if (_nr % X_DIM == 6)
          {

            fx_1.potentiometer[fx_1.presetNr][1] = 127 - (t * 16);
            fx_1.freeverb.damping((float)map(t, 0, 8, 0, 1.00));
          }
          if (_nr % X_DIM == 7)
          {
            fx_2.potentiometer[fx_2.presetNr][0] = 127 - (t * 16);

            fx_2.bitcrusher.bits(map(t, 0, 8, 16, 0));
          }
          if (_nr % X_DIM == 8)
          {
            int _rate[Y_DIM] = {44100, 22050, 11025, 5512, 2756, 1378, 1022, 689};
            fx_2.potentiometer[fx_2.presetNr][1] = 127 - (t * 16);
            fx_2.bitcrusher.sampleRate(_rate[t]); // does not work
            // fx_2.bitcrusher.sampleRate(5512 * t); // cool but wrong
            //  fx_2.bitcrusher.sampleRate(44100 - (5512 * t)); //
          }
          if (_nr % X_DIM == 9)
          {
            fx_3.potentiometer[fx_3.presetNr][0] = 127 - (t * 16);
            fx_3.delay.delay(0, 500 / (t + 1));
          }
          if (_nr % X_DIM == 10)
          {
            fx_3.potentiometer[fx_3.presetNr][1] = 127 - (t * 16);
            fx_3.delayMixer.gain(1, t / 8.00);
          }
          if (_nr % X_DIM == 11)
          {
            int frequency = note_frequency[t * 16] * tuning;
            MasterOut.finalFilter.frequency(frequency);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                sendControlChange(74, t * 16, allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
            }
          }
          if (_nr % X_DIM == 12)
          {
            MasterOut.finalFilter.resonance(map(t, 0, 8, 0, 5.00));
            /*
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                sendControlChange(71, t * 16, allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
            }*/
          }
          if (_nr % X_DIM == 13)
          {
            byte _clipLength[Y_DIM]{96, 72, 60, 48, 36, 24, 12, 6};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->parameter[SET_SEQUENCE_LENGTH] = _clipLength[t];
              }
            }
          }
          if (_nr % X_DIM == 14)
          {
            byte _stepDivision[Y_DIM]{1, 2, 3, 4, 6, 8, 12, 16};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->parameter[SET_STEP_DIVIVISION] = _stepDivision[t];
              }
            }
          }
          if (_nr % X_DIM == 15)
          {
            byte _stepLength[Y_DIM]{1, 2, 3, 4, 6, 8, 12, 16};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->parameter[SET_STEP_LENGTH] = _stepLength[t];
              }
            }
          }
        }
      }
    }
  }
}
