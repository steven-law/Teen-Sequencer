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
// potrow 0
#define ENCODER_STEP_FX 2
// potrow 1
#define ENCODER_SEQUENCE_LENGTH 0
#define ENCODER_STEP_DIVISION 1
#define ENCODER_STEP_LENGTH 2
// potrow 2
#define ENCODER_OCTAVE 3
#define ENCODER_SEQ_MODE 0
#define ENCODER_MIDICH_OUT 1
#define ENCODER_CLIP2_EDIT 3

#define NUM_PARAMETERS 16
#define SET_STEP_FX 2
#define SET_VELO2SET 3
#define SET_SEQUENCE_LENGTH 4
#define SET_STEP_DIVIVISION 5
#define SET_STEP_LENGTH 6
#define SET_OCTAVE 7
#define SET_SEQ_MODE 8
#define SET_MIDICH_OUT 9

#define SET_CLIP2_EDIT 11

extern File myFile;
extern int trackColor[9];
// Encoder Pins
extern bool enc_moved[4];
extern int encoded[4];
extern int encoder_colour[4];
extern bool change_plugin_row;
extern byte activeScreen;
extern int pixelTouchX;
extern int gridTouchY;
extern byte active_track;
extern byte arrangerpage;
void sendNoteOn(byte Note, byte Velo, byte Channel);
void sendNoteOff(byte Note, byte Velo, byte Channel);
void sendControlChange(byte control, byte value, byte channel);
void trellis_show_clockbar(byte trackNr, byte step);
void trellis_set_stepSeq_buffer(int _x, int _y, int color);
void trellis_set_arranger_buffer(int _page, int _x, int _y, int color);
void trellis_show();
// extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI1;
class Track
{

public:
    // Stepsequencer
    bool recordState = false;
    byte parameter[16]{0, 0, 128, 99, 96, 1, 3, 4, 0, 0, 0, 0};
     byte mixGainPot=127;
    float mixGain = 1;
    byte mixDryPot;
    float mixDry = 1;
    byte mixFX1Pot;
    float mixFX1 = 0;
    byte mixFX2Pot;
    float mixFX2 = 0;
    byte mixFX3Pot;
    float mixFX3 = 0;
    bool muted;
    bool soloed;
    bool muteThruSolo;
    int internal_clock = 0;
    int internal_clock_bar = 0;
    byte clip_to_play[256];
    Track(ILI9341_t3n *display, byte Y)
    {
        // MIDI1.setHandleNoteOn(myNoteOn);
        // MIDI1.setHandleNoteOff(myNoteOff);
        // MIDI1.begin();
        // SD.begin(BUILTIN_SDCARD);
        tft = display;
        my_Arranger_Y_axis = Y;
        MIDI_channel_in = Y;
        parameter[SET_MIDICH_OUT] = Y;
        // allocate tracks0-7 "array"
        clip = static_cast<clip_t *>(calloc(MAX_CLIPS, sizeof(clip_t)));
        // fill the tracks0-7 "array"

        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t < MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    clip[c].tick[t].voice[v] = NO_NOTE;
                    clip[c].tick[t].velo[v] = 7;
                    clip[c].tick[t].stepFX = 0;
                }
            }
        }
        if (clip == nullptr)
        {
            // Fehlerbehandlung, wenn calloc fehlschlägt
            Serial.println("Memory allocation failed");
        }

        for (int i = 0; i < 256; i++)
        {
            clip_to_play[i] = 8;
            noteOffset[i] = 0;
            barVelocity[i] = 127;
            play_presetNr_ccChannel[i] = 8;
            play_presetNr_ccValue[i] = 8;
        }
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < 16; t++)
            {
                CCchannel[p][t] = 128;
                CCvalue[p][t] = 0;
            }
        }
    }

    void drawStepSequencerStatic();
    void set_stepSequencer_parameters(byte row);
    void draw_stepSequencer_parameters(byte row);

    void set_note_on_tick(int x, int y);
    void draw_notes_in_grid();
    void draw_sequencer_modes(byte mode);
    void set_recordState(bool _status);
    bool get_recordState();
    void record_noteOn(byte Note, byte Velo, byte Channel);
    void record_noteOff(byte Note, byte Velo, byte Channel);
    void draw_MIDI_CC_screen();
    void set_MIDI_CC(byte row);
    // update
    void update(int PixelX, byte gridY);
    void noteOn(byte Note, byte Velo, byte Channel);
    void noteOff(byte Note, byte Velo, byte Channel);
    void save_track();
    void load_track();
    // songmode
    void draw_clip_to_play(byte n, byte b);
    void draw_arrangment_line(byte n, byte b);
    void draw_arrangment_lines(byte n, byte b);
    void draw_arranger_parameters(byte lastProw);
    void set_clip_to_play(byte n, byte b);
    void set_note_offset(byte n, int b);
    void set_barVelocity(byte n, int b);
    void set_play_presetNr_ccChannel(byte n, byte lastProw);
    void set_play_presetNr_ccValue(byte n, byte lastProw);
    //
    void play_sequencer_mode(byte cloock, byte start, byte end);
    void set_seq_mode_parameters(byte row);

    byte get_active_note(byte _clip, byte _tick, byte _voice);

private:
    void play_seq_mode0(byte cloock);
    void play_seq_mode1(byte cloock);
    void set_seq_mode1_parameters(byte row);
    void set_seq_mode1_value(byte XPos, byte YPos, const char *name);
    void draw_seq_mode1();

    void play_seq_mode2(byte cloock);
    void set_seq_mode2_parameters(byte row);
    void set_seq_mode2_value(byte XPos, byte YPos, const char *name, int min, int max);
    void draw_seq_mode2();

    void play_seq_mode3(byte cloock);
    void set_seq_mode3_parameters(byte row);
    void set_seq_mode3_value(byte XPos, byte YPos, const char *name, int min, int max);
    void draw_seq_mode3();

    void play_seq_mode4(byte cloock);
    void set_seq_mode4_parameters(byte row);
    void set_seq_mode4_value(byte XPos, byte YPos, const char *name);
    void draw_seq_mode4();

    ILI9341_t3n *tft; // Pointer to the display object

    byte MIDI_channel_in;

    byte my_Arranger_Y_axis;
    byte note2set;
    byte setStepFX = 74;
    byte tickStart;
    byte noteToPlay[MAX_VOICES];
    int pixelOn_X;
    int pixelOn_Y;
    int gridX_4_save;
    byte gridY_4_save;
    
    int noteOffset[256];
    byte barVelocity[256];
    byte sTick;

    bool internal_clock_is_on = false;

    int recordStartTick[MAX_VOICES];
    byte recordLastNote[MAX_VOICES];
    byte recordVelocity[MAX_VOICES];
    byte recordVoice;
    byte recordChannel;
    byte sequence_length = MAX_TICKS;
    struct tick_t
    {
        byte voice[MAX_VOICES];
        byte velo[MAX_VOICES];
        byte stepFX;
    };
    struct clip_t
    {
        tick_t tick[MAX_TICKS];
    };
    clip_t *clip = nullptr;
    byte search_free_voice = 0;
    byte old_cnote = NO_NOTE;
    byte oldNotesInArray[MAX_VOICES]{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE};
    bool note_is_on[MAX_VOICES] = {true, true, true, true, true, true, true, true, true, true, true, true};
    bool ready_for_NoteOff[MAX_VOICES] = {false, false, false, false, false, false, false, false, false, false, false, false};
    byte CCvalue[NUM_PRESETS + 1][16];
    byte CCchannel[NUM_PRESETS + 1][16];
    byte edit_presetNr_ccChannel = 0;
    byte edit_presetNr_ccValue = 0;
    byte play_presetNr_ccChannel[256];
    byte play_presetNr_ccValue[256];
    byte bar_to_edit = 0;

    byte SeqMod1Value[16]; // oct- ; oct+ ; vol- ; vol+
    byte SeqMod2Value[16];
    byte SeqMod3Value[16];
    byte SeqMod4Value[16];
    byte maxVal;

    byte mute_norm_solo_pot = 1;

    

    // sequencer Modes

    // void play_seq_mode0(byte cloock);

    void set_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name, byte min, byte max);
    void draw_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name);

    void set_stepSequencer_parameter_text(byte XPos, byte YPos, const char *name, const char *text, byte min, byte max);
    void draw_stepSequencer_parameter_text(byte XPos, byte YPos, const char *text, const char *name);
    // sequencer options:
    // octave

    void set_CCvalue(byte XPos, byte YPos);
    void set_CCchannel(byte XPos, byte YPos);
    void draw_MIDI_CC(byte XPos, byte YPos);

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
    void set_active_note(byte _clip, byte _tick, byte _voice, byte _note);
    
    void set_active_velo(byte _clip, byte _tick, byte _voice, byte _velo);
    byte get_active_velo(byte _clip, byte _tick, byte _voice);
    void set_active_stepFX(byte _clip, byte _tick, byte _voice, byte _stepFX);
    byte get_active_stepFX(byte _clip, byte _tick, byte _voice);
    void check_for_free_voices(byte onTick, byte cnote);
    void clear_notes_on_tick(byte cl_X);
    void draw_note_on_tick(byte dr_X);
    void clear_notes_in_grid();

    // stepsequencer

    void draw_Notenames();
    void drawOctaveTriangle();
    void draw_Clipselector();

    //----------------------------------------------------------------
    // arranger stuff
    void change_presets();
    void set_arranger_parameters(byte lastProw);

    // clip to play

    
    void drawsongmodepageselector();
    void gridSongMode(int songpageNumber);

    
    void draw_clipNr_arranger(byte n, byte b);
    byte get_clip_to_play(byte when);
    // note offset / note transpose

    void draw_noteOffset(byte n, int b);
    void draw_offset_arranger(byte n, byte b);
    // bar Velocity

    void draw_barVelocity(byte n, int b);
    // MIDI CC

    void draw_play_presetNr_ccChannel(byte n, byte lastProw);

    void draw_play_presetNr_ccValue(byte n, byte lastProw);
};

extern Track *allTracks[8];
#endif