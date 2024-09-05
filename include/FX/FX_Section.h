#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "project_variables.h"
#include <Plugins/plugin_List.h>
#include <FX/fx_List.h>
#include "ownLibs/mixers.h"

class FX_Section
{
public:
    //   byte FX2_Potentiomer[2];
    byte FX3_Potentiomer[2];
    // AudioAnalyzePeak peak3;
    // AudioAnalyzePeak peak2;
    AudioAmplifier dry[NUM_PLUGINS];
    AudioMixer12 dry_mixer;
    // AudioMixer12 FX3_mixer;
    AudioMixer4 endmixer;
    AudioConnection *patchCord[(NUM_PLUGINS * 2) + 4]; // total patchCordCount:46 including array typed ones.

    // constructor (this is called when class-object is created)
    FX_Section()
    {
        int pci = 0; // used only for adding new patchcords

        // patchCord[pci++] = new AudioConnection(plugin_3.modulator[0], 0, peak3, 0);
        // patchCord[pci++] = new AudioConnection(plugin_2.waveform[0], 0, peak2, 0);

        patchCord[pci++] = new AudioConnection(plugin_1.SongVol, 0, dry[0], 0);
        patchCord[pci++] = new AudioConnection(plugin_2.SongVol, 0, dry[1], 0);
        patchCord[pci++] = new AudioConnection(plugin_3.SongVol, 0, dry[2], 0);
        patchCord[pci++] = new AudioConnection(plugin_4.SongVol, 0, dry[3], 0);
        patchCord[pci++] = new AudioConnection(plugin_5.SongVol, 0, dry[4], 0);
        patchCord[pci++] = new AudioConnection(plugin_6.SongVol, 0, dry[5], 0);
        patchCord[pci++] = new AudioConnection(plugin_7.SongVol, 0, dry[6], 0);
        patchCord[pci++] = new AudioConnection(plugin_8.SongVol, 0, dry[7], 0);
        patchCord[pci++] = new AudioConnection(plugin_9.SongVol, 0, dry[8], 0);
        patchCord[pci++] = new AudioConnection(plugin_10.SongVol, 0, dry[9], 0);
        patchCord[pci++] = new AudioConnection(plugin_11.SongVol, 0, dry[10], 0);

        for (int i = 0; i < NUM_PLUGINS; i++)
        {
            patchCord[pci++] = new AudioConnection(dry[i], 0, dry_mixer, i);
        }

        patchCord[pci++] = new AudioConnection(dry_mixer, 0, endmixer, 0);
        patchCord[pci++] = new AudioConnection(fx_1.freeverb, 0, endmixer, 1);
        patchCord[pci++] = new AudioConnection(fx_2.bitcrusher, 0, endmixer, 2);
        patchCord[pci++] = new AudioConnection(fx_3.delayMixer, 0, endmixer, 3);
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
        dry_mixer.gain(5, 1);
        dry_mixer.gain(6, 1);
        dry_mixer.gain(7, 1);
        dry_mixer.gain(8, 1);

        endmixer.gain(0, 1);
        endmixer.gain(1, 1);
        endmixer.gain(2, 1);
        endmixer.gain(3, 1);
    }

    void get_peak()
    {
        // Serial.printf("FX2: %f , FX3: %f \n", peak2.read(), peak3.read());
    }
};
// TeensyDAW: end automatically generated code
