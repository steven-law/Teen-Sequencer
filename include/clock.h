#ifndef CLOCK_H
#define CLOCK_H
#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "hw_stuff.h"
#include "project_variables.h"

#define SONG_POSITION_POINTER_Y 228
#define BAR_POSITION_POINTER_Y 232
#define STEP_POSITION_POINTER_Y 236
void sendClock();
//clock

class Clock
{
public:
  ILI9341_t3n *tft; // Pointer to the display object
  uint32_t _next_clock = 0;
  uint32_t _clock = 160;
  uint32_t MIDItick = -1;
  uint32_t step_tick = -1;
  uint32_t bar_tick = -1;
  byte bar_tick_display = -1;
  byte start_of_loop = 0;
  byte end_of_loop = 255;
  bool clock_is_on_tick = false;
  bool seq_run = false;
  bool seq_rec = false;
  bool playing = false;
  bool stop_once = false;
  uint32_t master_tempo = 120;
  int syncPin = -1;
  uint32_t previousMillis_clock = 0;

  Clock(ILI9341_t3n *display)
  {
    tft = display;
  }

  // setup function, so we can put in an analog sync Output (Optional)
  void setAnalogSyncPin(int new_syncPin);

  bool is_playing();
  void setPlayStatus(bool p = false);

  void set_start_of_loop(byte n);
  void draw_clock_option(byte x, byte v);
  void set_end_of_loop(byte n);

  void set_tempo(byte n);

  int get_tempo();

  void send_MIDIclock();
  void send_sync_clock();

  bool process_MIDItick();
  uint32_t get_MIDItick();
  // step counter stuff
  uint32_t get_step_tick();
  bool Clock_is_on_tick();
  bool clock_is_on_step();
  void update_step_tick();
  void drawstepPosition(int current);
  // bar counter stuff
  uint32_t get_bar_tick();
  bool tick_is_on_bar();
  void update_bar_tick();
  void drawbarPosition();

  // display stuff
};
extern Clock Masterclock;
#endif // HW_STUFF_H
