#ifndef TRACK_H
#define TRACK_H

#include <Arduino.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <MIDI.h>
#include <SD.h>
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

#define ENCODER_OCTAVE 2
#define ENCODER_STEP_LENGTH 2
#define ENCODER_SEQUENCE_LENGTH 0
#define ENCODER_STEP_DIVISION 1
#define ENCODER_SEQ_MODE 0
#define ENCODER_MIDICH_OUT 1
#define ENCODER_CLIP2_EDIT 3
//
#define NUM_PARAMETERS 16
#define SET_OCTAVE 2
#define SET_VELO2SET 3
#define SET_SEQUENCE_LENGTH 4
#define SET_STEP_DIVIVISION 5
#define SET_STEP_LENGTH 6
#define SET_CLIP2_EDIT 7
#define SET_SEQ_MODE 8
#define SET_MIDICH_OUT 9

extern File myFile;
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
    byte parameter[16]{0, 0, 4, 99, 96, 1, 3, 0, 0, 0, 0, 0};
    byte my_Arranger_Y_axis;
    byte note2set;
    byte tick_start;
    byte noteToPlay[MAX_VOICES];
    int pixelOn_X;
    int pixelOn_Y;
    int gridX_4_save;
    byte gridY_4_save;
    byte clip_to_play[256];
    int noteOffset[256];
    byte tick;
    byte internal_clock = 0;
    bool internal_clock_is_on = false;
    byte internal_clock_bar = 0;
    byte sequence_length = MAX_TICKS;
    byte array[MAX_CLIPS][MAX_TICKS +1][MAX_VOICES];
    byte velocity[MAX_CLIPS][MAX_TICKS +1][MAX_VOICES];
    byte search_free_voice = 0;
    bool note_is_on[MAX_VOICES] = {true, true, true, true, true, true, true, true, true, true, true, true};
    bool ready_for_NoteOff[MAX_VOICES] = {false, false, false, false, false, false, false, false, false, false, false, false};
    byte CCvalue[NUM_PRESETS + 1][16];
    byte CCchannel[NUM_PRESETS + 1][16];
    byte edit_presetNr_ccChannel = 0;
    byte edit_presetNr_ccValue = 0;
    byte play_presetNr_ccChannel[256];
    byte play_presetNr_ccValue[256];
    byte bar_to_edit = 0;
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
        //SD.begin(BUILTIN_SDCARD);
        tft = display;
        my_Arranger_Y_axis = Y;
        MIDI_channel_in = Y;
        parameter[SET_MIDICH_OUT] = Y;
        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t <= MAX_TICKS ; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    this->array[c][t][v] = NO_NOTE;
                    this->velocity[c][t][v] = 99;
                }
            }
        }
        for (int i = 0; i < 256; i++)
        {
            clip_to_play[i] = 8;
            noteOffset[i] = 0;
            play_presetNr_ccChannel[i] = 8;
            play_presetNr_ccValue[i] = 8;
        }
        for (int p = 0; p < NUM_PRESETS; p++)
        {
            for (int t = 0; t < 16 + 1; t++)
            {
                CCchannel[p][t] = 128;
                CCvalue[p][t] = 0;
            }
        }
    }

    // update
    void update(int PixelX, byte gridY);
    void saveTrack();
    void loadTrack();
    // sequencer Modes
    void play_sequencer_mode(byte cloock, byte start, byte end);
    void set_SeqMode_parameters(byte row);
    void draw_sequencer_modes(byte mode);
    void noteOn(byte Note, byte Velo, byte Channel);
    void noteOff(byte Note, byte Velo, byte Channel);
    // void play_SeqMode0(byte cloock);
    void set_stepSequencer_parameters(byte row);
    void draw_stepSequencer_parameters(byte row);

    void set_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name, byte min, byte max);
    void draw_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name);

    void set_stepSequencer_parameter_text(byte XPos, byte YPos, const char *name, const char *text, byte min, byte max);
    void draw_stepSequencer_parameter_text(byte XPos, byte YPos, const char *text, const char *name);
    // sequencer options:
    // octave
    void set_octave(byte n);
    void drawOctaveNumber();
    byte get_octave();

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
    void change_presets();
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