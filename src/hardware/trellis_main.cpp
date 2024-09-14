#include "hardware/trellis_main.h"

void TrellisForMainGrid::setup()
{
    trellis->begin(0x70, 0x72, 0x73, 0x74, 0x71, 0x75, 0x76, 0x77);
}

void TrellisForMainGrid::assign_start_buffer(byte _lastPotRow)
{
    for (int i = 0; i < TRELLIS_PADS_Y_DIM - 1; i++)
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

        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 3, i, TRELLIS_PINK);
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 7, i, TRELLIS_OLIVE);
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 11, i, TRELLIS_AQUA);
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 15, i, TRELLIS_ORANGE);
    }

    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4, 7, TRELLIS_BLUE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 1, 7, TRELLIS_RED);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 3, 7, TRELLIS_WHITE);

    // neotrellis_recall_control_buffer();
    Serial.println("trellis setup done");
}
void TrellisForMainGrid::update(byte _trellisScreen, byte _lastpotrow)
{
    _lastPotRow = _lastpotrow;
    activePage = _trellisScreen;
    unsigned long trellisCurrentMillis = millis();
    if (trellisCurrentMillis - trellisReadPreviousMillis >= trellisReadInterval)
    {
        trellisReadPreviousMillis = trellisCurrentMillis;
        read();
        //trellis_recall_main_buffer(_trellisScreen);
        trellis_set_arranger();
        trellis_play_piano();
        trellis_perform();
        trellis_show_clockbar();
        trellis_setStepsequencer();
       // trellis_play_mixer();

        trellis_writeDisplay();
    }
}
void TrellisForMainGrid::read()
{

    // If a button was just pressed or released...
    if (trellis->readSwitches())
    {
        // go through every button
        for (uint8_t i = 0; i < numKeys; i++)
        {
            // if it was pressed, turn it on
            if (trellis->justPressed(i))
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
            if (trellis->justReleased(i))
            {
                int _nr = (i % 4) + 4 * (i / 16) + (i / 4 % 4) * 16;
                if (i > 63)
                {
                    _nr = _nr + 48;
                }
                trellisPressed[_nr] = false;
            }
        }
    }
}

void TrellisForMainGrid::trellis_set_main_buffer(int _page, int _x, int _y, int color)
{
    trellisMainGridBuffer[_page][_x][_y] = color;
    int _nr = _x + (_y * TRELLIS_PADS_X_DIM);
    trellis->setLED(TrellisLED[_nr]);
    if (trellisMainGridBuffer[_page][_x][_y] == 0)
        trellis->clrLED(TrellisLED[_nr]);
    Serial.printf("set main buffer page: %d, x: %d, y: %d, color: %d, trellisNr: %d\n", _page, _x, _y, color, TrellisLED[_nr]);
}
void TrellisForMainGrid::trellis_recall_main_buffer(int _page)
{

    for (int i = 0; i < TRELLIS_PADS_X_DIM; i++)
    {
        for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
        {
            // if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
            {
                int _nr = i + (y * TRELLIS_PADS_X_DIM);

                if (trellisMainGridBuffer[_page][i][y] > 0)
                    trellis->setLED(TrellisLED[_nr]);
                else
                    trellis->clrLED(TrellisLED[_nr]);
                // Serial.printf("recall main buffer page: %d, x: %d, y: %d, color: %d, trellisNr: %d\n", _page, i, y, trellisMainGridBuffer[_page][i][y], TrellisLED[_nr]);
            }
        }
    }
    Serial.println("load main buffer");
}
int TrellisForMainGrid::trellis_get_main_buffer(int _page, int _x, int _y)
{
    // Serial.println("return main buffer");
    return trellisMainGridBuffer[_page][_x][_y];
}
void TrellisForMainGrid::trellis_clear_main_buffer(int _page)
{

    for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
    {
        for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
        {
            int _nr = x + (y * TRELLIS_PADS_X_DIM);
            trellisMainGridBuffer[_page][x][y] = TRELLIS_BLACK;
            trellis->clrLED(TrellisLED[_nr]);
        }
    }
}

void TrellisForMainGrid::trellis_show_clockbar()
{
    if (myClock.isPlaying)
    {
        if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8 || trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
        {
            int color;
            for (int trackNr = 0; trackNr < TRELLIS_PADS_Y_DIM; trackNr++)
            {
                trellisShowClockPixel[trackNr] = false;

                if (allTracks[trackNr]->get_recordState())
                    color = TRELLIS_RED;
                else
                    color = TRELLIS_ORANGE;

                if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
                {
                    for (int step = 0; step < TRELLIS_PADS_X_DIM; step++)
                    {
                        byte _nr = step + (trackNr * NUM_STEPS);
                        trellis->setLED(TrellisLED[_nr]);
                        if (step > 0)
                        {
                            if (trellis_get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], step - 1, trackNr) > 0)
                                trellis->setLED(TrellisLED[_nr - 1]);
                            else
                                trellis->clrLED(TrellisLED[_nr - 1]);
                        }
                        if (step == 0)
                        {
                            if (trellis_get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], (allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1, trackNr) > 0)
                                trellis->setLED(TrellisLED[((allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
                            else
                                trellis->clrLED(TrellisLED[((allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
                            // byte oldNr = (allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1;
                            // neotrellis.setPixelColor(oldNr, trackNr, trellis_get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], oldNr, trackNr));
                            //  Serial.println(oldNr);
                        }
                    }
                }
                else
                {
                    // if (step % 16 == 0)
                    {
                        byte _nr = myClock.barTick + (trackNr * NUM_STEPS);
                        trellis->setLED(TrellisLED[_nr]);

                        if (myClock.barTick > 0)
                        {
                            if (trellis_get_main_buffer(trellisScreen, myClock.barTick - 1, trackNr) > 0)
                                trellis->setLED(TrellisLED[_nr - 1]);
                            else
                                trellis->clrLED(TrellisLED[_nr - 1]);
                        }
                        // neotrellis.setPixelColor(myClock.barTick - 1, trackNr, trellis_get_main_buffer(trellisScreen, myClock.barTick - 1, trackNr));
                        if (myClock.barTick == 0)
                        {
                            if (trellis_get_main_buffer(trellisScreen, myClock.endOfLoop - 1, trackNr) > 0)
                                trellis->setLED(TrellisLED[(myClock.endOfLoop - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
                            else
                                trellis->clrLED(TrellisLED[(myClock.endOfLoop - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
                            //  neotrellis.setPixelColor(myClock.endOfLoop - 1, trackNr, trellis_get_main_buffer(trellisScreen, myClock.endOfLoop - 1, trackNr));
                            // Serial.println(oldNr);
                        }
                    }
                }
            }
        }
    }
}
void TrellisForMainGrid::trellis_writeDisplay()
{
    // tell the trellis to set the LEDs we requested
    trellis->writeDisplay();
}

void TrellisForMainGrid::trellis_play_piano()
{

    if (activePage == TRELLIS_SCREEN_PIANO)
    {
        static bool _holdNote[NUM_STEPS * NUM_TRACKS];
        for (int x = 0; x < NUM_STEPS; x++)
        {
            for (int y = 0; y < NUM_TRACKS; y++)
            {
                // byte _keyNr = x + (y * TRELLIS_PADS_X_DIM);
                byte _keynote = x + (y * NUM_STEPS);
                if (trellisPressed[_keynote] && !_holdNote[_keynote])
                {
                    // Serial.printf("pianoPage _keyNr:%d, _keyNote: %d\n", _keyNr, _keynote);
                    _holdNote[_keynote] = true;

                    allTracks[trellisPianoTrack]->noteOn(_keynote, 99, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
                    if (allTracks[trellisPianoTrack]->get_recordState())
                        allTracks[trellisPianoTrack]->record_noteOn(_keynote, 99, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
                    Serial.printf("trellisPiano NoteON key:%d, track:%d\n", _keynote, trellisPianoTrack);
                    Serial.println(_holdNote[_keynote]);
                    break;
                }

                else if (!trellisPressed[_keynote] && _holdNote[_keynote])
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
void TrellisForMainGrid::trellis_set_arranger()
{
    if (activePage >= TRELLIS_SCREEN_ARRANGER_1 && !neotrellisPressed[TRELLIS_BUTTON_ARRANGER])
    {
        if (!neotrellisPressed[TRELLIS_BUTTON_RECORD])
        {
            for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
            {
                for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
                {
                    byte _key = x + (y * TRELLIS_PADS_X_DIM);
                    byte _clipPos = _key % TRELLIS_PADS_X_DIM;
                    byte _track = _key / TRELLIS_PADS_X_DIM;
                    byte _clipNr = gridTouchY;
                    byte _clipTrack = _clipPos + (arrangerpage * 16);
                    if (_clipPos < 16)
                    {
                        if (trellisPressed[_key])
                        {
                            trellisPressed[_key] = false;
                            change_plugin_row = true;
                            if (activeScreen >= INPUT_FUNCTIONS_FOR_ARRANGER)
                            {
                                for (int i = 0; i < allTracks[_track]->parameter[SET_STEP_DIVIVISION]; i++)
                                {
                                    allTracks[_track]->clip_to_play[_clipTrack + i] = _clipNr;
                                    allTracks[_track]->draw_clip_to_play(3, _clipTrack + i);
                                    allTracks[_track]->draw_arrangment_line(3, _clipTrack + i);
                                    // trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, _clipPos + i, _track, _color);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void TrellisForMainGrid::draw_perform_page()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        for (int i = 0; i < NUM_STEPS; i++)
        {
            drawPot(i % 4, i / 4, performCC[i], "CC");
            trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, i, 7, TRELLIS_BLACK);
        }
        trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4, 7, TRELLIS_BLUE);
        trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 1, 7, TRELLIS_RED);
        trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
        trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 3, 7, TRELLIS_WHITE);
    }
}
void TrellisForMainGrid::set_performCC(byte XPos, byte YPos, const char *name)
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
void TrellisForMainGrid::set_perform_page(byte row)
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
void TrellisForMainGrid::trellis_setStepsequencer()
{
    if (activePage < TRELLIS_SCREEN_SEQUENCER_CLIP_8)
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
