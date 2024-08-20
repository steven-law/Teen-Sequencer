#include "ownLibs/clock.h"

// setup function, so we can put in an analog sync Output (Optional)
void Clock::setAnalogSyncPin(int new_syncPin)
{
    syncPin = new_syncPin;
    if (syncPin >= 0)
    {
        digitalWrite(syncPin, LOW); //
        pinMode(syncPin, OUTPUT);
    }
}

bool Clock::is_playing()
{
    return playing;
}
void Clock::setPlayStatus(bool p )
{ // if we want to start the clock use, "clock.is_playing(true);"
    playing = p;
}

void Clock::set_start_of_loop(byte n)
{
    if (enc_moved[n])
    {
        start_of_loop = constrain(start_of_loop + encoded[n], 0, 254);

        draw_clock_option(POSITION_START_LOOP_BUTTON, start_of_loop);

        enc_moved[n] = false;
    }
}
void Clock::draw_clock_option(byte x, byte v)
{
    tft->setCursor(STEP_FRAME_W * x + 2, 3);
    tft->setFont(Arial_10);
    tft->setTextColor(ILI9341_WHITE);
    tft->setTextSize(1);
    tft->fillRect(STEP_FRAME_W * x + 2, 1, STEP_FRAME_W * 2 - 4, STEP_FRAME_H - 2, ILI9341_DARKGREY);
    tft->print(v);
    tft->updateScreenAsync();
}
void Clock::set_end_of_loop(byte n)
{
    if (enc_moved[n])
    {
        end_of_loop = constrain(end_of_loop + encoded[n], 2, 255);

        draw_clock_option(POSITION_END_LOOP_BUTTON, end_of_loop);

        enc_moved[n] = false;
    }
}

void Clock::set_tempo(byte n)
{
    if (enc_moved[n])
    {
        master_tempo = constrain(master_tempo + encoded[n], 1, 250);
        // midi clock messages should be sent 24 times
        // for every quarter note
        _clock = 60000000L / master_tempo / 24;
        draw_clock_option(POSITION_BPM_BUTTON, master_tempo);
        enc_moved[n] = false;
    }
}

int Clock::get_tempo()
{
    return master_tempo;
}

void Clock::send_MIDIclock()
{
    // spit out a MIDItick
    sendClock();
    //usbMIDI.sendRealTime(usbMIDI.Clock); // send a midiclock to usb host
                                         // MIDI.sendRealTime(0xF8);              //send a midiclock to serial midi
}
void Clock::send_sync_clock()
{

    // spit out a MIDItick
    if (syncPin >= 0)
    {
        digitalWrite(syncPin, HIGH);
        uint32_t currentMillis = millis();
        if (currentMillis - previousMillis_clock >= 10)
        {
            previousMillis_clock = currentMillis;
            digitalWrite(syncPin, LOW);
        }
    }
}

bool Clock::process_MIDItick()
{

    if (playing)
    {
        // if the clock is running
        if (msecsclock >= _clock)
        {               // compare clockvalues
            MIDItick++; // spit out a MIDItick
            send_MIDIclock();
            msecsclock = 0;
            update_step_tick();
            update_bar_tick();
            clock_is_on_tick = true;
            stop_once = true;
            return true;
        }
        else
            clock_is_on_tick = false;
        if (MIDItick == 96)
        {
            MIDItick = 0;
        }
    }

    if (!playing)
    {
        if (stop_once)
        {
            stop_once = false;
            MIDItick = -1;
            step_tick = -1;
            bar_tick = start_of_loop;
            bar_tick_display = -1;
            tft->fillRect(STEP_FRAME_W * 2, SONG_POSITION_POINTER_Y, STEP_FRAME_W * 16, 4, ILI9341_DARKGREY);
            tft->fillRect(STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y, STEP_FRAME_W * 16, 4, ILI9341_DARKGREY);
            tft->fillRect(STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y, STEP_FRAME_W * 16, 4, ILI9341_DARKGREY);
            tft->asyncUpdateActive();
            clock_is_on_tick = false;
        }
    }
    return false;
}
uint32_t Clock::get_MIDItick()
{ // returns the actual MIDItick count
    return MIDItick;
}
// step counter stuff
uint32_t Clock::get_step_tick()
{ // returns the actual STEPtick count
    // int miditick;
    return MIDItick % 6;
}
bool Clock::Clock_is_on_tick()
{
    return MIDItick % 1 == 0;
}
bool Clock::clock_is_on_step()
{
    return MIDItick % 6 == 0;
}
void Clock::update_step_tick()
{
    if (MIDItick % 6 == 0)
    {
        step_tick++;
        if (step_tick == 16)
        {
            step_tick = 0;
        }
        drawstepPosition(step_tick);
    }
}
void Clock::drawstepPosition(int current)
{
    if (clock_is_on_step())
    {
        // draw the songpointer positions
        for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
        {
            for (int stepwidth = 1; stepwidth <= 16; stepwidth++)
            {
                tft->drawFastHLine(current * stepwidth + STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_GREEN);
                tft->drawFastHLine((current - 1) * stepwidth + STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
            }
            if (current == 0)
            {
                tft->drawFastHLine(STEP_FRAME_W * 17, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
            }
        }
        // Serial.print(current);
        tft->updateScreenAsync();
    }
}
// bar counter stuff
uint32_t Clock::get_bar_tick()
{ // returns the actual BARtick count
    return MIDItick % 96;
}
bool Clock::tick_is_on_bar()
{
    return MIDItick % 96 == 0;
}
void Clock::update_bar_tick()
{
    if (MIDItick % 96 == 0)
    {

        bar_tick++;
        bar_tick_display++;
        if (bar_tick == end_of_loop)
        {
            bar_tick = start_of_loop;
        }
        // reset the bar_tick_display so the bar pointer (on the bottom display starts from the first step again
        if (bar_tick_display == 16)
        {
            bar_tick_display = 0;
        }
        Serial.println(bar_tick);
        drawbarPosition();
    }
}
void Clock::drawbarPosition()
{

    // draw phrasenumber
    tft->fillRect(STEP_FRAME_W * POSITION_BAR_BUTTON + 1, 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, ILI9341_DARKGREY);
    tft->setTextColor(ILI9341_WHITE);
    tft->setFont(Arial_9);
    tft->setCursor(STEP_FRAME_W * POSITION_BAR_BUTTON + 4, 3);
    tft->print(bar_tick);

    // drawbarPosition
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        tft->drawPixel(bar_tick + STEP_FRAME_W * 2, (SONG_POSITION_POINTER_Y + songPointerThickness), ILI9341_GREEN);

        tft->drawFastHLine((bar_tick_display)*phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_GREEN);
        tft->drawFastHLine((bar_tick_display - 1) * phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_DARKGREY);
    }
    if (bar_tick_display == 0)
    {
        // pixelphrase = 0;
        tft->fillRect(STEP_FRAME_W * 17, BAR_POSITION_POINTER_Y, STEP_FRAME_W, 4, ILI9341_DARKGREY);
        // tft->fillRect(STEP_FRAME_W * 2, GRID_POSITION_POINTER_Y, STEP_FRAME_W * 1, 4, ILI9341_GREEN);
    }
    // Serial.println(step_tick);
    // Serial.println("bar");
    tft->updateScreenAsync();
}


Clock Masterclock(&tft);
