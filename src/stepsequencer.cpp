#include <Arduino.h>
#include "project_variables.h"
#include "Track.h"

void Track::set_stepSequencer_parameters(byte row)
{

    switch (row)
    {
    case 0:
        encoder_SetCursor(parameter[SET_STEP_LENGTH] * 2, 14); // Encoder: 0+1
        set_coordinateX(0, 0);
        set_coordinateY(1, 0);

        set_stepSequencer_parameter_value(ENCODER_STEP_FX, 0, CCnames[setStepFX], 0, 129);
        set_stepSequencer_parameter_value(3, 0, "Velo", 1, 128);
        break;
    case 1:
        set_stepSequencer_parameter_value(ENCODER_SEQUENCE_LENGTH, 1, "seqL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_STEP_DIVISION, 1, "sDiv", 1, 16);
        set_stepSequencer_parameter_value(ENCODER_STEP_LENGTH, 1, "stpL", 1, 96);
        set_stepSequencer_parameter_value(ENCODER_OCTAVE, 1, "Oct", 0, 11);

        break;
    case 2:
        set_stepSequencer_parameter_text(ENCODER_SEQ_MODE, 2, "sMod", seqModname[parameter[SET_SEQ_MODE]], 0, 4);
        set_stepSequencer_parameter_text(ENCODER_MIDICH_OUT, 2, "MCh", channelOutNames[parameter[SET_MIDICH_OUT]], 0, MAX_OUTPUTS);
        set_stepSequencer_parameter_value(ENCODER_CLIP2_EDIT, 2, "Clip", 0, NUM_USER_CLIPS);

        break;
    case 3:
    default:
        break;
    }
    draw_stepSequencer_parameters(row);
}
void Track::draw_stepSequencer_parameters(byte lastProw)
{
    if (change_plugin_row)
    {
        tft->fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        if (lastProw == 0)
        {
            draw_coordinateX(0, 0);
            draw_coordinateY(1, 0);
            draw_stepSequencer_parameter_value(ENCODER_STEP_FX, 0, CCnames[setStepFX]);
            draw_stepSequencer_parameter_value(3, 0, "Velo");
        }
        if (lastProw == 1)
        {
            draw_stepSequencer_parameter_value(ENCODER_SEQUENCE_LENGTH, 1, "seqL");
            draw_stepSequencer_parameter_value(ENCODER_STEP_DIVISION, 1, "sDiv");
            draw_stepSequencer_parameter_value(ENCODER_STEP_LENGTH, 1, "stpL");
            draw_stepSequencer_parameter_value(ENCODER_OCTAVE, 1, "Oct");
        }
        if (lastProw == 2)
        {
            draw_stepSequencer_parameter_text(ENCODER_SEQ_MODE, 2, seqModname[parameter[8]], "sMod");
            draw_stepSequencer_parameter_text(ENCODER_MIDICH_OUT, 2, channelOutNames[parameter[SET_MIDICH_OUT]], "MCh");

            draw_stepSequencer_parameter_value(ENCODER_CLIP2_EDIT, 2, "Clip");
        }
    }
}

void Track::set_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name, byte min, byte max)
{

    if (enc_moved[XPos])
    {
        byte index = XPos + (YPos * NUM_ENCODERS);
        enc_moved[XPos] = false;
        parameter[index] = constrain(parameter[index] + encoded[XPos], min, max);
        Serial.printf("parameter: %d, value: %d, name %s\n", index, parameter[index], name);
        if (index == SET_OCTAVE || index == SET_CLIP2_EDIT)
        {
            clear_notes_in_grid();
            draw_notes_in_grid();
        }
        draw_stepSequencer_parameter_value(XPos, YPos, name);
    }
}
void Track::draw_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name)
{
    byte index = XPos + (YPos * NUM_ENCODERS);
    draw_Text(XPos, YPos, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 5, 0, 4, name, encoder_colour[XPos], false, false);
    draw_Value(XPos, YPos, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 6, 4, 4, parameter[index], encoder_colour[XPos], true, false);
}

void Track::set_stepSequencer_parameter_text(byte XPos, byte YPos, const char *name, const char *text, byte min, byte max)
{
    if (enc_moved[XPos])
    {
        byte index = XPos + (YPos * NUM_ENCODERS);
        enc_moved[XPos] = false;
        parameter[index] = constrain(parameter[index] + encoded[XPos], min, max);
        Serial.printf("parameter: %d, value: %d, name %s, text %s\n", index, parameter[index], name, text);
        draw_stepSequencer_parameter_text(XPos, YPos, text, name);
    }
}
void Track::draw_stepSequencer_parameter_text(byte XPos, byte YPos, const char *text, const char *name)

{
    byte index = XPos + (YPos * NUM_ENCODERS);
    Serial.printf("Drawing text at index %d, name %s, text %s\n", index, name, text);
    draw_Text(XPos, YPos, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 5, 0, 4, name, encoder_colour[XPos], false, false);
    draw_Text(XPos, YPos, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 6, 4, 4, text, encoder_colour[XPos], true, false);
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
        sendControlChange(CCchannel[edit_presetNr_ccChannel][n], CCvalue[edit_presetNr_ccValue][n], parameter[SET_MIDICH_OUT]);
        enc_moved[n] = false;
    }
}
void Track::draw_MIDI_CC(byte XPos, byte YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    drawPot(XPos, YPos, CCvalue[edit_presetNr_ccValue][n], CCnames[CCchannel[edit_presetNr_ccChannel][n]]);
}
void Track::set_MIDI_CC(byte row)
{
    draw_MIDI_CC_screen();
    if (!trellisPressed[TRELLIS_BUTTON_SHIFT] && !trellisPressed[TRELLIS_BUTTON_ENTER])
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
    if (trellisPressed[TRELLIS_BUTTON_SHIFT])
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
    if (trellisPressed[TRELLIS_BUTTON_ENTER])
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
}
// coordinates
void Track::set_coordinateX(byte n, byte lastProw)
{

    // if (enc_moved[n])
    if (millis() % 100 == 0)
    {
        draw_coordinateX(0, lastProw);
        draw_coordinateY(1, lastProw);

        //  enc_moved[n] = false;
    }
}
void Track::set_coordinateY(byte n, byte lastProw)
{

    if (millis() % 100 == 0)
    {
        draw_coordinateX(0, lastProw);
        draw_coordinateY(1, lastProw);

        //   enc_moved[n] = false;
    }
}
void Track::draw_coordinateX(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Tick", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, tickStart, encoder_colour[n], true, false);
}
void Track::draw_coordinateY(byte n, byte lastProw)
{
    draw_Text(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 5, 4, 4, "Note", encoder_colour[n], false, false);
    draw_Value(n, lastProw, SEQUENCER_OPTIONS_VERY_RIGHT, (n * 2) + 6, 4, 4, note2set, encoder_colour[n], true, false);
}
// helpers
// sequencer note input stuff
void Track::set_active_note(byte _clip, byte _tick, byte _voice, byte _note)
{
    this->clip[_clip].tick[_tick].voice[_voice] = _note;
}
byte Track::get_active_note(byte _clip, byte _tick, byte _voice)
{
    return this->clip[_clip].tick[_tick].voice[_voice];
}
void Track::set_active_velo(byte _clip, byte _tick, byte _voice, byte _velo)
{
    this->clip[_clip].tick[_tick].velo[_voice] = _velo;
}
byte Track::get_active_velo(byte _clip, byte _tick, byte _voice)
{
    return this->clip[_clip].tick[_tick].velo[_voice];
}
void Track::set_active_stepFX(byte _clip, byte _tick, byte _voice, byte _stepFX)
{
    this->clip[_clip].tick[_tick].stepFX = _stepFX;
}
byte Track::get_active_stepFX(byte _clip, byte _tick, byte _voice)
{
    return this->clip[_clip].tick[_tick].stepFX;
}
void Track::set_note_on_tick(int x, int y)
{
    for (int i = 0; i < parameter[SET_STEP_LENGTH]; i++)
    {
        sTick = x + i;
        pixelOn_X = (y - SEQ_GRID_TOP);
        note2set = (y - SEQ_GRID_TOP) + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        this->check_for_free_voices(sTick, note2set);
        if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
        {
            this->clear_notes_on_tick(sTick);
            this->draw_note_on_tick(sTick);
        }
    }
}
void Track::check_for_free_voices(byte onTick, byte newNote)
{
    Serial.printf("newNote: %d onTick: %d\n", newNote, onTick);
    search_free_voice = pixelOn_X;
    /*
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // Serial.printf("Voice: %d, Note: %d\n", i, array[parameter[SET_CLIP2_EDIT]][onTick][i]);
        if (array[parameter[SET_CLIP2_EDIT]][onTick][i] == newNote)
            Serial.printf("should clear:   old Note= %d on voice: %d, with new Note: %d on voice: %d\n", array[parameter[SET_CLIP2_EDIT]][onTick][i], i, newNote, search_free_voice);
        if (array[parameter[SET_CLIP2_EDIT]][onTick][i] == NO_NOTE)
            Serial.printf("should set new: old Note= %d on voice: %d, with new Note: %d on voice: %d\n", array[parameter[SET_CLIP2_EDIT]][onTick][i], i, newNote, search_free_voice);
        if (array[parameter[SET_CLIP2_EDIT]][onTick][i] != newNote && array[parameter[SET_CLIP2_EDIT]][onTick][i] < NO_NOTE)
            Serial.printf("should replace: old Note= %d on voice: %d, with new Note: %d on voice: %d\n", array[parameter[SET_CLIP2_EDIT]][onTick][i], i, newNote, search_free_voice);
    }
    */

    // for (int i = 0; i < MAX_VOICES; i++)
    {
        // löschen der Note

        if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice) == newNote)
        {

            set_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, NO_NOTE);
            set_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, 0);
            set_active_stepFX(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, 0);
            // search_free_voice = i;
            /*Serial.printf("Cleared note: %d at tick: %d, voice: %d with velocity: %d\n",
                          clip[parameter[SET_CLIP2_EDIT]].tick[onTick].voice[search_free_voice],
                          onTick,
                          search_free_voice,
                          clip[parameter[SET_CLIP2_EDIT]].tick[onTick].velo[search_free_voice]);
                          */
            // break;
        }
        // setzen neuer Note
        else if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice) == NO_NOTE)
        {

            // if (old_cnote != newNote)
            // search_free_voice++;
            set_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, newNote);
            set_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_VELO2SET]);
            set_active_stepFX(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_STEP_FX]);
            // search_free_voice = i;
            /*Serial.printf("Set new note: %d at tick: %d, voice: %d with velocity: %d\n",
                          clip[parameter[SET_CLIP2_EDIT]].tick[onTick].voice[search_free_voice],
                          onTick,
                          search_free_voice,
                          clip[parameter[SET_CLIP2_EDIT]].tick[onTick].velo[search_free_voice]);
                          */
            // break;
        }
        /*
                // ersetzen alter note
                else if (array[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice] != newNote && array[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice] < NO_NOTE)
                {

                    if (old_cnote != newNote)
                        search_free_voice++;
                    byte noteToReplace = array[parameter[SET_CLIP2_EDIT]][onTick][i];
                    array[parameter[SET_CLIP2_EDIT]][onTick][i] = NO_NOTE;
                    array[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice] = newNote;
                    velocity[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice] = parameter[SET_VELO2SET];

                    Serial.printf("Replaced old note: %d on voice: %d, with new note: %d  voice: %d with velocity: %d, at tick: %d\n",
                                  noteToReplace,
                                  i,
                                  array[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice],
                                  search_free_voice,
                                  velocity[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice],
                                  onTick);
                    break;
                }
                */
    }
    int color;
    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, v) == NO_NOTE)
        {
            color = TRELLIS_BLACK;
            break;
        }
    }
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, v) < NO_NOTE)
        {
            color = trellisTrackColor[MIDI_channel_in - 1];
            break;
        }
    }
    trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (sTick / 6), (MIDI_channel_in - 1), color);
    // trellis_show();
    //  if (search_free_voice >= MAX_VOICES)
    //  search_free_voice = 0;
    //  old_cnote = array[parameter[SET_CLIP2_EDIT]][onTick][search_free_voice];
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
    int stepcolor = trackColor[active_track] + (parameter[SET_CLIP2_EDIT] * 20);

    for (int i = 0; i < MAX_VOICES; i++)
    {
        byte note = this->clip[parameter[SET_CLIP2_EDIT]].tick[dr_X].voice[i];
        byte velo = this->clip[parameter[SET_CLIP2_EDIT]].tick[dr_X].velo[i];

        if (note >= parameter[SET_OCTAVE] * NOTES_PER_OCTAVE && note < (parameter[SET_OCTAVE] + 1) * NOTES_PER_OCTAVE)
        {
            byte PixelOn_Y = ((note - (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + 1) * STEP_FRAME_H;
            int minY = map(velo, 0, 127, 0, 4);
            int maxY = map(velo, 0, 127, 0, 5);

            // Serial.printf("draw velocity: %d tick: %d for note: %d on voice: %d\n", velo, dr_X, note, i);

            for (int w = -minY; w < maxY; w++)
            {
                tft->drawPixel((dr_X * 2) + 32, PixelOn_Y + w + 8, stepcolor);
                tft->drawPixel((dr_X * 2) + 32 + 1, PixelOn_Y + w + 8, stepcolor);
            }
        }
        else
        {
            // Serial.printf("Skipping note: %d at tick: %d, voice: %d\n", note, dr_X, i);
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
    //clear_notes_in_grid();
    for (int i = 0; i < MAX_TICKS; i++)
    {
        // clear_notes_on_tick(i);
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

void Track::set_recordState(bool _status)
{
    int circleColor;
    this->recordState = _status;
    if (recordState)
        circleColor = ILI9341_RED;
    else
        circleColor = ILI9341_LIGHTGREY;
    tft->fillCircle(STEP_FRAME_W * POSITION_RECORD_BUTTON + 7, 7, 6, circleColor);
}
bool Track::get_recordState()
{
    return recordState;
}
