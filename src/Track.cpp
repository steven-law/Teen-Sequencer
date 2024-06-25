#include <Arduino.h>
#include "Track.h"

void Track::update(int PixelX, byte gridY)
{
    if (PixelX > 2 * STEP_FRAME_W && PixelX < 14 * STEP_FRAME_W && gridY > 1 && gridY < 13)
    {
        note2set = (gridY - SEQ_GRID_TOP) + (octave * NOTES_PER_OCTAVE);
        tick_start = (PixelX - SEQ_GRID_LEFT) / 2;
        pixelOn_X = PixelX;
        pixelOn_Y = gridY;

        if (buttonPressed[BUTTON_SHIFT])
        {
            for (int t = 0; t < MAX_TICKS; t++)
            {
                // for (int v = 0; v < MAX_VOICES; v++)
                //{
                Serial.printf("tick:%d velocity:%d\n", t, velocity[0][t][0]);
                //}
                // Serial.println();
            }
            Serial.println();
            Serial.printf("clip2edit:%d\n", clip_to_edit);
            buttonPressed[BUTTON_SHIFT] = false;
        }
    }
}

void Track::play_sequencer_mode(byte cloock, byte start, byte end)
{

    if (cloock % step_division == 0)
    {
        internal_clock++;
        internal_clock_is_on = true;
    }
    else
        internal_clock_is_on = false;
    if (internal_clock == sequence_length)
    {
        internal_clock = 0;
        internal_clock_bar++;
    }
    if (internal_clock_bar == end)
        internal_clock_bar = start;
    // Serial.printf("bar: %d, tick: %d\n", internal_clock_bar, internal_clock);
    //  Serial.println(internal_clock_bar);
    if (internal_clock_is_on)
    {
        if (!muted || soloed)
        {
            if (sequencer_mode == 0)
            {
                play_SeqMode0(internal_clock);
            }
            if (sequencer_mode == 1)
            {
                play_SeqMode1(internal_clock);
            }
            if (sequencer_mode == 2)
            {
                play_SeqMode2(internal_clock);
            }
            if (sequencer_mode == 3)
            {
                play_SeqMode3(internal_clock);
            }
        }
    }
}
void Track::set_SeqMode_parameters(byte row)
{
    if (sequencer_mode == 1)
        set_SeqMode1_parameters(row);
    if (sequencer_mode == 2)
        set_SeqMode2_parameters(row);
    if (sequencer_mode == 3)
        set_SeqMode3_parameters(row);
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
}
/*void play_SeqMode0(byte cloock)
{

    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (array[clip_to_play[internal_clock_bar]][cloock][v] < NO_NOTE)
        {
            if (!note_is_on[v])
            {
                noteToPlay[v] = array[clip_to_play[internal_clock_bar]][cloock][v] + noteOffset[internal_clock_bar];
                note_is_on[v] = true;
                Masterout->noteOn(noteToPlay[v], VELOCITY_NOTE_ON, MIDI_channel_out, v);
            }
        }
        if (array[clip_to_play[internal_clock_bar]][cloock][v] == NO_NOTE)
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                Masterout->noteOff(noteToPlay[v], VELOCITY_NOTE_OFF, MIDI_channel_out, v);

                // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, v, noteToPlay[v]);
            }
        }
    }
}*/
void Track::noteOn(byte Note, byte Velo, byte Channel)
{
    Serial.printf("sending velo: %d\n", Velo);
    usbMIDI.sendNoteOn(Note, Velo, Channel);
}
void Track::noteOff(byte Note, byte Velo, byte Channel)
{
    usbMIDI.sendNoteOff(Note, Velo, Channel);
}
// sequencer options:
// octave
void Track::set_octave(byte n)
{
    if (enc_moved[n])
    {
        octave = constrain(octave + encoded[n], 0, 9);
        Serial.println(octave);
        drawOctaveNumber();
        clear_notes_in_grid();
        draw_notes_in_grid();
        enc_moved[n] = false;
    }
}
void Track::drawOctaveNumber()
{
    // draw the octave number
    tft->fillRect(STEP_FRAME_W * 18 + 1, STEP_FRAME_H * OCTAVE_CHANGE_TEXT, STEP_FRAME_W * 2, STEP_FRAME_H * 1 + 1, ILI9341_DARKGREY);
    tft->setCursor(STEP_FRAME_W * 18 + 11, STEP_FRAME_H * OCTAVE_CHANGE_TEXT);
    tft->setFont(Arial_16);
    tft->setTextColor(ILI9341_GREEN);
    tft->setTextSize(1);
    tft->print(octave);
}
byte Track::get_octave()
{
    return octave;
}
// sequencer Mode
void Track::set_sequencer_mode(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        sequencer_mode = constrain(sequencer_mode + encoded[n], 0, 16);
        draw_sequencer_mode(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_sequencer_mode(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "sMod", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, sequencer_mode, encoder_colour[n], true, false);

    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "sMod", sequencer_mode, n,0);
}
byte Track::get_sequencer_mode()
{
    return sequencer_mode;
}
// step division
void Track::set_step_division(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        step_division = constrain(step_division + encoded[n], 1, 16);
        draw_step_division(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_step_division(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "sDiv", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, step_division, encoder_colour[n], true, false);
}
byte Track::get_step_division()
{
    return step_division;
}
// midi channel out
void Track::set_MIDI_channel_out(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        MIDI_channel_out = constrain(MIDI_channel_out + encoded[n], 1, 16);
        draw_MIDI_channel_out(n, lastProw);
        Serial.println(MIDI_channel_out);
        enc_moved[n] = false;
    }
}
void Track::draw_MIDI_channel_out(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "MCh", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, MIDI_channel_out, encoder_colour[n], true, false);
}
byte Track::get_MIDI_channel_out()
{
    return MIDI_channel_out;
}
// sequence length
void Track::set_sequence_length(byte n, byte lastProw)
{

    if (enc_moved[n])
    {
        sequence_length = constrain(sequence_length + encoded[n], 2, MAX_TICKS);
        draw_sequence_length(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_sequence_length(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "seqL", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, sequence_length, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
byte Track::get_sequence_length()
{
    return sequence_length;
}
// sequence length
void Track::set_step_length(byte n, byte lastProw)
{

    if (enc_moved[n])
    {
        step_length = constrain(step_length + encoded[n], 1, MAX_TICKS);
        draw_step_length(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_step_length(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "stpL", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, step_length, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
byte Track::get_step_length()
{
    return step_length;
}

// clip to edit
void Track::set_clip_to_edit(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        clip_to_edit = constrain(clip_to_edit + encoded[n], 0, NUM_USER_CLIPS);
        draw_clip_to_edit(n, lastProw);
        clear_notes_in_grid();
        draw_notes_in_grid();
        enc_moved[n] = false;
    }
}
void Track::draw_clip_to_edit(byte n, byte lastProw)
{
    for (int ClipNr = 0; ClipNr < 8; ClipNr++)
    {
        tft->drawRect(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2, STEP_FRAME_H * 13 + 1, STEP_FRAME_W * 2, STEP_FRAME_H - 1, ILI9341_DARKGREY);
    }
    tft->drawRect(STEP_FRAME_W * 2 * clip_to_edit + STEP_FRAME_W * 2, STEP_FRAME_H * 13 + 1, STEP_FRAME_W * 2, STEP_FRAME_H - 1, encoder_colour[n]);
}
byte Track::get_clip_to_edit()
{
    return clip_to_edit;
}
// sequencer Mode
void Track::set_velocity(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        byte velo = constrain(velocity[clip_to_edit][tick_start][active_voice] + encoded[n], 0, 127);
        for (int i = 0; i < step_length; i++)
        {
            velocity[clip_to_edit][tick_start + i][active_voice] = velo;
            Serial.printf("set velocity: %d tick: %d for note: %d\n", velocity[clip_to_edit][tick_start + i][active_voice], tick_start + i, note2set);
            draw_note_on_tick(tick_start + i);
            draw_velocity(n, lastProw);
            enc_moved[n] = false;
        }
    }
}
void Track::draw_velocity(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Velo", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, velocity[clip_to_edit][tick_start][active_voice], encoder_colour[n], true, false);

    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "sMod", sequencer_mode, n,0);
}
byte Track::get_velocity()
{
    return sequencer_mode;
}

// helpers
void Track::draw_sequencer_screen(byte lastProw)
{
    if (lastProw == 0)
    {
        drawOctaveNumber();
        draw_velocity(3, 0);
    }
    if (lastProw == 1)
    {
        draw_sequence_length(ENCODER_SEQUENCE_LENGTH, 1);
        draw_step_division(ENCODER_STEP_DIVISION, 1);
        draw_step_length(ENCODER_STEP_LENGTH, 1);
        draw_clip_to_edit(ENCODER_CLIP2_EDIT, 1);
    }
    if (lastProw == 2)
    {
        draw_sequencer_mode(ENCODER_SEQ_MODE, 2);
        draw_MIDI_channel_out(ENCODER_MIDICH_OUT, 1);
    }
}
// sequencer note input stuff
void Track::set_note_on_tick()
{
    for (int i = 0; i < step_length; i++)
    {
        tick = tick_start + i;
        check_for_free_voices(tick, note2set);
        array[clip_to_edit][tick][active_voice] = note2set;
        // Serial.printf("Track: %d , set Note: %d  to tick: %d with Voice : %d and velocity: %d \n", active_track, array[clip_to_edit][tick][active_voice], tick, active_voice, velocity[clip_to_edit][tick][active_voice]);
        clear_notes_on_tick(tick);
        draw_note_on_tick(tick);
    }
}
void Track::check_for_free_voices(byte onTick, byte cnote)
{
    for (int i = 0; i < MAX_VOICES; i++)
    {
        if (array[clip_to_edit][onTick][i] == cnote)
        {
            array[clip_to_edit][onTick][i] = NO_NOTE;
            note2set = NO_NOTE;
            search_free_voice = i;
        }
    }
    if (array[clip_to_edit][onTick][search_free_voice] < NO_NOTE)
        search_free_voice++;
    if (search_free_voice == MAX_VOICES)
        search_free_voice = 0;
    active_voice = search_free_voice;
}
void Track::clear_notes_on_tick(byte cl_X)
{
    int piX = (cl_X * 2) + 2 * STEP_FRAME_W;
    for (int i = 0; i < NOTES_PER_OCTAVE; i++)
    {
        for (int w = -4; w < 5; w++)
        {
            tft->drawPixel(piX, (STEP_FRAME_H * (i + 1)) + w + 8, ILI9341_DARKGREY);
            tft->drawPixel(piX + 1, (STEP_FRAME_H * (i + 1)) + w + 8, ILI9341_DARKGREY);
        }
    }
}
void Track::draw_note_on_tick(byte dr_X)
{
    int stepcolor = trackColor[active_track] + (clip_to_edit * 20);
    // int thisTick = (dr_X - SEQ_GRID_LEFT) / 2;
    for (int i = 0; i < MAX_VOICES; i++)
    {
        if (array[clip_to_edit][dr_X][i] >= octave * NOTES_PER_OCTAVE && array[clip_to_edit][dr_X][i] < (octave + 1) * NOTES_PER_OCTAVE)
        {

            byte PixelOn_Y = ((array[clip_to_edit][dr_X][i] - (octave * NOTES_PER_OCTAVE)) + 1) * STEP_FRAME_H;
            int minY = map(velocity[clip_to_edit][dr_X][i], 0, 127, 0, 4);
            int maxY = map(velocity[clip_to_edit][dr_X][i], 0, 127, 0, 5);
            Serial.printf("draw velocity: %d tick: %d for note: %d\n", velocity[clip_to_edit][dr_X][i], dr_X, array[clip_to_edit][dr_X][i]);
            // Serial.printf("tick: %d,  minY=%d, axY=%d\n", dr_X, minY, maxY);
            for (int w = -minY; w < maxY; w++)
            {
                tft->drawPixel((dr_X * 2) + 32, PixelOn_Y + w + 8, stepcolor);
                tft->drawPixel((dr_X * 2) + 32 + 1, PixelOn_Y + w + 8, stepcolor);
                // Serial.println(w);
            }
        }
    }
}
void Track::clear_notes_in_grid()
{
    for (int i = 0; i < MAX_TICKS; i++)
    {
        clear_notes_on_tick(i);
    }
}
void Track::draw_notes_in_grid()
{
    for (int i = 0; i < MAX_TICKS; i++)
    {
        draw_note_on_tick(i);
    }
}

// stepsequencer
void Track::drawStepSequencerStatic()
{
    clearWorkSpace();
    draw_Notenames();
    drawOctaveTriangle();
    draw_Clipselector();
    // draw the Main Grid
    for (int i = 0; i < 17; i++)
    { // vert Lines
        int step_Frame_X = i * 12;
        tft->drawFastVLine(step_Frame_X + STEP_FRAME_W * 2, STEP_FRAME_H, GRID_LENGTH_VERT, ILI9341_WHITE); //(x, y-start, length, color)
        if (i % 4 == 0)
        {
            tft->drawFastVLine((i * 12) + 32, STEP_FRAME_H, STEP_FRAME_H * 12, ILI9341_LIGHTGREY); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < 13; i++)
    { // hor lines
        int step_Frame_Y = i * 16;
        tft->drawFastHLine(STEP_FRAME_W * 2, step_Frame_Y + STEP_FRAME_H, NUM_STEPS * 12, ILI9341_WHITE); //(x-start, y, length, color)
    }
    // tft->asyncUpdateActive();
}
void Track::draw_Notenames()
{
    for (int n = 0; n < MAX_VOICES; n++)
    { // hor notes
        tft->fillRect(STEP_FRAME_W, STEP_FRAME_H * n + STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H, trackColor[my_Arranger_Y_axis - 1]);
        tft->setCursor(20, STEP_FRAME_H * n + 20);
        tft->setFont(Arial_8);
        tft->setTextColor(ILI9341_BLACK);
        tft->setTextSize(1);
        tft->print(noteNames[n]);
    }
}
void Track::drawOctaveTriangle()
{
    // draw Octavebuttons
    int leftmost = STEP_FRAME_W * OCTAVE_CHANGE_LEFTMOST;
    int rightmost = STEP_FRAME_W * OCTAVE_CHANGE_RIGHTMOST;
    int UP_topmost = STEP_FRAME_H * OCTAVE_CHANGE_UP_TOPMOST;
    int UP_bottommost = STEP_FRAME_H * OCTAVE_CHANGE_UP_BOTTOMMOST;
    int DOWN_topmost = STEP_FRAME_H * OCTAVE_CHANGE_DOWN_TOPMOST;
    int DOWN_bottommost = STEP_FRAME_H * OCTAVE_CHANGE_DOWN_BOTTOMMOST;
    tft->fillRect(leftmost + 1, STEP_FRAME_H * 2, STEP_FRAME_W * 2, STEP_FRAME_H * 3, ILI9341_DARKGREY);
    tft->fillTriangle(leftmost + 1, UP_bottommost, rightmost, UP_bottommost, leftmost + STEP_FRAME_W, UP_topmost, ILI9341_LIGHTGREY);        // octave arrow up
    tft->fillTriangle(leftmost + 1, DOWN_topmost, rightmost - 2, DOWN_topmost, leftmost + STEP_FRAME_W, DOWN_bottommost, ILI9341_LIGHTGREY); // x1, y1, x2, y2, x3, y3
}
void Track::draw_Clipselector()
{
    for (int ClipNr = 0; ClipNr < 8; ClipNr++)
    {
        tft->fillRect(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 1, STEP_FRAME_H * 13 + 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, trackColor[active_track] + (ClipNr * 20));
        tft->setCursor(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 4, STEP_FRAME_H * 13 + 4);
        tft->setFont(Arial_8);
        tft->setTextColor(ILI9341_BLACK);
        tft->setTextSize(1);
        tft->print("Clip ");
        tft->print(ClipNr);
    }
}

//----------------------------------------------------------------
// arranger stuff
// clip to play
void Track::set_clip_to_play(byte n, byte b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        byte when = ((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * (arrangerpage));
        if (enc_moved[n])
        {
            clip_to_play[when] = constrain(clip_to_play[when] + encoded[n], 0, NUM_USER_CLIPS + 1);
            draw_clip_to_play(n, when);
            enc_moved[n] = false;
        }
    }
}
void Track::draw_clip_to_play(byte n, byte b)
{
    draw_Value(n, 0, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, -3, clip_to_play[b], encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "clNr", clip_to_play[b], n, 0);
    draw_arrangment_line(n, b);
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
            enc_moved[n] = false;
        }
    }
}
void Track::draw_noteOffset(byte n, int b)
{
    draw_Value(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, -3, noteOffset[b], encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "ofSet", noteOffset[b], n, 0);
    draw_arrangment_line(n, b);
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

Track track1(&tft, 1);
Track track2(&tft, 2);
Track track3(&tft, 3);
Track track4(&tft, 4);

Track track5(&tft, 5);
Track track6(&tft, 6);
Track track7(&tft, 7);
Track track8(&tft, 8);

Track *allTracks[8]{&track1, &track2, &track3, &track4, &track5, &track6, &track7, &track8};
