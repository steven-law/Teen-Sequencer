#ifndef TRACK_H
#define TRACK_H

#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <MIDI.h>
#include "project_variables.h"
#include "project_functions.h"
// void draw_sequencer_option(byte x, const char *nameshort, int value, byte enc, const char *pluginName);
#define SEQ_GRID_LEFT 30
#define SEQ_GRID_RIGHT (18 * STEP_FRAME_W) - 2
#define SEQ_GRID_TOP 1
#define SEQ_GRID_BOTTOM 12
#define SEQUENCER_OPTIONS_VERY_RIGHT 18
#define SEQUENCER_OPTIONS_RIGHT 16
#define OCTAVE_CHANGE_LEFTMOST 18
#define OCTAVE_CHANGE_RIGHTMOST 20
#define OCTAVE_CHANGE_UP_TOPMOST 2
#define OCTAVE_CHANGE_UP_BOTTOMMOST 3
#define OCTAVE_CHANGE_DOWN_TOPMOST 4
#define OCTAVE_CHANGE_DOWN_BOTTOMMOST 5

#define OCTAVE_CHANGE_TEXT 3
#define NOTES_PER_OCTAVE 12
#define MIDI_CC_RANGE 127
#define BARS_PER_PAGE 16
#define NO_NOTE 128

#define MAX_CLIPS 9
#define NUM_USER_CLIPS 7
#define NUM_PRESETS 8
#define MAX_VOICES 12

#define ENCODER_STEP_LENGTH 2
#define ENCODER_SEQUENCE_LENGTH 0
#define ENCODER_STEP_DIVISION 1
#define ENCODER_SEQ_MODE 0
#define ENCODER_MIDICH_OUT 1

#define ENCODER_CLIP2_EDIT 3

extern int trackColor[9];
// Encoder Pins
extern bool enc_moved[4];
extern int encoded[4];
extern int encoder_colour[4];
extern bool change_plugin_row;
extern byte encoder_function;
extern int pixelTouchX;
extern int gridTouchY;
extern byte active_track;
extern byte arrangerpage;
void sendNoteOn(byte Note, byte Velo, byte Channel);
void sendNoteOff(byte Note, byte Velo, byte Channel);
void sendControlChange(byte control, byte value, byte channel);
// extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI1;
class Track
{
public:
    void play_SeqMode0(byte cloock);
    void play_SeqMode1(byte cloock);
    void set_SeqMode1_parameters(byte row);
    void set_SeqMode1_value(byte XPos, byte YPos, const char *name);
    void draw_SeqMode1();

    void play_SeqMode2(byte cloock);
    void set_SeqMode2_parameters(byte row);
    void set_SeqMode2_value(byte XPos, byte YPos, const char *name, int min, int max);
    void draw_SeqMode2();

    void play_SeqMode3(byte cloock);
    void set_SeqMode3_parameters(byte row);
    void set_SeqMode3_value(byte XPos, byte YPos, const char *name, int min, int max);
    void draw_SeqMode3();

    void play_SeqMode4(byte cloock);
    void set_SeqMode4_parameters(byte row);
    void set_SeqMode4_value(byte XPos, byte YPos, const char *name);
    void draw_SeqMode4();

    ILI9341_t3n *tft; // Pointer to the display object

    byte MIDI_channel_in;
    byte MIDI_channel_out;
    byte octave = 4;
    byte my_Arranger_Y_axis;
    byte sequencer_mode = 0;
    byte note2set;
    byte tick_start;
    byte noteToPlay[MAX_VOICES];
    int pixelOn_X;
    int pixelOn_Y;
    byte clip_to_edit = 0;
    byte clip_to_play[255];
    int noteOffset[255];
    byte tick;
    byte internal_clock = 0;
    bool internal_clock_is_on = false;
    byte internal_clock_bar = 0;
    byte step_division = 1;
    byte sequence_length = MAX_TICKS;
    byte step_length = 6;
    byte array[MAX_CLIPS][MAX_TICKS + 1][MAX_VOICES];
    byte velocity[MAX_CLIPS][MAX_TICKS + 1][MAX_VOICES];
    byte active_voice = 0;
    byte search_free_voice = 0;
    bool note_is_on[MAX_VOICES] = {true, true, true, true, true, true, true, true, true, true, true, true};
    bool ready_for_NoteOff[MAX_VOICES] = {false, false, false, false, false, false, false, false, false, false, false, false};
    byte CCvalue[NUM_PRESETS + 1][16];
    byte CCchannel[NUM_PRESETS + 1][16];
    byte edit_presetNr_ccChannel = 0;
    byte edit_presetNr_ccValue = 0;
    byte play_presetNr_ccChannel = 0;
    byte play_presetNr_ccValue = 0;
    const char *CCnames[128]{"CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC",
                             "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC", "CC"};

    const char *noteNames[12]{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    const char *seqModname[5]{"Step", "Rand", "Drop", "BitRd", "PotS"};
    byte SeqMod_1_Poti[4]; // oct- ; oct+ ; vol- ; vol+
    byte SeqMod_2_Poti[16];
    byte SeqMod_3_Poti[12];
    byte SeqMod_4_Poti[16];
    byte maxVal;

    byte mute_norm_solo_pot = 1;
    bool muted;
    bool soloed;

    Track(ILI9341_t3n *display, byte Y)
    {
        // MIDI1.setHandleNoteOn(myNoteOn);
        // MIDI1.setHandleNoteOff(myNoteOff);
        // MIDI1.begin();
        tft = display;
        my_Arranger_Y_axis = Y;
        MIDI_channel_in = Y;
        MIDI_channel_out = Y;
        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t < MAX_TICKS + 1; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    array[c][t][v] = NO_NOTE;
                    velocity[c][t][v] = 99;
                }
            }
        }
        for (int i = 0; i < 255; i++)
        {
            clip_to_play[i] = 8;
            noteOffset[i] = 0;
        }
    }

    // update
    void update(int PixelX, byte gridY);

    // sequencer Modes
    void play_sequencer_mode(byte cloock, byte start, byte end);
    void set_SeqMode_parameters(byte row);
    void draw_sequencer_modes(byte mode);
    void noteOn(byte Note, byte Velo, byte Channel);
    void noteOff(byte Note, byte Velo, byte Channel);
    // void play_SeqMode0(byte cloock);

    // sequencer options:
    // octave
    void set_octave(byte n);
    void drawOctaveNumber();
    byte get_octave();
    // sequencer Mode
    void set_sequencer_mode(byte n, byte lastProw);
    void draw_sequencer_mode(byte n, byte lastProw);
    byte get_sequencer_mode();
    // step division
    void set_step_division(byte n, byte lastProw);
    void draw_step_division(byte n, byte lastProw);
    byte get_step_division();
    // midi channel out
    void set_MIDI_channel_out(byte n, byte lastProw);
    void draw_MIDI_channel_out(byte n, byte lastProw);
    byte get_MIDI_channel_out();
    // sequence length
    void set_sequence_length(byte n, byte lastProw);
    void draw_sequence_length(byte n, byte lastProw);
    byte get_sequence_length();
    // step length
    void set_step_length(byte n, byte lastProw);
    void draw_step_length(byte n, byte lastProw);
    byte get_step_length();
    // clip to edit
    void set_clip_to_edit(byte n, byte lastProw);
    void draw_clip_to_edit(byte n, byte lastProw);
    byte get_clip_to_edit();
    // velocity
    void set_velocity(byte n, byte lastProw);
    void draw_velocity(byte n, byte lastProw);
    byte get_velocity();

    void set_CCvalue(byte XPos, byte YPos);
    void set_CCchannel(byte XPos, byte YPos);
    void draw_MIDI_CC(byte XPos, byte YPos);
    void set_MIDI_CC(byte row);
    void draw_MIDI_CC_screen();
    void set_edit_presetNr_ccChannel(byte n, byte lastProw);
    void draw_edit_presetNr_ccChannel(byte n, byte lastProw);
    void set_edit_presetNr_ccValue(byte n, byte lastProw);
    void draw_edit_presetNr_ccValue(byte n, byte lastProw);
    // coordinates
    void set_coordinateX(byte n, byte lastProw);
    void set_coordinateY(byte n, byte lastProw);
    void draw_coordinateX(byte n, byte lastProw);
    void draw_coordinateY(byte n, byte lastProw);
    // helpers

    void draw_sequencer_screen(byte lastProw);
    // sequencer note input stuff
    void set_note_on_tick();
    void check_for_free_voices(byte onTick, byte cnote);
    void clear_notes_on_tick(byte cl_X);
    void draw_note_on_tick(byte dr_X);
    void clear_notes_in_grid();
    void draw_notes_in_grid();

    // stepsequencer
    void drawStepSequencerStatic();
    void draw_Notenames();
    void drawOctaveTriangle();
    void draw_Clipselector();

    //----------------------------------------------------------------
    // arranger stuff
    void set_arranger_parameters(byte lastProw);
    void draw_arranger_parameters(byte lastProw);
    // clip to play
    void set_clip_to_play(byte n, byte b);
    void draw_clip_to_play(byte n, byte b);
    void drawsongmodepageselector();
    void gridSongMode(int songpageNumber);

    void draw_arrangment_lines(byte n, byte b);
    void draw_arrangment_line(byte n, byte b);
    void draw_clipNr_arranger(byte n, byte b);
    byte get_clip_to_play(byte when);
    // note offset / note transpose
    void set_note_offset(byte n, int b);
    void draw_noteOffset(byte n, int b);
    void draw_offset_arranger(byte n, byte b);
    // MIDI CC
    void set_play_presetNr_ccChannel(byte n, byte lastProw);
    void draw_play_presetNr_ccChannel(byte n, byte lastProw);
    void set_play_presetNr_ccValue(byte n, byte lastProw);
    void draw_play_presetNr_ccValue(byte n, byte lastProw);
};

extern Track *allTracks[8];
#endif