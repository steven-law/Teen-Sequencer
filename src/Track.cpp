#include <Arduino.h>
#include "Track.h"

// MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
//  midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midi_out_serial = &MIDI1;
void Track::update(int PixelX, byte gridY)
{
    // MIDI1.read();
    if (PixelX > 2 * STEP_FRAME_W && PixelX < 14 * STEP_FRAME_W && gridY >= 1 && gridY < 13)
    {
        note2set = (gridY - SEQ_GRID_TOP) + (octave * NOTES_PER_OCTAVE);
        tick_start = (PixelX - SEQ_GRID_LEFT) / 2;
        pixelOn_X = PixelX;
        pixelOn_Y = gridY;
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
            if (sequencer_mode == 4)
            {
                play_SeqMode4(internal_clock);
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
    if (sequencer_mode == 4)
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
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, seqModname[sequencer_mode], encoder_colour[n], true, false);

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
// step length
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
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Clip", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, clip_to_edit, encoder_colour[n], true, false);

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
// step velocity
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
// MIDI CC
void Track::set_CCchannel(byte XPos, byte YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[n])
    {
        CCchannel[edit_presetNr_ccChannel][n] = constrain(CCchannel[edit_presetNr_ccChannel][n] + encoded[n], 1, 128);
        draw_MIDI_CC(XPos, YPos);
        enc_moved[n] = false;
    }
}
void Track::set_CCvalue(byte XPos, byte YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[n])
    {

        CCvalue[edit_presetNr_ccValue][n] = constrain(CCvalue[edit_presetNr_ccValue][n] + encoded[n], 0, 127);
        draw_MIDI_CC(XPos, YPos);
        sendControlChange(CCchannel[edit_presetNr_ccChannel][n], CCvalue[edit_presetNr_ccValue][n], MIDI_channel_out);
        // usbMIDI.sendControlChange(CCchannel[n], CCvalue[n], MIDI_channel_out);
        enc_moved[n] = false;
    }
}
void Track::draw_MIDI_CC(byte XPos, byte YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    char _CCName[6];
    sprintf(_CCName, "CC%d\0", CCchannel[edit_presetNr_ccChannel][n]);
    drawPot(XPos, YPos, CCvalue[edit_presetNr_ccValue][n], _CCName);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
void Track::set_MIDI_CC(byte row)
{
    draw_MIDI_CC_screen();
    if (!buttonPressed[BUTTON_SHIFT] && !buttonPressed[BUTTON_ENTER])
    {
        if (row == 0)
        {
            set_CCvalue(0, 0);
            set_CCvalue(1, 0);
            set_CCvalue(2, 0);
            set_CCvalue(3, 0);
        }
        if (row == 1)
        {
            set_CCvalue(0, 1);
            set_CCvalue(1, 1);
            set_CCvalue(2, 1);
            set_CCvalue(3, 1);
        }
        if (row == 2)
        {
            set_CCvalue(0, 2);
            set_CCvalue(1, 2);
            set_CCvalue(2, 2);
            set_CCvalue(3, 2);
        }
        if (row == 3)
        {
            set_CCvalue(0, 3);
            set_CCvalue(1, 3);
            set_CCvalue(2, 3);
            set_CCvalue(3, 3);
        }
    }
    if (buttonPressed[BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_CCchannel(0, 0);
            set_CCchannel(1, 0);
            set_CCchannel(2, 0);
            set_CCchannel(3, 0);
        }
        if (row == 1)
        {
            set_CCchannel(0, 1);
            set_CCchannel(1, 1);
            set_CCchannel(2, 1);
            set_CCchannel(3, 1);
        }
        if (row == 2)
        {
            set_CCchannel(0, 2);
            set_CCchannel(1, 2);
            set_CCchannel(2, 2);
            set_CCchannel(3, 2);
        }
        if (row == 3)
        {
            set_CCchannel(0, 3);
            set_CCchannel(1, 3);
            set_CCchannel(2, 3);
            set_CCchannel(3, 3);
        }
    }
    if (buttonPressed[BUTTON_ENTER])
    {
        set_edit_presetNr_ccChannel(2, 0);
        set_edit_presetNr_ccValue(3, 0);
    }
}
void Track::draw_MIDI_CC_screen()
{

    if (change_plugin_row)
    {
        clearWorkSpace();
        change_plugin_row = false;
        draw_MIDI_CC(0, 0);
        draw_MIDI_CC(1, 0);
        draw_MIDI_CC(2, 0);
        draw_MIDI_CC(3, 0);

        draw_MIDI_CC(0, 1);
        draw_MIDI_CC(1, 1);
        draw_MIDI_CC(2, 1);
        draw_MIDI_CC(3, 1);

        draw_MIDI_CC(0, 2);
        draw_MIDI_CC(1, 2);
        draw_MIDI_CC(2, 2);
        draw_MIDI_CC(3, 2);

        draw_MIDI_CC(0, 3);
        draw_MIDI_CC(1, 3);
        draw_MIDI_CC(2, 3);
        draw_MIDI_CC(3, 3);

        draw_edit_presetNr_ccChannel(2, 0);
        draw_edit_presetNr_ccValue(3, 0);
    }
}
void Track::set_edit_presetNr_ccChannel(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        edit_presetNr_ccChannel = constrain(edit_presetNr_ccChannel + encoded[n], 0, NUM_PRESETS - 1);

        change_plugin_row = true;
        draw_MIDI_CC_screen();
        draw_edit_presetNr_ccChannel(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_edit_presetNr_ccChannel(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 0, 4, "cc-Set", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, edit_presetNr_ccChannel, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
void Track::set_edit_presetNr_ccValue(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        edit_presetNr_ccValue = constrain(edit_presetNr_ccValue + encoded[n], 0, NUM_PRESETS - 1);

        change_plugin_row = true;
        draw_MIDI_CC_screen();
        draw_edit_presetNr_ccValue(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_edit_presetNr_ccValue(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 0, 4, "vl-Set", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, edit_presetNr_ccValue, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
// coordinates
void Track::set_coordinateX(byte n, byte lastProw)
{

    if (enc_moved[n])
    {
        draw_coordinateX(0, lastProw);
        draw_coordinateY(1, lastProw);
        tft->updateScreenAsync();
        enc_moved[n] = false;
    }
}
void Track::set_coordinateY(byte n, byte lastProw)
{

    if (enc_moved[n])
    {
        draw_coordinateX(0, lastProw);
        draw_coordinateY(1, lastProw);
tft->updateScreenAsync();
        enc_moved[n] = false;
    }
}
void Track::draw_coordinateX(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Tick", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, tick_start, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
void Track::draw_coordinateY(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Note", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, note2set, encoder_colour[n], true, false);
   tft->updateScreenAsync(); // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
// helpers
void Track::draw_sequencer_screen(byte lastProw)
{
    if (lastProw == 0)
    {
        draw_coordinateX(0, 0);
        draw_coordinateY(1, 0);
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
        draw_MIDI_channel_out(ENCODER_MIDICH_OUT, 2);
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
void Track::set_arranger_parameters(byte lastProw){}
void Track::draw_arranger_parameters(byte lastProw)
{
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
            draw_arrangment_line(n, when);
            enc_moved[n] = false;
        }
    }
}
void Track::draw_clip_to_play(byte n, byte b)
{
    draw_Text(n, 0, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, -3, "Clip", encoder_colour[n], true, false);
    draw_Value(n, 0, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, -3, clip_to_play[b], encoder_colour[n], true, false);
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
    draw_Text(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, -3, "Trns", encoder_colour[n], true, false);
    draw_Value(n, 1, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, -3, noteOffset[b], encoder_colour[n], true, false);
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
        play_presetNr_ccChannel = constrain(play_presetNr_ccChannel + encoded[n], 0, NUM_PRESETS);

        change_plugin_row = true;
        // draw_MIDI_CC_screen();
        draw_play_presetNr_ccChannel(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_play_presetNr_ccChannel(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 0, 4, "cc-Set", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, play_presetNr_ccChannel, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
}
void Track::set_play_presetNr_ccValue(byte n, byte lastProw)
{
    if (enc_moved[n])
    {
        play_presetNr_ccValue = constrain(play_presetNr_ccValue + encoded[n], 0, NUM_PRESETS);

        change_plugin_row = true;
        // draw_MIDI_CC_screen();
        draw_play_presetNr_ccValue(n, lastProw);
        enc_moved[n] = false;
    }
}
void Track::draw_play_presetNr_ccValue(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 0, 4, "vl-Set", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, play_presetNr_ccValue, encoder_colour[n], true, false);
    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "tiks", sequence_length, n, 0);
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
