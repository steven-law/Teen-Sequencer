
#include <Arduino.h>
#include <project_variables.h>
#include <Plugins/pluginClass.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
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
    // Serial.println(potentiometer[presetNr][n]);
    return potentiometer[presetNr][n];
}

void PluginControll::save_plugin()
{
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");
    trellisPressed[TRELLIS_BUTTON_ENTER] = false;

    sprintf(_trackname, "plugin%d.txt\0", myID);
    Serial.println(_trackname);

    // delete the file:
    // Serial.println("Removing:");
    SD.remove(_trackname);
    // Serial.println("Done:");

    // open the file.
    // Serial.println("Creating and opening:");
    myFile = SD.open(_trackname, FILE_WRITE);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (myFile)
    {
        // save tracks
        // Serial.println("Writing track:");

        for (int p = 0; p < NUM_PLUGIN_PRESETS; p++)
        {

            for (int v = 0; v < 16; v++)
            {
                myFile.print((char)this->potentiometer[p][v]);
                Serial.printf("save plugin parameter preset: %d, value: %d\n", p, this->potentiometer[p][v]);
            }
        }
        myFile.print((char)this->presetNr);
        // close the file:
        myFile.close();
        // Serial.println("all saved:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }
    Serial.println("plugin saving Done:");
}
void PluginControll::load_plugin()
{
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");

    sprintf(_trackname, "plugin%d.txt\0", myID);
    Serial.println(_trackname);

    // open the file.
    // Serial.println("Creating and opening:");
    myFile = SD.open(_trackname, FILE_READ);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (myFile)
    {
        // save tracks
        // Serial.println("Writing track:");

        for (int p = 0; p < NUM_PLUGIN_PRESETS; p++)
        {
            for (int v = 0; v < 16; v++)
            {
                this->potentiometer[p][v] = myFile.read();

                Serial.printf("load plugin parameter preset: %d, value: %d\n", p, this->potentiometer[p][v]);
            }
        }
        this->presetNr = myFile.read();
        // close the file:
        myFile.close();
        Serial.println("all loaded:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }
    Serial.println("plugin loading Done:");
}
