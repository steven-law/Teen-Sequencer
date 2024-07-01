// library includes
#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <SPI.h>
#include <Wire.h>
#include <MIDI.h>
#include "hw_stuff.h"
#include "project_variables.h"
#include "cursor.h"
#include "Track.h"
#include "clock.h"
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
// Clock Masterclock(&tft);
//  put function declarations here:
void sendNoteOn(byte Note, byte Velo, byte Channel);
void sendNoteOff(byte Note, byte Velo, byte Channel);
void sendControlChange(byte control, byte value, byte channel);
void setup()
{
  // put your setup code here, to run once:
  tft_setup(100);
  button_setup(100);
  encoder_setup(100);
  midi_setup(100);
  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  MIDI1.begin();
  // MIDI1.setHandleControlChange(myControlChange);
  startUpScreen();
  tft.updateScreenAsync();
}

void loop()
{
  // put your main code here, to run repeatedly:
  readEncoders();
  readMainButtons();
  readMIDI();

  MIDI1.read();
  if (millis() % 50 == 0)
  {

    if (encoder_function == INPUT_FUNCTIONS_FOR_ARRANGER)
      cursor.update(pixelTouchX, gridTouchY, 1, TRACK_FRAME_H);
    else
      cursor.update(pixelTouchX, gridTouchY, 1, STEP_FRAME_H);
    tft.fillRect(70, lastPotRow * 4, 10, 3, ILI9341_RED);
  }
  input_behaviour();
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->update(pixelTouchX, gridTouchY);
    // allTracks[i]->saveTrack();
    // allTracks[i]->loadTrack();
  }

  Masterclock.process_MIDItick();
  clock_to_notes();

  if (millis() % 50 == 0)
    tft.updateScreenAsync();
}

// put function definitions here:
void sendNoteOn(byte Note, byte Velo, byte Channel)
{
  MIDI1.sendNoteOn(Note, Velo, Channel);
  usbMIDI.sendNoteOn(Note, Velo, Channel);
}
void sendNoteOff(byte Note, byte Velo, byte Channel)
{
  MIDI1.sendNoteOff(Note, Velo, Channel);
  usbMIDI.sendNoteOff(Note, Velo, Channel);
}
void sendControlChange(byte control, byte value, byte channel)
{
  usbMIDI.sendControlChange(control, value, channel);
  MIDI1.sendControlChange(control, value, channel);
}