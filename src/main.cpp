/*ideas:
move variables into flashmem
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
#include "Adafruit_NeoTrellis.h"
// #include <Adafruit_NeoTrellis.h>
//  #include "neoTrellis.h"
#include "hw_stuff.h"
#include "project_variables.h"
#include "cursor.h"
#include "Track.h"
#include <plugin_List.h>
#include <fx_List.h>
#include "clock.h"
#include "Output.h"
#include "myClock.h"
#define INT_PIN 33
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP 0xFC
// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x3E, &Wire1), Adafruit_NeoTrellis(0x36, &Wire1), Adafruit_NeoTrellis(0x32, &Wire1), Adafruit_NeoTrellis(0x30, &Wire1), Adafruit_NeoTrellis(0x2F, &Wire1), Adafruit_NeoTrellis(0x46, &Wire1)},

    {Adafruit_NeoTrellis(0x3F, &Wire1), Adafruit_NeoTrellis(0x37, &Wire1), Adafruit_NeoTrellis(0x33, &Wire1), Adafruit_NeoTrellis(0x31, &Wire1), Adafruit_NeoTrellis(0x35, &Wire1), Adafruit_NeoTrellis(255, &Wire1)}

};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM / 4, X_DIM / 4);
Adafruit_NeoTrellis control(0x46, &Wire1);
// Adafruit_MultiTrellis control((Adafruit_NeoTrellis *)_control, 1, 1);
Plugin_2 plugin_2("1OSC", 18);
Plugin_3 plugin_3("2FM", 19);
PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_2, &plugin_3};
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
// Clock Masterclock(&tft);
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long trellisPreviousMillis = 0; // will store last time LED was updated

// constants won't change:
const long trellisInterval = 10000; // interval at which to blink (milliseconds)
//  put function declarations here:
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
void trellis_set_buffer(int _nr, int color);
int trellis_get_buffer(int _nr);
void trellis_setStepsequencer(int _key);
void trellis_set_mute(int _key);
void trellis_set_solo(int _key);
void draw_clock_option(byte x, byte v);
void drawstepPosition(int current);
void drawbarPosition(byte _bar);
void trellis_start_clock();
void trellis_stop_clock();
void trellis_show_clockbar(byte trackNr, byte step);
void trellis_show();
void trellis_show_sequencer(int _key);
void trellis_show_plugin(int _key);
void trellis_show_seqMode(int _key);
void trellis_set_potRow();
void trellis_SetCursor(byte maxY);
void trellis_show_arranger(int _key);
void setup()
{
  while (!Serial)
  {
  }
  //   put your setup code here, to run once:
  tft_setup(100);
  button_setup(100);
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

  if (!control.begin())
  {
    Serial.println("DAWfailed to begin controlPanel");
    while (1)
      delay(1);
  }

  Serial.println("DAWtrellis started");

  myClock.setup();
  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  usbMidi1.setHandleNoteOn(myNoteOn);
  usbMidi1.setHandleNoteOff(myNoteOff);
  AudioMemory(200);
  MasterOut.setup();
  note_frequency = new float[128];
  for (int r = 0; r < 128; r++)
  {
    note_frequency[r] = pow(2.0, ((double)(r - SAMPLE_ROOT) / 12.0));
  }
  // MIDI1.setHandleControlChange(myControlChange);
  startUpScreen();
  tft.updateScreenAsync();
  trellis_setup();
}

void loop()
{
  // Serial.println("hello");
  //  put your main code here, to run repeatedly:
  readEncoders();
  readMainButtons();
  readMIDI();

  MIDI1.read();
  myusb.Task();
  usbMidi1.read();
  detect_USB_device();
  if (!digitalRead(INT_PIN))
  {
    Serial.println("reading trellis");
    trellis.read(false);
    // delay(1);
  }

  for (int i = 0; i < X_DIM * Y_DIM; i++)
  {
    trellis_setStepsequencer(i);
    trellis_set_mute(i);
    trellis_set_solo(i);
    trellis_show_sequencer(i);
    trellis_show_plugin(i);
    trellis_show_seqMode(i);
    trellis_show_arranger(i);
  }
  // trellis.read();
  //  control.read();
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
  /*unsigned long trellisCurrentMillis = millis();
  if (trellisCurrentMillis - trellisPreviousMillis >= trellisInterval)
  {
    trellisPreviousMillis = trellisCurrentMillis;
    //trellis.trellisReset();
    Serial.println("Trellis resetted really");
    for (int i = 0; i < X_DIM * Y_DIM; i++)
      trellis.setPixelColor(i, trellis_get_buffer(i));
    Serial.println("Trellis resetted");
  }
  */
  // Masterclock.process_MIDItick();
  // clock_to_notes();

  if (millis() % 50 == 0)
  {
    tft.updateScreenAsync();
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
    MasterOut.noteOn(Note, Velo, Channel - (48 + 1), 0);
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
    if (Channel <= 16 && Channel > 0)
      usbMIDI.sendControlChange(control, value, Channel);
    if (Channel > 16)
      MIDI1.sendControlChange(control, value, Channel - 16);
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
      buttonPressed[BUTTON_ROW] = true;

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
        buttonPressed[BUTTON_ENTER] = true;
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
TrellisCallback blink2(keyEvent evt)
{

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    control.pixels.setPixelColor(evt.bit.NUM, TRELLIS_WHITE); // on rising
    Serial.println(evt.bit.NUM);
    buttonPressed[evt.bit.NUM] = true;
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    control.pixels.setPixelColor(evt.bit.NUM, controlColors[evt.bit.NUM]); // off falling
    buttonPressed[evt.bit.NUM] = false;
  }
  control.pixels.show();
  return 0;
}
// define a callback for key presses
TrellisCallback blink(keyEvent evt)
{

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    trellis.setPixelColor(evt.bit.NUM, TRELLIS_WHITE); // on rising
    Serial.printf("keypressd :%d\n", evt.bit.NUM);
    trellisPressed[evt.bit.NUM] = true;
    trellisReleased[evt.bit.NUM] = false;
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    trellis.setPixelColor(evt.bit.NUM, trellis_get_buffer(evt.bit.NUM)); // off falling
    trellisPressed[evt.bit.NUM] = false;
    trellisReleased[evt.bit.NUM] = true;
    Serial.printf("keyreleased :%d\n", evt.bit.NUM);
  }
  trellis.show();
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
void trellis_set_buffer(int _nr, int color)
{
  trellisLedBuffer[_nr] = color;
  trellis.setPixelColor(_nr, color);
  Serial.printf("set buffer key: %d, color: %d\n", _nr, color);
  // trellis.show();
}
int trellis_get_buffer(int _nr)
{
  return trellisLedBuffer[_nr];
}
void trellis_static()
{

  for (int i = 0; i < NUM_TRACKS; i++)
  {
    int nr = (16 + (i * X_DIM));
    trellis.setPixelColor(nr, TRELLIS_1); // off falling
    trellis_set_buffer(nr, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    int nr = (17 + (i * X_DIM));
    trellis.setPixelColor(nr, TRELLIS_1); // off falling
    trellis_set_buffer(nr, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    int nr = (18 + (i * X_DIM));
    trellis.setPixelColor(nr, TRELLIS_1); // off falling
    trellis_set_buffer(nr, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    int nr = (19 + (i * X_DIM));
    trellis.setPixelColor(nr, trellisTrackColor[i]); // off falling
    trellis_set_buffer(nr, trellisTrackColor[i]);
  }
  trellis_set_buffer(TRELLIS_POTROW, TRELLIS_ORANGE);
  trellis_set_buffer(TRELLIS_RECORD, TRELLIS_RED);
  trellis_set_buffer(TRELLIS_START_CLOCK, TRELLIS_GREEN);
  trellis_set_buffer(TRELLIS_STOP_CLOCK, TRELLIS_OLIVE);
  trellis_set_buffer(TRELLIS_SHOW_SONGMODE, TRELLIS_WHITE);

  trellis.show();
  Serial.println("trellis setup done");

}
void trellis_show_sequencer(int _key)
{
  if (trellisPressed[TRELLIS_SHOW_SEQUENCER])
  {
    if (_key % X_DIM == 19)
    {
      if (trellisPressed[_key])
      {
        trellisPressed[_key] = false;
        active_track = _key / X_DIM;
        show_active_track();
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
        change_plugin_row = true;
        allTracks[active_track]->drawStepSequencerStatic();
        allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
        allTracks[active_track]->draw_notes_in_grid();
        trellis_set_buffer(TRELLIS_SHOW_SEQUENCER, trellisTrackColor[active_track]);
      }
    }
  }
}
void trellis_show_plugin(int _key)
{
  if (trellisPressed[TRELLIS_SHOW_PLUGIN])
  {
    if (_key % X_DIM == 19)
    {
      if (trellisPressed[_key])
      {
        trellisPressed[_key] = false;
        active_track = _key / X_DIM;

        show_active_track();
        change_plugin_row = true;
        // allTracks[active_track]->draw_MIDI_CC_screen();
        activeScreen = INPUT_FUNCTIONS_FOR_PLUGIN;
        trellis_set_buffer(TRELLIS_SHOW_PLUGIN, trellisTrackColor[active_track]);
      }
    }
  }
}
void trellis_show_seqMode(int _key)
{
  if (trellisPressed[TRELLIS_SHOW_SEQMODE])
  {
    if (_key % X_DIM == 19)
    {
      if (trellisPressed[_key])
      {
        trellisPressed[_key] = false;
        active_track = _key / X_DIM;
        show_active_track();
        allTracks[active_track]->draw_sequencer_modes(allTracks[active_track]->parameter[SET_SEQ_MODE]);
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER_MODES;
        Serial.println("SeqMode selected");
        trellis_set_buffer(TRELLIS_SHOW_SEQMODE, trellisTrackColor[active_track]);
      }
    }
  }
}
void trellis_set_mute(int _key)
{
  byte track = _key / X_DIM;

  if (_key % X_DIM == 16)
  {
    if (trellisPressed[_key])
    {
      trellisPressed[_key] = false;
      if (!allTracks[track]->muted)
      {
        trellis_set_buffer(_key, TRELLIS_BLACK);
        allTracks[track]->muted = true;
        Serial.println("set mute");
      }
      else if (allTracks[track]->muted)
      {
        trellis_set_buffer(_key, TRELLIS_1);
        allTracks[track]->muted = false;
        Serial.println("set unmute");
      }
    }
  }
}

void trellis_set_solo(int _key)
{
  byte track = _key / X_DIM;
  if (_key % X_DIM == 17)
  {
    if (trellisPressed[_key])
    {
      trellisPressed[_key] = false;
      if (!allTracks[track]->soloed)
      {
        trellis_set_buffer(_key, TRELLIS_RED);
        allTracks[track]->soloed = true;
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (!allTracks[o]->soloed)
          {
            allTracks[o]->muted = true;
          }
        }
        Serial.println("set solo");
      }
      else if (allTracks[track]->soloed)
      {
        trellis_set_buffer(_key, TRELLIS_1);
        allTracks[track]->soloed = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (!allTracks[o]->soloed)
          {
            allTracks[o]->muted = false;
          }
        }
        Serial.println("set unsolo");
      }
    }
  }
}
void trellis_setStepsequencer(int _key)
{
  if (trellisPressed[TRELLIS_SHOW_SEQUENCER])
    trellisScreen = TRELLIS_SCREEN_SEQUENCER;
  if (trellisScreen == TRELLIS_SCREEN_SEQUENCER)
  {
    if (_key % X_DIM < 16)
    {
      byte trellisNote;
      byte track = _key / X_DIM;
      byte step = _key % X_DIM;
      int keyTick = (step * 6);
      while (trellisPressed[_key])
      {

        // gridTouchY = 1;
        // buttonPressed[BUTTON_ENTER] = true;
        if (gridTouchY > 0 && gridTouchY <= 12)
        {

          trellisNote = gridTouchY;
        }
        else
          trellisNote = 1;

        // if (!trellisPressed[_key])
        {
          trellisPressed[_key] = false;
          // trellisReleased[_key] = false;
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

void trellis_show_clockbar(byte trackNr, byte step)
{
  trellisShowClockPixel[trackNr] = false;
  int color;
  byte _nr = ((trackNr * X_DIM) + step);
  if (allTracks[trackNr]->get_recordState())
    color = TRELLIS_RED;
  else
    color = TRELLIS_ORANGE;

  trellis.setPixelColor(_nr, color);
  if (step > 0)
    trellis.setPixelColor(_nr - 1, trellis_get_buffer(_nr - 1));
  if (step == 0)
  {
    byte oldNr = (_nr + (allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6)) - 1;
    trellis.setPixelColor(oldNr, trellis_get_buffer(oldNr));
    // Serial.println(oldNr);
  }
  trellis.show();
}

void trellis_show()
{
  trellis.show();
}

void trellis_start_clock()
{
  if (trellisPressed[TRELLIS_START_CLOCK])
  {
    myClock.set_start();
    Serial.println("Play");
    trellisPressed[TRELLIS_START_CLOCK] = false;
  }
}
void trellis_stop_clock()
{
  if (trellisPressed[TRELLIS_STOP_CLOCK])
  {
    myClock.set_stop();
    Serial.println("Stop");
    trellisPressed[TRELLIS_STOP_CLOCK] = false;
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
    // buttonPressed[BUTTON_ROW] = false;
    Serial.printf("potrwo=%d\n", lastPotRow);
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
void trellis_show_arranger(int _key)
{
  if (trellisPressed[TRELLIS_SHOW_SONGMODE])
  {
    trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
    if (_key <= SONGMODE_PAGE_16)
    {
      if (trellisPressed[_key])
      {
        trellisPressed[_key] = false;
        trellisPressed[TRELLIS_SHOW_SONGMODE] = false;
        activeScreen = INPUT_FUNCTIONS_FOR_ARRANGER;
        trellisScreen = _key + 10;
        Serial.printf("songpage= %d\n", _key);
        gridSongMode(_key);
        show_active_songpage();
      }
    }
  }
}