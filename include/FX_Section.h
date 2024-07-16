#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "project_variables.h"
#include <plugin_List.h>
#include <fx_List.h>
#include "mixers.h"

class FX_Section
{
public:
    //   byte FX2_Potentiomer[2];
    byte FX3_Potentiomer[2];
    AudioAnalyzePeak peak3;
    AudioAnalyzePeak peak2;
    AudioAmplifier dry_1;

    AudioAmplifier FX3_1;
    AudioAmplifier dry_2;

    AudioAmplifier FX3_2;
    AudioAmplifier dry_3;

    AudioAmplifier FX3_3;
    AudioAmplifier dry_4;

    AudioAmplifier FX3_4;
    AudioAmplifier dry_5;

    AudioAmplifier FX3_5;
    AudioAmplifier dry_6;

    AudioAmplifier FX3_6;
    AudioAmplifier dry_7;

    AudioAmplifier FX3_7;
    AudioAmplifier dry_8;

    AudioAmplifier FX3_8;
    AudioMixer12 dry_mixer;

    AudioMixer12 FX3_mixer;

    AudioMixer4 endmixer;
    AudioConnection *patchCord[22]; // total patchCordCount:46 including array typed ones.

    // constructor (this is called when class-object is created)
    FX_Section()
    {
        int pci = 0; // used only for adding new patchcords

        //patchCord[pci++] = new AudioConnection(plugin_3.modulator[0], 0, peak3, 0);
        //patchCord[pci++] = new AudioConnection(plugin_2.waveform[0], 0, peak2, 0);

        //patchCord[pci++] = new AudioConnection(plugin_1.SongVol, 0, dry_1, 0);
        patchCord[pci++] = new AudioConnection(plugin_2.SongVol, 0, dry_2, 0);
        patchCord[pci++] = new AudioConnection(plugin_3.SongVol, 0, dry_3, 0);
        //patchCord[pci++] = new AudioConnection(plugin_4.SongVol, 0, dry_4, 0);
        //patchCord[pci++] = new AudioConnection(plugin_5.SongVol, 0, dry_5, 0);
        //patchCord[pci++] = new AudioConnection(plugin_6.SongVol, 0, dry_6, 0);
        //patchCord[pci++] = new AudioConnection(plugin_7.SongVol, 0, dry_7, 0);
        //patchCord[pci++] = new AudioConnection(plugin_8.SongVol, 0, dry_8, 0);
        patchCord[pci++] = new AudioConnection(dry_1, 0, dry_mixer, 0);
        patchCord[pci++] = new AudioConnection(dry_2, 0, dry_mixer, 1);
        patchCord[pci++] = new AudioConnection(dry_3, 0, dry_mixer, 2);
        patchCord[pci++] = new AudioConnection(dry_4, 0, dry_mixer, 3);
        patchCord[pci++] = new AudioConnection(dry_5, 0, dry_mixer, 4);
        patchCord[pci++] = new AudioConnection(dry_6, 0, dry_mixer, 5);
        patchCord[pci++] = new AudioConnection(dry_7, 0, dry_mixer, 6);
        patchCord[pci++] = new AudioConnection(dry_8, 0, dry_mixer, 7);
        patchCord[pci++] = new AudioConnection(dry_mixer, 0, endmixer, 0);
        patchCord[pci++] = new AudioConnection(fx_1.freeverb, 0, endmixer, 1);
        patchCord[pci++] = new AudioConnection(fx_2.bitcrusher, 0, endmixer, 2);
        patchCord[pci++] = new AudioConnection(fx_3.FX_mixer, 0, endmixer, 3);
    }
    void setup()
    {
        fx_1.setup();
        fx_2.setup();
        fx_3.setup();
        dry_mixer.gain(0, 1);
        dry_mixer.gain(1, 1);
        dry_mixer.gain(2, 1);
        dry_mixer.gain(3, 1);
        dry_mixer.gain(4, 1);

        endmixer.gain(0, 1);
        endmixer.gain(1, 1);
        endmixer.gain(2, 1);
        endmixer.gain(3, 1);
    }

    void get_peak()
    {
        Serial.printf("FX2: %f , FX3: %f \n", peak2.read(), peak3.read());
    }
};
// TeensyDAW: end automatically generated code
