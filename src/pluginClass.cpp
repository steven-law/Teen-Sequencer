
#include <Arduino.h>
#include <project_variables.h>
#include <pluginClass.h>
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern bool buttonPressed[NUM_BUTTONS];
void drawPot(int XPos, byte YPos, int dvalue, const char *dname);

void PluginControll::setup() {}
void PluginControll::noteOn(byte notePlayed, float velocity, byte voice) {}
void PluginControll::noteOff(byte notePlayed, byte voice) {}
void PluginControll::set_parameters(byte row) {}
void PluginControll::draw_plugin() {}

void PluginControll::set_presetNr()
{
    if (enc_moved[PRESET_ENCODER])
    {
        presetNr = constrain(presetNr + encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        draw_plugin();
    }
}
byte PluginControll::get_Potentiometer(byte XPos, byte YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    potentiometer[presetNr][n] = constrain(potentiometer[presetNr][n] + encoded[XPos], 0, MIDI_CC_RANGE);
    drawPot(XPos, YPos, potentiometer[presetNr][n], name);
    //Serial.println(potentiometer[presetNr][n]);
    return potentiometer[presetNr][n];
}


