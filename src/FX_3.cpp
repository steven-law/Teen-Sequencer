#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "mixers.h"
#include "project_variables.h"
#include <FX_3.h>

void FX_3::setup()
{
    FX_mixer.gain(0, 1);
    FX_mixer.gain(1, 1);
    FX_mixer.gain(2, 1);
    FX_mixer.gain(3, 1);
    FX_mixer.gain(4, 1);
}
void FX_3::noteOn(byte notePlayed, float velocity, byte voice) {}
void FX_3::noteOff(byte notePlayed, byte voice) {}
void FX_3::set_parameters(byte row){}
void FX_3::draw_plugin(){}
