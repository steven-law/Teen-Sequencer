#include <Arduino.h>
#include "Track.h"
File myFile;
// MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
//  midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midi_out_serial = &MIDI1;
void Track::update(int PixelX, byte gridY)
{
    // MIDI1.read();
    if (PixelX > SEQ_GRID_LEFT && PixelX < 14 * STEP_FRAME_W && gridY >= 1 && gridY < 13)
    {
        note2set = (gridY - SEQ_GRID_TOP) + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        tick_start = (PixelX - SEQ_GRID_LEFT) / 2;
        pixelOn_X = PixelX;
        pixelOn_Y = gridY;
        bar_to_edit = ((PixelX - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * (arrangerpage));
    }
    gridX_4_save = PixelX / STEP_FRAME_W;
    gridY_4_save = gridY;
    saveTrack();
    loadTrack();
}
void Track::saveTrack()
{
    if (buttonPressed[BUTTON_ENTER])
    {
        Serial.printf("X= %d, Y= %d\n", gridX_4_save, gridY_4_save);
        if (gridX_4_save >= 13 && gridX_4_save <= 14 && gridY_4_save == 0)
        {
            SD.begin(BUILTIN_SDCARD);
            Serial.println("in save mode:");
            buttonPressed[BUTTON_ENTER] = false;

            sprintf(_trackname, "track%d.txt\0", MIDI_channel_in);
            Serial.println(_trackname);
            tft->fillScreen(ILI9341_DARKGREY);
            tft->setTextColor(ILI9341_WHITE);
            tft->setFont(Arial_8);
            tft->setCursor(0, 0);

            // delete the file:
            tft->print("Removing:");
            Serial.println("Removing:");
            tft->print(_trackname);
            SD.remove(_trackname);
            tft->println("Done");
            Serial.println("Done:");

            // open the file.
            tft->print("Creating and opening:");
            Serial.println("Creating and opening:");

            tft->print(_trackname);

            myFile = SD.open(_trackname, FILE_WRITE);
            Serial.println(_trackname);
            tft->println("Done");
            Serial.println("Done:");
            // if the file opened okay, write to it:
            if (myFile)
            {
                // save tracks
                tft->print("Writing track to:");
                tft->print(_trackname);
                Serial.println("Writing track:");

                for (int c = 0; c < MAX_CLIPS; c++)
                {
                    for (int t = 0; t <= MAX_TICKS ; t++)
                    {
                        for (int v = 0; v < MAX_VOICES; v++)
                        {
                            myFile.print((char)array[c][t][v]);
                            myFile.print((char)velocity[c][t][v]);
                        }
                    }
                }
                for (int t = 0; t < MAX_TICKS; t++)
                    Serial.printf(" tick: %d,  note: %d\n", t, array[0][t][0]);

                Serial.println("array saved:");
                for (int i = 0; i < 256; i++)
                {
                    myFile.print((char)clip_to_play[i]);
                    myFile.print((char)noteOffset[i]);
                    myFile.print((char)play_presetNr_ccChannel[i]);
                    myFile.print((char)play_presetNr_ccValue[i]);
                }
                Serial.println("song saved:");
                for (int p = 0; p < NUM_PRESETS; p++)
                {
                    for (int t = 0; t < NUM_PARAMETERS + 1; t++)
                    {
                        myFile.print((char)CCchannel[p][t]);
                        myFile.print((char)CCvalue[p][t]);
                    }
                }
                Serial.println("midi saved:");
                for (int i = 0; i < NUM_PARAMETERS; i++)
                    myFile.print((char)parameter[i]);
                Serial.println("settings saved:");

                // close the file:
                myFile.close();
                tft->println("Done");
                Serial.println("all saved:");
            }

            else
            {
                // if the file didn't open, print an error:
                tft->println("error opening:");
                Serial.println("error:");
                tft->print(_trackname);
            }

            tft->println("Saving done.");
            Serial.println("saving Done:");
            startUpScreen();
        }
    }
}
void Track::loadTrack()
{
    if (buttonPressed[BUTTON_ENTER])
    {
        buttonPressed[BUTTON_ENTER] = false;
        Serial.printf("X= %d, Y= %d\n", gridX_4_save, gridY_4_save);

        if (gridX_4_save == 15 && gridY_4_save == 0)
        {
            SD.begin(BUILTIN_SDCARD);
            Serial.println("in load mode");
            sprintf(_trackname, "track%d.txt\0", MIDI_channel_in);
            Serial.println(_trackname);
            tft->fillScreen(ILI9341_DARKGREY);
            tft->setFont(Arial_8);
            tft->setTextColor(ILI9341_WHITE);
            tft->setCursor(0, 0);
            // open the file for reading:
            myFile = SD.open(_trackname);
            Serial.println(_trackname);
            if (myFile)
            {
                tft->println("opening:");
                Serial.println("opening:");

                tft->println(_trackname);

                // read from the file until there's nothing else in it:
                // load track 1
                tft->print("Reading clips from:");
                tft->println(_trackname);

                for (int c = 0; c < MAX_CLIPS; c++)
                {
                    for (int t = 0; t <= MAX_TICKS ; t++)
                    {
                        for (int v = 0; v < MAX_VOICES; v++)
                        {
                            array[c][t][v] = myFile.read();
                            // Serial.printf("clip: %d, tick: %d, voice: %d, note: %d\n", c, t, v, array[0][t][0]);
                            velocity[c][t][v] = myFile.read();
                        }
                    }
                }
                for (int t = 0; t < MAX_TICKS; t++)
                    Serial.printf(" tick: %d,  note: %d\n", t, array[0][t][0]);

                Serial.println("array loaded:");

                for (int i = 0; i < 256; i++)
                {
                    clip_to_play[i] = myFile.read();
                    noteOffset[i] = myFile.read();
                    play_presetNr_ccChannel[i] = myFile.read();
                    play_presetNr_ccValue[i] = myFile.read();
                }
                Serial.println("song loaded:");

                for (int p = 0; p < NUM_PRESETS; p++)
                {
                    for (int t = 0; t < NUM_PARAMETERS + 1; t++)
                    {
                        CCchannel[p][t] = myFile.read();
                        CCvalue[p][t] = myFile.read();
                    }
                }
                Serial.println("midi loaded:");

                for (int i = 0; i < NUM_PARAMETERS; i++)
                    parameter[i] = myFile.read();
                Serial.println("settings loaded:");

                tft->println("Done");
                Serial.println("Loading done");

                startUpScreen();
                // close the file:
                myFile.close();
            }
            else
            {
                // if the file didn't open, print an error:
                Serial.println("error:");

                tft->println("error opening:");
                tft->println(_trackname);
            }
        }
    }
}
void Track::play_sequencer_mode(byte cloock, byte start, byte end)
{
    if (cloock % parameter[SET_STEP_DIVIVISION] == 0)
    {
        internal_clock++;
        internal_clock_is_on = true;
    }
    else
        internal_clock_is_on = false;
    if (internal_clock == parameter[SET_SEQUENCE_LENGTH])
    {
        internal_clock = 0;
        internal_clock_bar++;
        // change_presets();
    }
    if (internal_clock_bar == end)
        internal_clock_bar = start;
    // Serial.printf("bar: %d, tick: %d\n", internal_clock_bar, internal_clock);
    //  Serial.println(internal_clock_bar);
    if (internal_clock_is_on)
    {
        if (!muted || soloed)
        {
            if (parameter[SET_SEQ_MODE] == 0)
            {
                play_SeqMode0(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 1)
            {
                play_SeqMode1(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 2)
            {
                play_SeqMode2(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 3)
            {
                play_SeqMode3(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 4)
            {
                play_SeqMode4(internal_clock);
            }
        }
    }
}
void Track::set_SeqMode_parameters(byte row)
{
    if (parameter[SET_SEQ_MODE] == 1)
        set_SeqMode1_parameters(row);
    if (parameter[SET_SEQ_MODE] == 2)
        set_SeqMode2_parameters(row);
    if (parameter[SET_SEQ_MODE] == 3)
        set_SeqMode3_parameters(row);
    if (parameter[SET_SEQ_MODE] == 4)
        set_SeqMode4_parameters(row);
}
void Track::draw_sequencer_modes(byte mode)
{
    clearWorkSpace();
    change_plugin_row = true;
    if (mode == 1)
        draw_SeqMode1();
    if (mode == 2)
        draw_SeqMode2();
    if (mode == 3)
        draw_SeqMode3();
    if (mode == 4)
        draw_SeqMode4();
}

void Track::noteOn(byte Note, byte Velo, byte Channel)
{
    Serial.printf("sending noteOn: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOn(Note, Velo, Channel);
    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
    //
}
void Track::noteOff(byte Note, byte Velo, byte Channel)
{
    Serial.printf("sending noteOff: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOff(Note, Velo, Channel);
    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
}

//---------------------------arranger stuff-------------------------------------
void Track::draw_arranger_parameters(byte lastProw)
{
    if (change_plugin_row)
    {
        tft->fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        if (lastProw == 0)
        {
            draw_clip_to_play(2, 0);
            draw_noteOffset(3, 0);
            // drawOctaveNumber();
            // draw_velocity(3, 0);
        }
        if (lastProw == 2)
        {
            draw_play_presetNr_ccChannel(2, 2);
            draw_play_presetNr_ccValue(3, 2);
            // drawOctaveNumber();
            // draw_velocity(3, 0);
        }
    }
}
void Track::change_presets()
{
    for (int i = 0; i < 16; i++)
    {
        if (CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i] < 128)
            sendControlChange(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i], CCvalue[play_presetNr_ccValue[internal_clock_bar]][i], parameter[SET_MIDICH_OUT]);
    }
}
// clip to play
void Track::set_clip_to_play(byte n, byte b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        if (enc_moved[n])
        {
            clip_to_play[bar_to_edit] = constrain(clip_to_play[bar_to_edit] + encoded[n], 0, NUM_USER_CLIPS + 1);
            draw_clip_to_play(n, bar_to_edit);
            draw_arrangment_line(n, bar_to_edit);

            enc_moved[n] = false;
        }
    }
}
void Track::draw_clip_to_play(byte n, byte b)
{
    draw_Text(n, 0, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Clip", encoder_colour[n], false, false);
    draw_Value(n, 0, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, clip_to_play[b], encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "clNr", clip_to_play[b], n, 0);
}
void Track::drawsongmodepageselector()
{
    // draw 16 rects of 16x16px in the 13th row
    for (int pages = 2; pages < 18; pages++)
    {
        // drawActiveRect(pages, 13, 1, 1, selectPage == pages + 8, "", ILI9341_LIGHTGREY);
        tft->drawRect(STEP_FRAME_W * pages, STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
        tft->setFont(Arial_8);
        tft->setTextColor(ILI9341_WHITE);
        tft->setCursor(STEP_FRAME_W * pages + 3, STEP_FRAME_H * 13 + 8);
        tft->print((pages - 1));
    }
}
void Track::gridSongMode(int songpageNumber)
{ // static Display rendering
    // int page_phrase_start = songpageNumber * 16;
    // int page_phrase_end = (songpageNumber + 1) * 16;
    drawsongmodepageselector();
    // drawActiveRect(18, 3, 2, 2, false, "clear", ILI9341_RED);

    // vertical pointer Lines
    int shownLines = 257 / phraseSegmentLength;
    for (int f = 0; f < shownLines; f++)
    {                                                                                                 // do this for all phrases
        tft->drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 360); //(x, y-start, y-length, color)
        if (f % 4 == 0)
        {
            tft->drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 370); //(x, y-start, y-length, color)
        }
    }
}

void Track::draw_arrangment_lines(byte n, byte b) // b= active page
{
    for (int i = 0; i < 16; i++)
    {
        draw_arrangment_line(n, i + (BARS_PER_PAGE * (b - SONGMODE_PAGE_1)));
        Serial.printf("active page = %d, which bar = %d\n", b, i + (16 * (b - SONGMODE_PAGE_1)));
    }
}
void Track::draw_arrangment_line(byte n, byte b) // b= 0-255; which bar
{
    if (clip_to_play[b] == 8)
    {
        for (int thickness = -7; thickness < 7; thickness++)
        {
            tft->drawFastHLine(((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((my_Arranger_Y_axis)*TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, ILI9341_DARKGREY); //(x-start, y, length, color)
        }
    }
    else
    {
        // for other clips
        for (int thickness = -7; thickness < 7; thickness++)
        {
            tft->drawFastHLine(((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((my_Arranger_Y_axis)*TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, trackColor[my_Arranger_Y_axis - 1] + (clip_to_play[b] * 20)); //(x-start, y, length, color)
        }
        draw_clipNr_arranger(n, b);
        draw_offset_arranger(n, b);
    }
}
void Track::draw_clipNr_arranger(byte n, byte b)
{
    // draw clipnumber in the arranger
    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_BLACK);
    tft->setCursor((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + 2, (my_Arranger_Y_axis)*TRACK_FRAME_H + 6);
    tft->print(clip_to_play[b]);
}
byte Track::get_clip_to_play(byte when)
{
    return clip_to_play[when];
}
// note offset / note transpose
void Track::set_note_offset(byte n, int b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        byte when = ((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * arrangerpage);
        if (enc_moved[n])
        {
            noteOffset[when] = constrain(noteOffset[when] + encoded[n], -24, +24);
            draw_noteOffset(n, when);
            draw_arrangment_line(n, when);
            enc_moved[n] = false;
        }
    }
}
void Track::draw_noteOffset(byte n, int b)
{
    draw_Text(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Trns", encoder_colour[n], false, false);
    draw_Value(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, noteOffset[b], encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "ofSet", noteOffset[b], n, 0);
}
void Track::draw_offset_arranger(byte n, byte b)
{
    int xoffset;
    if (noteOffset[b] < 0)
        xoffset = 1;
    else
        xoffset = 5;
    // draw clipnumber in the arranger
    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_BLACK);
    tft->setCursor((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + xoffset, (my_Arranger_Y_axis)*TRACK_FRAME_H + 11);
    tft->print(noteOffset[b]);
}
// MIDICC
void Track::set_play_presetNr_ccChannel(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        play_presetNr_ccChannel[bar_to_edit] = constrain(play_presetNr_ccChannel[bar_to_edit] + encoded[n], 0, NUM_PRESETS);
        Serial.printf("cc-Set: %d, vl-set: %d, bar: %d\n", play_presetNr_ccChannel[bar_to_edit], play_presetNr_ccValue[bar_to_edit], bar_to_edit);
        change_plugin_row = true;
        // draw_MIDI_CC_screen();
        draw_play_presetNr_ccChannel(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_play_presetNr_ccChannel(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 0, 4, "cc-Set", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, play_presetNr_ccChannel[bar_to_edit], encoder_colour[n], true, false);
}
void Track::set_play_presetNr_ccValue(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        play_presetNr_ccValue[bar_to_edit] = constrain(play_presetNr_ccValue[bar_to_edit] + encoded[n], 0, NUM_PRESETS);
        Serial.printf("cc-Set: %d, vl-set: %d, bar: %d\n", play_presetNr_ccChannel[bar_to_edit], play_presetNr_ccValue[bar_to_edit], bar_to_edit);
        change_plugin_row = true;
        // draw_MIDI_CC_screen();
        draw_play_presetNr_ccValue(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_play_presetNr_ccValue(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 0, 4, "vl-Set", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, play_presetNr_ccValue[bar_to_edit], encoder_colour[n], true, false);
}

Track track1(&tft, 1);
Track track2(&tft, 2);
Track track3(&tft, 3);
Track track4(&tft, 4);

Track track5(&tft, 5);
Track track6(&tft, 6);
Track track7(&tft, 7);
Track track8(&tft, 8);

Track *allTracks[8]{&track1, &track2, &track3, &track4, &track5, &track6, &track7, &track8};
