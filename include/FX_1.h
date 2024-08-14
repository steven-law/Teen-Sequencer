#ifndef FX_1_H
#define FX_1_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "project_variables.h"
#include <pluginClass.h>
#include <plugin_List.h>

#include "mixers.h"

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern bool buttonPressed[NUM_BUTTONS];
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

class FX_1 : public PluginControll
{
public:
    AudioAmplifier pl_1;
    AudioAmplifier pl_2;
    AudioAmplifier pl_3;
    AudioAmplifier pl_4;
    AudioAmplifier pl_5;
    AudioAmplifier pl_6;
    AudioAmplifier pl_7;
    AudioAmplifier pl_8;
    AudioMixer12 FX_mixer;
    AudioEffectFreeverb freeverb;
    AudioConnection *patchCord[17]; // total patchCordCount:46 including array typed ones.

    FX_1(const char *Name, byte ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(plugin_1.SongVol, 0, pl_1, 0);
        patchCord[pci++] = new AudioConnection(plugin_2.SongVol, 0, pl_2, 0);
        patchCord[pci++] = new AudioConnection(plugin_3.SongVol, 0, pl_3, 0);
        patchCord[pci++] = new AudioConnection(plugin_4.SongVol, 0, pl_4, 0);
        patchCord[pci++] = new AudioConnection(plugin_5.SongVol, 0, pl_5, 0);
        patchCord[pci++] = new AudioConnection(plugin_6.SongVol, 0, pl_6, 0);
        patchCord[pci++] = new AudioConnection(plugin_7.SongVol, 0, pl_7, 0);
        patchCord[pci++] = new AudioConnection(plugin_8.SongVol, 0, pl_8, 0);
        patchCord[pci++] = new AudioConnection(pl_1, 0, FX_mixer, 0);
        patchCord[pci++] = new AudioConnection(pl_2, 0, FX_mixer, 1);
        patchCord[pci++] = new AudioConnection(pl_3, 0, FX_mixer, 2);
        patchCord[pci++] = new AudioConnection(pl_4, 0, FX_mixer, 3);
        patchCord[pci++] = new AudioConnection(pl_5, 0, FX_mixer, 4);
        patchCord[pci++] = new AudioConnection(pl_6, 0, FX_mixer, 5);
        patchCord[pci++] = new AudioConnection(pl_7, 0, FX_mixer, 6);
        patchCord[pci++] = new AudioConnection(pl_8, 0, FX_mixer, 7);
        patchCord[pci++] = new AudioConnection(FX_mixer, 0, freeverb, 0);
    }
  
    virtual ~FX_1() = default;

    virtual void setup() override;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) override;
    virtual void noteOff(byte notePlayed, byte voice) override;
    virtual void set_parameters(byte row) override;
    virtual void draw_plugin() override;

    void set_RV_roomsize(byte XPos, byte YPos, const char *name);
    void set_RC_damping(byte XPos, byte YPos, const char *name);
};
#endif // PLUGIN_3_H