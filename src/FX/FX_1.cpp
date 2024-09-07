
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "project_variables.h"
#include <FX/FX_1.h>

void FX_1::setup()
{
    FX_mixer.gain(0, 1);
    FX_mixer.gain(1, 1);
    FX_mixer.gain(2, 1);
    FX_mixer.gain(3, 1);
    FX_mixer.gain(4, 1);
}
void FX_1::noteOn(byte notePlayed, float velocity, byte voice) {}
void FX_1::noteOff(byte notePlayed, byte voice) {}
void FX_1::set_parameters(byte row)
{
    draw_plugin();
    if (row == 0)
    {
        set_RV_roomsize(0, 0, "Roomsize");
        set_RC_damping(1, 0, "Damping");
    }
}
void FX_1::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, potentiometer[presetNr][0], "Roomsize");
        drawPot(1, 0, potentiometer[presetNr][1], "Damping");
    }
}

void FX_1::set_RV_roomsize(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float size = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        freeverb.roomsize(size);
    }
}
void FX_1::set_RC_damping(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float damp = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        freeverb.damping(damp);
    }
}
void FX_1::change_preset()
{
    float size = potentiometer[presetNr][0] / MIDI_CC_RANGE_FLOAT;
    freeverb.roomsize(size);
    float damp = potentiometer[presetNr][1] / MIDI_CC_RANGE_FLOAT;
    freeverb.damping(damp);
}