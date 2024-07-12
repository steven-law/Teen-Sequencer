/*ideas:
add volume per bar in songmode, thickness of line represents volume
done: add 2nd parameter to edit per tick
done: choose midi output
done: show active songpage
done: look what takes so long to load the song scren
*/
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
void sendControlChange(byte control, byte value, byte Channel);
void sendClock();
void setup()
{
  // while (!Serial){}
  //   put your setup code here, to run once:
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
    // allTracks[i]->save_track();
    // allTracks[i]->load_track();
  }

  Masterclock.process_MIDItick();
  clock_to_notes();

  if (millis() % 50 == 0)
    tft.updateScreenAsync();
}

// put function definitions here:
void sendNoteOn(byte Note, byte Velo, byte Channel)
{

  if (Channel <= 16 && Channel > 0)
    usbMIDI.sendNoteOn(Note, Velo, Channel);
  if (Channel > 16)
    MIDI1.sendNoteOn(Note, Velo, Channel - 16);
}
void sendNoteOff(byte Note, byte Velo, byte Channel)
{

  if (Channel <= 16 && Channel > 0)
    usbMIDI.sendNoteOff(Note, Velo, Channel);
  if (Channel > 16)
    MIDI1.sendNoteOff(Note, Velo, Channel - 16);
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
}

void myNoteOn(byte channel, byte note, byte velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
}
void myNoteOff(byte channel, byte note, byte velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
}