#ifndef PLUGIN_7_H
#define PLUGIN_7_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "mixers.h"
#include <pluginClass.h>
#include <synth_fm_drum.h>
#include <effect_dynamics.h>

// TeensyDAW: begin automatically generated code
// Name: Boom
// Description: Subtractive Synthesizer
// Voices: 1

// Kick
// Pot 1: Sweep
// Pot 2: Noise
// Pot 3: O-Drive
// Pot 4: Decay

//
// Pot 5:
// Pot 6:
// Pot 7:
// Pot 8:

//
// Pot 9:
// Pot 10:
// Pot 11:
// Pot 12:

//
// Pot 13:
// Pot 14:
// Pot 15:
// Pot 16:

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern bool buttonPressed[NUM_BUTTONS];
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();
class Plugin_7 : public PluginControll
{
public:
    float threshold;
    float attack = 0.1f;
    float release = 0.5f;
    float hysterisis = 1.0f;
    float ratio = 35.0f;
    float kneeWidth = 1.0f;

    AudioSynthFMDrum fm_drum;
    AudioEffectDynamics dynamics;
    AudioAmplifier amp;
    AudioAmplifier MixGain;
    AudioAmplifier SongVol;
    AudioConnection *patchCord[4]; // total patchCordCount:2 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_7(const char *Name, byte ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        // patchCord[pci++] = new AudioConnection(fm_drum, 0, dynamics, 0);
        // patchCord[pci++] = new AudioConnection(dynamics, 0, MixGain, 0);
        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);

        patchCord[pci++] = new AudioConnection(fm_drum, 0, dynamics, 0);
        patchCord[pci++] = new AudioConnection(dynamics, 0, amp, 0);
        patchCord[pci++] = new AudioConnection(amp, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
    }
    virtual ~Plugin_7() = default;

    virtual void setup() override;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) override;
    virtual void noteOff(byte notePlayed, byte voice) override;
    virtual void set_parameters(byte row) override;
    virtual void draw_plugin() override;

    void set_fmdrum_pitchMod(byte XPos, byte YPos, const char *name);
    void set_fmdrum_decay(byte XPos, byte YPos, const char *name);
    void set_fmdrum_noise(byte XPos, byte YPos, const char *name);
    void set_fmdrum_overdrive(byte XPos, byte YPos, const char *name);

    void set_dynamics_threshold(byte XPos, byte YPos, const char *name);
    void set_dynamics_attack(byte XPos, byte YPos, const char *name);
    void set_dynamics_release(byte XPos, byte YPos, const char *name);
    void set_dynamics_hysterisis(byte XPos, byte YPos, const char *name);

    void set_dynamics_ratio(byte XPos, byte YPos, const char *name);
    void set_dynamics_kneeWidth(byte XPos, byte YPos, const char *name);
    void set_dynamics_makeupGain(byte XPos, byte YPos, const char *name);
    void set_dynamics_autoMakeupGain(byte XPos, byte YPos, const char *name);
    void set_amp_gain(byte XPos, byte YPos, const char *name);
};
#endif // PLUGIN_3_H