#include <Arduino.h>
#include "Track.h"
// File myFile;
//  MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
//   midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midi_out_serial = &MIDI1;
void Track::update(int PixelX, byte gridY)
{
    // MIDI1.read();
    if (PixelX > SEQ_GRID_LEFT && PixelX < 14 * STEP_FRAME_W && gridY >= 1 && gridY < 13)
    {
        // note2set = (gridY - SEQ_GRID_TOP) + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        // tickStart = (PixelX - SEQ_GRID_LEFT) / 2;
        // pixelOn_X = PixelX;
        // pixelOn_Y = gridY;
    }
    bar_to_edit = ((PixelX - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * (arrangerpage));
    gridX_4_save = PixelX / STEP_FRAME_W;
    gridY_4_save = gridY;
    // save_track();
    // load_track();
}
void Track::save_track()
{
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");
    trellisPressed[TRELLIS_BUTTON_ENTER] = false;

    sprintf(_trackname, "track%d.txt\0", MIDI_channel_in);
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

        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t <= MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    myFile.print((char)this->clip[c].tick[t].voice[v]);
                    myFile.print((char)this->clip[c].tick[t].velo[v]);
                }
                myFile.print((char)this->clip[c].tick[t].stepFX);
            }
        }
        for (int t = 0; t <= MAX_TICKS; t++)
        {
            // Serial.printf("save track: %d, tick: %d, note: %d, channel out; %d\n", MIDI_channel_in, t, this->clip[0].tick[t].voice[0], parameter[SET_MIDICH_OUT]);
        }
        // Serial.println("array saved:");
        for (int i = 0; i < 256; i++)
        {
            myFile.print((char)clip_to_play[i]);
            myFile.print((char)noteOffset[i]);
            myFile.print((char)barVelocity[i]);
            myFile.print((char)play_presetNr_ccChannel[i]);
            myFile.print((char)play_presetNr_ccValue[i]);
        }
        // Serial.println("song saved:");
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < NUM_PARAMETERS; t++)
            {
                myFile.print((char)CCchannel[p][t]);
                myFile.print((char)CCvalue[p][t]);
            }
        }
        // Serial.println("midi saved:");
        for (int i = 0; i < NUM_PARAMETERS; i++)
        {
            myFile.print((char)parameter[i]);
            myFile.print((char)SeqMod1Value[i]);
            myFile.print((char)SeqMod2Value[i]);
            myFile.print((char)SeqMod3Value[i]);
            myFile.print((char)SeqMod4Value[i]);
            Serial.printf("save parameter %d = %d\n", i, parameter[i]);
        }
        // Serial.println("settings & seqmodes saved:");
        myFile.print((char)setStepFX);
        myFile.print((char)mixGainPot);
        myFile.print((char)mixDryPot);
        myFile.print((char)mixFX1Pot);
        myFile.print((char)mixFX2Pot);
        myFile.print((char)mixFX3Pot);
        // close the file:
        myFile.close();
        // Serial.println("all saved:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }
    if (parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    {
        allPlugins[parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)]->save_plugin();
    }

    Serial.println("track saving Done:");
    // startUpScreen();
}
void Track::load_track()
{

    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in load mode");
    sprintf(_trackname, "track%d.txt\0", MIDI_channel_in);
    Serial.println(_trackname);
    //  open the file for reading:
    myFile = SD.open(_trackname, FILE_READ);
    // Serial.println(_trackname);
    if (myFile)
    {
        // Serial.println("opening:");
        //  read from the file until there's nothing else in it:
        //  load track 1
        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t <= MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    this->clip[c].tick[t].voice[v] = myFile.read();
                    // Serial.printf("clip: %d, tick: %d, voice: %d, note: %d\n", c, t, v, this->array[0][t][0]);
                    this->clip[c].tick[t].velo[v] = myFile.read();
                }
                this->clip[c].tick[t].stepFX = myFile.read();
            }
        }
        for (int i = 0; i < MAX_TICKS; i++)
        {
            for (int v = 0; v < MAX_VOICES; v++)
            {
                if (clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[v] < NO_NOTE)
                {
                    trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (i / 6), (MIDI_channel_in - 1), trellisTrackColor[MIDI_channel_in - 1]);
                }
            }
        }
        for (int t = 0; t <= MAX_TICKS; t++)
        {
            // Serial.printf("load track: %d, tick: %d, note: %d, channel out; %d\n", MIDI_channel_in, t, this->clip[0].tick[t].voice[0], parameter[SET_MIDICH_OUT]);
        }
        // Serial.println("array loaded:");

        for (int i = 0; i < 256; i++)
        {
            clip_to_play[i] = myFile.read();
            noteOffset[i] = myFile.read();
            barVelocity[i] = myFile.read();
            play_presetNr_ccChannel[i] = myFile.read();
            play_presetNr_ccValue[i] = myFile.read();
            if (clip_to_play[i] <= NUM_USER_CLIPS)
            {
                Serial.println((i / 16) + TRELLIS_SCREEN_ARRANGER_1);
                trellis_set_main_buffer((i / 16) + TRELLIS_SCREEN_ARRANGER_1, i % 16, (MIDI_channel_in - 1), trellisTrackColor[MIDI_channel_in - 1] + (clip_to_play[i] * 20));
            }
        }
        // Serial.println("song loaded:");

        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < NUM_PARAMETERS; t++)
            {
                CCchannel[p][t] = myFile.read();
                CCvalue[p][t] = myFile.read();
            }
        }
        // Serial.println("midi loaded:");

        for (int i = 0; i < NUM_PARAMETERS; i++)
        {
            parameter[i] = myFile.read();
            SeqMod1Value[i] = myFile.read();
            SeqMod2Value[i] = myFile.read();
            SeqMod3Value[i] = myFile.read();
            SeqMod4Value[i] = myFile.read();
            Serial.printf("load parameter %d = %d\n", i, parameter[i]);
        }
        setStepFX = myFile.read();
        mixGainPot = myFile.read();
        mixDryPot = myFile.read();
        mixFX1Pot = myFile.read();
        mixFX2Pot = myFile.read();
        mixFX3Pot = myFile.read();
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
    if (parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    {
        allPlugins[parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)]->load_plugin();
    }
    Serial.println("track Loading done");
}
void Track::play_sequencer_mode(byte cloock, byte start, byte end)
{
    if (cloock % 96 == 0)
        external_clock_bar++;
    if (external_clock_bar == end)
        external_clock_bar = start;
    if (cloock % parameter[SET_STEP_DIVIVISION] == 0)
    {
        internal_clock++;
        internal_clock_is_on = true;
        // if (internal_clock % 6 == 0)
        // trellis_show_clockbar(MIDI_channel_in, internal_clock / 6);
    }
    else
        internal_clock_is_on = false;

    if (internal_clock >= parameter[SET_SEQUENCE_LENGTH])
    {
        internal_clock = 0;
        internal_clock_bar++;
        change_presets();
    }
    if (internal_clock_bar == end)
        internal_clock_bar = start;
    // Serial.printf("bar: %d, tick: %d\n", internal_clock_bar, internal_clock);
    //  Serial.println(internal_clock_bar);
    if (internal_clock_is_on)
    {
        if (!muted && !muteThruSolo)
        {
            if (parameter[SET_SEQ_MODE] == 0)
            {
                play_seq_mode0(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 1)
            {
                play_seq_mode1(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 2)
            {
                play_seq_mode2(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 3)
            {
                play_seq_mode3(internal_clock);
            }
            if (parameter[SET_SEQ_MODE] == 4)
            {
                play_seq_mode4(internal_clock);
            }
        }
    }
}
void Track::set_seq_mode_parameters(byte row)
{
    if (parameter[SET_SEQ_MODE] == 1)
        set_seq_mode1_parameters(row);
    if (parameter[SET_SEQ_MODE] == 2)
        set_seq_mode2_parameters(row);
    if (parameter[SET_SEQ_MODE] == 3)
        set_seq_mode3_parameters(row);
    if (parameter[SET_SEQ_MODE] == 4)
        set_seq_mode4_parameters(row);
}
void Track::draw_sequencer_modes(byte mode)
{
    clearWorkSpace();
    change_plugin_row = true;
    if (mode == 1)
        draw_seq_mode1();
    if (mode == 2)
        draw_seq_mode2();
    if (mode == 3)
        draw_seq_mode3();
    if (mode == 4)
        draw_seq_mode4();
}

void Track::noteOn(byte Note, byte Velo, byte Channel)
{
    // Serial.printf("sending noteOn: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOn(Note, Velo, Channel);
    if (Note <= 64)
    {
        cvNoteOn = true;
        cvNote = Note;
        // Serial.printf("Tracks frees cv note: %d\n", cvNote);
        //  sendNoteOn_CV_Gate(Note, Channel);
    }
    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
    //
}
void Track::noteOff(byte Note, byte Velo, byte Channel)
{
    // Serial.printf("sending noteOff: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOff(Note, Velo, Channel);
    if (Note <= 64)
    {
        cvNoteOff = true;
    }
    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
}
void Track::record_noteOn(byte Note, byte Velo, byte Channel)
{
    recordVoice = Note % NOTES_PER_OCTAVE;
    recordLastNote[recordVoice] = Note;
    recordVelocity[recordVoice] = Velo;
    recordStartTick[recordVoice] = internal_clock;
    recordChannel = Channel;
    // clip[parameter[SET_CLIP2_EDIT]].tick[internal_clock].voice[0] = Note;
    // clip[parameter[SET_CLIP2_EDIT]].tick[internal_clock].velo[0] = Velo;
}
void Track::record_noteOff(byte Note, byte Velo, byte Channel)
{
    recordVoice = Note % NOTES_PER_OCTAVE;
    if (recordLastNote[recordVoice] == Note)
    {
        for (int i = recordStartTick[recordVoice]; i <= internal_clock; i++)
        {
            clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[recordVoice] = Note;
            clip[parameter[SET_CLIP2_EDIT]].tick[i].velo[recordVoice] = recordVelocity[recordVoice];
            trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (i / 6), (MIDI_channel_in - 1), trellisTrackColor[MIDI_channel_in - 1]);
        }
    }
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
        if (lastProw == 1)
        {
            draw_barVelocity(0, 1);
        }
        if (lastProw == 2)
        {
            // draw_barVelocity(0, 1);
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
        {
            Serial.print("cc:");
            Serial.println(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i]);
            sendControlChange(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i], CCvalue[play_presetNr_ccValue[internal_clock_bar]][i], parameter[SET_MIDICH_OUT]);
        }
    }
}
// clip to play
void Track::set_clip_to_play(byte n, byte b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        if (enc_moved[n])
        {

            for (int i = 0; i < parameter[SET_STEP_DIVIVISION]; i++)
            {
                clip_to_play[bar_to_edit + i] = constrain(clip_to_play[bar_to_edit + i] + encoded[n], 0, NUM_USER_CLIPS + 1);
                draw_arrangment_line(n, bar_to_edit + i);
            }
            draw_clip_to_play(n, bar_to_edit);
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
        // Serial.printf("active page = %d, which bar = %d\n", b, i + (16 * (b - SONGMODE_PAGE_1)));
    }
}
void Track::draw_arrangment_line(byte n, byte b) // b= 0-255; which bar
{
    int minY = map(barVelocity[b], 0, 127, 0, 10);

    if (clip_to_play[b] == MAX_CLIPS - 1)
    {
        for (int thickness = -10; thickness < 10; thickness++)
        {
            tft->drawFastHLine(((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((my_Arranger_Y_axis)*TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, ILI9341_DARKGREY); //(x-start, y, length, color)
        }
        trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (b % 16), my_Arranger_Y_axis - 1, TRELLIS_BLACK);
    }
    else
    {
        // for other clips
        for (int thickness = -minY; thickness < minY; thickness++)
        {
            tft->drawFastHLine(((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((my_Arranger_Y_axis)*TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, trackColor[my_Arranger_Y_axis - 1] + (clip_to_play[b] * 20)); //(x-start, y, length, color)
        }
        trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (b % 16), my_Arranger_Y_axis - 1, trellisTrackColor[my_Arranger_Y_axis - 1] + (clip_to_play[b] * 20));

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
            int _tempOffset = constrain(noteOffset[when] + encoded[n], -99, +99);

            draw_noteOffset(n, when);
            for (int i = 0; i < parameter[SET_STEP_DIVIVISION]; i++)
            {
                noteOffset[when + i] = _tempOffset;
                draw_arrangment_line(n, when + i);
            }
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
// velocity per bar
void Track::set_barVelocity(byte n, int b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        byte when = ((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * arrangerpage);
        if (enc_moved[n])
        {
            barVelocity[when] = constrain(barVelocity[when] + encoded[n], 0, 127);
            draw_barVelocity(n, when);
            for (int i = 0; i < parameter[SET_STEP_DIVIVISION]; i++)
            {

                draw_arrangment_line(n, when + i);
            }
            enc_moved[n] = false;
        }
    }
}
void Track::draw_barVelocity(byte n, int b)
{
    draw_Text(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Velo", encoder_colour[n], false, false);
    draw_Value(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, barVelocity[b], encoder_colour[n], true, false);
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
