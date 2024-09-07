#ifndef PLUGIN_9_H
#define PLUGIN_9_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
#include "project_variables.h"
#include "project_functions.h"

// TeensyDAW: begin automatically generated code
// Name: mDrm
// Description: Plays 12 Samplesounds stored on Flash (PROGMEM)
// Voices: 12

// Pot 1: Vol1
// Pot 2: Vol2
// Pot 3: Vol3
// Pot 4: Vol4

// Pot 5: Vol5
// Pot 6: Vol6
// Pot 7: Vol7
// Pot 8: Vol8

// Pot 9: Vol9
// Pot 10: Vol10
// Pot 11: Vol11
// Pot 12: Vol12
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();
class Plugin_9 : public PluginControll
{
public:
    AudioPlaySdRaw playSD[MAX_VOICES];
    AudioMixer12 mixer;
    AudioAmplifier MixGain;
    AudioAmplifier SongVol;
    AudioConnection *patchCord[MAX_VOICES+2]; // total patchCordCount:14 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_9(const char *Name, byte ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        for (int i = 0; i < MAX_VOICES; i++)
        {
            patchCord[pci++] = new AudioConnection(playSD[i], 0, mixer, i);
        }
    }
    virtual ~Plugin_9() = default;
    virtual void setup() override;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) override;
    virtual void noteOff(byte notePlayed, byte voice) override;
    virtual void set_parameters(byte row) override;
    virtual void draw_plugin() override;
    
 virtual void change_preset() override;
    void set_mixer_gain(byte XPos, byte YPos, const char *name);
    void assign_mixer_gain(byte value, byte channel);
};
#endif // PLUGIN_9_H