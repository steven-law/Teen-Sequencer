#ifndef FX_1_H
#define FX_1_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "project_variables.h"
#include <Plugins/pluginClass.h>
#include <Plugins/plugin_List.h>

#include "ownLibs/mixers.h"

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

class FX_1 : public PluginControll
{
public:
    AudioAmplifier pl[NUM_PLUGINS];
    AudioMixer12 FX_mixer;
    AudioEffectFreeverb freeverb;
    AudioConnection *patchCord[NUM_PLUGINS * 2 + 1]; // total patchCordCount:46 including array typed ones.

    FX_1(const char *Name, byte ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(plugin_1.SongVol, 0, pl[0], 0);
        patchCord[pci++] = new AudioConnection(plugin_2.SongVol, 0, pl[1], 0);
        patchCord[pci++] = new AudioConnection(plugin_3.SongVol, 0, pl[2], 0);
        patchCord[pci++] = new AudioConnection(plugin_4.SongVol, 0, pl[3], 0);
        patchCord[pci++] = new AudioConnection(plugin_5.SongVol, 0, pl[4], 0);
        patchCord[pci++] = new AudioConnection(plugin_6.SongVol, 0, pl[5], 0);
        patchCord[pci++] = new AudioConnection(plugin_7.SongVol, 0, pl[6], 0);
        patchCord[pci++] = new AudioConnection(plugin_8.SongVol, 0, pl[7], 0);
        patchCord[pci++] = new AudioConnection(plugin_9.SongVol, 0, pl[8], 0);
        patchCord[pci++] = new AudioConnection(pl[0], 0, FX_mixer, 0);
        patchCord[pci++] = new AudioConnection(pl[1], 0, FX_mixer, 1);
        patchCord[pci++] = new AudioConnection(pl[2], 0, FX_mixer, 2);
        patchCord[pci++] = new AudioConnection(pl[3], 0, FX_mixer, 3);
        patchCord[pci++] = new AudioConnection(pl[4], 0, FX_mixer, 4);
        patchCord[pci++] = new AudioConnection(pl[5], 0, FX_mixer, 5);
        patchCord[pci++] = new AudioConnection(pl[6], 0, FX_mixer, 6);
        patchCord[pci++] = new AudioConnection(pl[7], 0, FX_mixer, 7);
        patchCord[pci++] = new AudioConnection(pl[8], 0, FX_mixer, 8);

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