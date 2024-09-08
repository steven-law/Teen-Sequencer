#include <ownLibs/myClock.h>

ILI9341_t3n *MyClock::tft = nullptr; // Hier wird der Display-Pointer auf `nullptr` gesetzt, muss aber im Code richtig initialisiert werden
byte MyClock::tempo = 120;
byte MyClock::startOfLoop = 0;
byte MyClock::endOfLoop = 4;

byte MyClock::barTick = -1;
MyClock *MyClock::instance = nullptr;

MyClock::MyClock(ILI9341_t3n *display)
{
    MyClock::tft = display; // Initialisierung der statischen Variable
    instance = this;        // Setzen der Instanz
}
void MyClock::setup()
{
    // Set the clock BPM to 120 BPM
    uClock.setTempo(tempo);
    uClock.init();                        // Inits the clock
    uClock.setOnSync24(onSync24Callback); // Set the callback function for the clock output to send MIDI Sync message.
    uClock.setOnStep(onStepCallback);     // Set the callback function for the step sequencer on 16ppqn
    uClock.setOnClockStart(onClockStart); // Set the callback function for MIDI Start and Stop messages.
    uClock.setOnClockStop(onClockStop);
}

void MyClock::onSync24Callback(uint32_t tick) // The callback function wich will be called by Clock each Pulse of 24PPQN clock resolution.
{
    // Send MIDI_CLOCK to external gears
    // Serial.write(MIDI_CLOCK);
    tick = tick % MAX_TICKS;
    // Serial.printf("tick: %d\n", tick);

    sendClock();
    clock_to_notes(tick);
}
void MyClock::onStepCallback(uint32_t tick) // Each call represents exactly one step.
{

    tick = tick % NUM_STEPS;
    if (tick == 0)
    {
        barTick++;
        barTick = barTick % endOfLoop; // Setze wert2 mit Modulo-Operator zurück
    }
    // Serial.printf("Bar: %d\n", tick);
    drawstepPosition(tick);
    drawbarPosition(barTick);
    for (int i = 0; i < NUM_TRACKS; i++)
    {
        trellisShowClockPixel[i] = true;
    }
}

void MyClock::onClockStart() // The callback function wich will be called when clock starts by using Clock.start() method.
{
    // Serial.write(MIDI_START);
}
void MyClock::onClockStop() // The callback function wich will be called when clock stops by using Clock.stop() method.
{
    barTick = startOfLoop;
    // Serial.write(MIDI_STOP);
}

void MyClock::set_tempo(byte _encoder)
{
    static byte tempo;
    if (enc_moved[_encoder])
    {
        tempo = constrain(tempo + encoded[_encoder], 50, 255);
        uClock.setTempo(tempo);
        draw_clock_option(POSITION_BPM_BUTTON, tempo);
    }
}
void MyClock::set_start()
{
    uClock.start();
}
void MyClock::set_stop()
{
    uClock.stop();
    barTick=-1;

}
void MyClock::draw_clock_option(byte x, byte v)
{
    tft->setCursor(STEP_FRAME_W * x + 2, 3);
    tft->setFont(Arial_10);
    tft->setTextColor(ILI9341_WHITE);
    tft->setTextSize(1);
    tft->fillRect(STEP_FRAME_W * x + 2, 1, STEP_FRAME_W * 2 - 4, STEP_FRAME_H - 2, ILI9341_DARKGREY);
    tft->print(v);
    tft->updateScreenAsync();
}

void MyClock::drawstepPosition(int current)
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
void MyClock::drawbarPosition(byte _bar)
{

    // draw phrasenumber
    tft->fillRect(STEP_FRAME_W * POSITION_BAR_BUTTON + 1, 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, ILI9341_DARKGREY);
    tft->setTextColor(ILI9341_WHITE);
    tft->setFont(Arial_9);
    tft->setCursor(STEP_FRAME_W * POSITION_BAR_BUTTON + 4, 3);
    tft->print(_bar);

    // drawbarPosition
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        tft->drawPixel(_bar + STEP_FRAME_W * 2, (SONG_POSITION_POINTER_Y + songPointerThickness), ILI9341_GREEN);

        tft->drawFastHLine((_bar)*phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_GREEN);
        tft->drawFastHLine((_bar - 1) * phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_DARKGREY);
    }
    if (_bar == 0)
    {
        // pixelphrase = 0;
        tft->fillRect(STEP_FRAME_W * (endOfLoop + 2), BAR_POSITION_POINTER_Y, STEP_FRAME_W, POSITION_POINTER_THICKNESS + 1, ILI9341_DARKGREY);
        // tft->fillRect(STEP_FRAME_W * 2, GRID_POSITION_POINTER_Y, STEP_FRAME_W * 1, 4, ILI9341_GREEN);
    }
    // Serial.println(step_tick);
    // Serial.println("bar");
    tft->updateScreenAsync();
}
void MyClock::set_start_of_loop(byte n)
{
    if (enc_moved[n])
    {
        startOfLoop = constrain(startOfLoop + encoded[n], 0, 254);

        draw_clock_option(POSITION_START_LOOP_BUTTON, startOfLoop);

        enc_moved[n] = false;
    }
}

void MyClock::set_end_of_loop(byte n)
{
    if (enc_moved[n])
    {
        endOfLoop = constrain(endOfLoop + encoded[n], 2, 255);

        draw_clock_option(POSITION_END_LOOP_BUTTON, endOfLoop);

        enc_moved[n] = false;
    }
}

void MyClock::save_clock(byte _songNr)
{
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");
    neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;

    sprintf(_trackname, "%dclock.txt\0", _songNr);
    Serial.println(_trackname);

    // delete the file:
    // Serial.println("Removing:");
    SD.remove(_trackname);
    // Serial.println("Done:");

    // open the file.
    // Serial.println("Creating and opening:");
    myFile = SD.open(_trackname, FILE_WRITE);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (myFile)
    {
        // save tracks
        // Serial.println("Writing track:");
        // Serial.println("settings & seqmodes saved:");
        byte _tempo = tempo / 2;
        byte _startOfLoop = startOfLoop / 2;
        byte _endOfLoop = endOfLoop / 2;
        myFile.print((char)_tempo);
        myFile.print((char)_startOfLoop);
        myFile.print((char)_endOfLoop);
        // close the file:
        myFile.close();
        // Serial.println("all saved:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }

    Serial.println("clock saving Done:");
    // startUpScreen();
}
void MyClock::load_clock(byte _songNr)
{
    byte _tempo;
    byte _startOfLoop;
    byte _endOfLoop;
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in load mode");
    sprintf(_trackname, "%dclock.txt\0", _songNr);
    Serial.println(_trackname);
    //  open the file for reading:
    myFile = SD.open(_trackname, FILE_READ);
    // Serial.println(_trackname);
    if (myFile)
    {
        // Serial.println("opening:");
        //  read from the file until there's nothing else in it:
        //  load track 1

        _tempo = myFile.read();
        _startOfLoop = myFile.read();
        _endOfLoop = myFile.read();

        // Serial.println("settings loaded:");

        // startUpScreen();
        //  close the file:
        myFile.close();
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }
    tempo = _tempo * 2;
    startOfLoop = _startOfLoop * 2;
    endOfLoop = _endOfLoop * 2;
    uClock.setTempo(tempo);
    draw_clock_option(POSITION_BPM_BUTTON, tempo);
    draw_clock_option(POSITION_START_LOOP_BUTTON, startOfLoop);
    draw_clock_option(POSITION_END_LOOP_BUTTON, endOfLoop);
    Serial.println("clock Loading done");
}

// MyClock myClock(&tft);