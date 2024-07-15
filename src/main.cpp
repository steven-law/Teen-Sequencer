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
#include "hw_stuff.h"
#include "project_variables.h"
#include "cursor.h"
#include "Track.h"
#include "clock.h"
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice usbMidi1(myusb);
USBHIDParser hid1(myusb);
// USBHIDParser hid2(myusb);
MouseController mouse1(myusb);
// Clock Masterclock(&tft);

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

void setup()
{
  // while (!Serial){}
  //   put your setup code here, to run once:
  tft_setup(100);
  button_setup(100);
  encoder_setup(100);
  delay(1500);
  midi_setup(100);
  MIDI1.begin();
  myusb.begin();

  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  usbMidi1.setHandleNoteOn(myNoteOn);
  usbMidi1.setHandleNoteOff(myNoteOff);

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
  myusb.Task();
  usbMidi1.read();
  detect_USB_device();
  //detect_mouse();
  if (millis() % 50 == 0)
  {
    // Serial.println(usbMidi1.idProduct());

    if (encoder_function == INPUT_FUNCTIONS_FOR_ARRANGER)
    {
      mouse(2, 14);
      cursor.update(pixelTouchX, gridTouchY, 1, TRACK_FRAME_H);
    }
    else
    {
      mouse(2, 14);
      cursor.update(pixelTouchX, gridTouchY, 1, STEP_FRAME_H);
    }

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
  if (Channel > 0 && Channel <= 16)
    MIDI1.sendNoteOn(Note, Velo, Channel);
  if (Channel > 16 && Channel <= 32)
    usbMIDI.sendNoteOn(Note, Velo, Channel - 16);
  if (Channel > 32 && Channel <= 48)
    usbMidi1.sendNoteOn(Note, Velo, Channel - 32);
}
void sendNoteOff(byte Note, byte Velo, byte Channel)
{

  if (Channel > 0 && Channel <= 16)
    MIDI1.sendNoteOff(Note, Velo, Channel);
  if (Channel > 16 && Channel <= 32)
    usbMIDI.sendNoteOff(Note, Velo, Channel - 16);
  if (Channel > 32 && Channel <= 48)
  {
    usbMidi1.sendNoteOff(Note, Velo, Channel - 32);
    // Serial.println(usbMidi1.idVendor());
  }
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
    if (encoder_function == INPUT_FUNCTIONS_FOR_SEQUENCER)
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
    if (encoder_function == INPUT_FUNCTIONS_FOR_ARRANGER)
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