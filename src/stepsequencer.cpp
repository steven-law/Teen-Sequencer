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
    // change_plugin_row=true;
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
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT] && !neotrellisPressed[TRELLIS_BUTTON_ENTER])
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
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
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
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
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
        note2set = (y - SEQ_GRID_TOP) + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        this->check_for_free_voices(sTick, note2set);
    }
}
void Track::check_for_free_voices(byte onTick, byte newNote)
{
    Serial.printf("newNote: %d onTick: %d\n", newNote, onTick);
    search_free_voice = pixelOn_X;

    // löschen der Note

    if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice) == newNote)
    {

        set_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, NO_NOTE);
        set_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, 0);
        set_active_stepFX(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, 0);
    }
    // setzen neuer Note
    else if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice) == NO_NOTE)
    {

        set_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, newNote);
        set_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_VELO2SET]);
        set_active_stepFX(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_STEP_FX]);
    }

    int trellisColor;
    int tftColor;
    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, v) == NO_NOTE)
        {
            trellisColor = TRELLIS_BLACK;
            tftColor = ILI9341_DARKGREY;
            break;
        }
    }
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, v) < NO_NOTE)
        {
            trellisColor = trellisTrackColor[MIDI_channel_in - 1];
            tftColor = trackColor[my_Arranger_Y_axis - 1] + (this->parameter[SET_CLIP2_EDIT] * 20);
            break;
        }
    }
    byte note = get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice);
    byte velo = get_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice);

    if (note >= parameter[SET_OCTAVE] * NOTES_PER_OCTAVE && note < (parameter[SET_OCTAVE] + 1) * NOTES_PER_OCTAVE)
    {
        if (active_track == my_Arranger_Y_axis - 1)
            void tftClass::draw_note_on_tick(note, onTick, this->parameter[SET_CLIP2_EDIT], velo, tftColor);
    }
    trellis_set_main_buffer(this->parameter[SET_CLIP2_EDIT], (onTick / 6), (MIDI_channel_in - 1), trellisColor);
}



// stepsequencer

void Track::set_recordState(bool _status)
{
    int circleColor;
    this->recordState = _status;
}
bool Track::get_recordState()
{
    return recordState;
}
