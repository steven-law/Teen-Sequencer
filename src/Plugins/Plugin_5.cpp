#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "project_variables.h"
#include "project_functions.h"
#include <Plugins/Plugin_5.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

void Plugin_5::setup()
{

    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
    }
    pink.amplitude(1);
    hhEnv.delay(0);
    hhEnv.attack(0);
    hhEnv.hold(0);
    hhEnv.decay(0);
    hhEnv.sustain(0.4);
    hhFilterEnv.sustain(0.4);
    hhEnv.release(200);

    MixGain.gain(1);
    SongVol.gain(1);
    tomL.secondMix(0);
    tomL.pitchMod(0.7);
    tomM.secondMix(0);
    tomM.pitchMod(0.7);
    tomH.secondMix(0);
    tomH.pitchMod(0.7);
}
void Plugin_5::noteOn(byte notePlayed, float velocity, byte voice)
{
    if (voice == 0)
        fm_drum.noteOn();
    if (voice == 1)
        fm_snare.noteOn();
    if (voice == 2)
    {
        hhEnv.noteOn();
        hhFilterEnv.noteOn();
    }
    if (voice == 3)
        tomL.noteOn();
    if (voice == 4)
        tomM.noteOn();
    if (voice == 5)
        tomH.noteOn();
}
void Plugin_5::noteOff(byte notePlayed, byte voice)
{
    if (notePlayed == 50)
    {
        hhEnv.noteOff();
        hhFilterEnv.noteOff();
    }
}

void Plugin_5::set_parameters(byte row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_fmdrum_frequency(0, 0, "Freq", 10, 300);
            set_fmdrum_pitchMod(1, 0, "Sweep");
            set_fmdrum_overdrive(2, 0, "O-Drive");
            set_fmdrum_decay(3, 0, "Decay");
        }

        if (row == 1)
        {
            set_fmsnare_frequency(0, 1, "Freq", 10, 300);
            set_fmsnare_pitchMod(1, 1, "Sweep");
            set_fmsnare_noise(2, 1, "Noise");
            set_fmsnare_decay(3, 1, "Decay");
        }

        if (row == 2)
        {
            set_hhfilter_frequency(0, 2, "Freq", 1000, 8000);
            set_hhfilter_resonance(1, 2, "Reso", 0, 5.00);
            set_hhEnv_attack(2, 2, "Attack", 0, 50);
            // set_hhEnv_decay(3, 2, "Decay", 0, 50);
            set_hhEnv_release(3, 2, "Decay", 0, 2000);
        }

        if (row == 3)
        {
            set_tomL_frequency(0, 3, "TomL");
            set_tomM_frequency(1, 3, "TomM");
            set_tomH_frequency(2, 3, "TomH");
            set_toms_decay(3, 3, "Decay", 0, 2000);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_5::draw_plugin()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        mytft->clearWorkSpace();
        // Serial.println("drawing plugin 5");
        mytft->drawPot(0, 0, potentiometer[presetNr][0], "Freq");
        mytft->drawPot(1, 0, potentiometer[presetNr][1], "Sweep");
        mytft->drawPot(2, 0, potentiometer[presetNr][2], "O-Drive");
        mytft->drawPot(3, 0, potentiometer[presetNr][3], "Decay");

        mytft->drawPot(0, 1, potentiometer[presetNr][4], "Freq");
        mytft->drawPot(1, 1, potentiometer[presetNr][5], "Sweep");
        mytft->drawPot(2, 1, potentiometer[presetNr][6], "Noise");
        mytft->drawPot(3, 1, potentiometer[presetNr][7], "Decay");

        mytft->drawPot(0, 2, potentiometer[presetNr][8], "Freq");
        mytft->drawPot(1, 2, potentiometer[presetNr][9], "Reso");
        mytft->drawPot(2, 2, potentiometer[presetNr][10], "Attack");
        mytft->drawPot(3, 2, potentiometer[presetNr][11], "Decay");

        mytft->drawPot(0, 3, potentiometer[presetNr][12], "TomL");
        mytft->drawPot(1, 3, potentiometer[presetNr][13], "TomM");
        mytft->drawPot(2, 3, potentiometer[presetNr][14], "TomH");
        mytft->drawPot(3, 3, potentiometer[presetNr][15], "Decay");

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_5::change_preset(){}
void Plugin_5::set_fmsnare_frequency(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int freq = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        fm_snare.frequency(freq);
    }
}
void Plugin_5::set_fmsnare_pitchMod(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_snare.fm(sustain);
    }
}
void Plugin_5::set_fmsnare_decay(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_snare.decay(sustain);
    }
}
void Plugin_5::set_fmsnare_noise(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_snare.noise(sustain);
        fm_snare.overdrive(sustain);
    }
}
void Plugin_5::set_fmsnare_overdrive(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_snare.overdrive(sustain);
    }
}

void Plugin_5::set_fmdrum_frequency(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        //int n = XPos + (YPos * NUM_ENCODERS);
        //potentiometer[presetNr][n] = getEncodervalue(XPos, YPos, name, potentiometer[presetNr][n]);

        int freq = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        fm_drum.frequency(freq);
    }
}
void Plugin_5::set_fmdrum_pitchMod(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_drum.fm(sustain);
    }
}
void Plugin_5::set_fmdrum_decay(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
        fm_drum.decay(sustain);
    }
}
void Plugin_5::set_fmdrum_noise(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
        fm_drum.noise(sustain);
    }
}
void Plugin_5::set_fmdrum_overdrive(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
        fm_drum.overdrive(sustain);
    }
}

void Plugin_5::set_hhfilter_frequency(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int frequency = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        filter.frequency(frequency);
    }
}
void Plugin_5::set_hhfilter_resonance(byte XPos, byte YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        float reso = (float)(get_Potentiometer(XPos, YPos, name) / (MIDI_CC_RANGE_FLOAT / max)) + min;
        for (int i = 0; i < MAX_VOICES; i++)
        {
            filter.resonance(reso);
        }
    }
}
void Plugin_5::set_hhfilter_sweep(byte XPos, byte YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        float swp = get_Potentiometer(XPos, YPos, name) / (MIDI_CC_RANGE_FLOAT / max) + min;
        for (int i = 0; i < MAX_VOICES; i++)
        {
            filter.octaveControl(swp);
        }
    }
}

void Plugin_5::set_hhEnv_attack(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int attack = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        hhEnv.attack(attack);
    }
}
void Plugin_5::set_hhEnv_decay(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int decay = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        hhEnv.decay(decay);
        hhFilterEnv.decay(decay);
    }
}
void Plugin_5::set_hhEnv_sustain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
        hhEnv.sustain(sustain);
        hhFilterEnv.sustain(sustain);
    }
}
void Plugin_5::set_hhEnv_release(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int release = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        hhEnv.release(release);
        hhFilterEnv.release(release);
        hhEnv.decay(release / 4);
        hhFilterEnv.decay(release / 4);
    }
}

void Plugin_5::set_tomL_frequency(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int freq = note_frequency[get_Potentiometer(XPos, YPos, name)] * tuning;
        tomL.frequency(freq);
    }
}
void Plugin_5::set_tomM_frequency(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int freq = note_frequency[get_Potentiometer(XPos, YPos, name)] * tuning;
        tomM.frequency(freq);
    }
}
void Plugin_5::set_tomH_frequency(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int freq = note_frequency[get_Potentiometer(XPos, YPos, name)] * tuning;
        tomH.frequency(freq);
    }
}
void Plugin_5::set_toms_decay(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int decay = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);
        tomL.length(decay);
        tomM.length(decay);
        tomH.length(decay);
    }
}