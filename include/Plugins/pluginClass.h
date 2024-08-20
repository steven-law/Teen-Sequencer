#ifndef PLUGIN_CLASS
#define PLUGIN_CLASS

#include <project_variables.h>
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
void drawPot(int XPos, byte YPos, int dvalue, const char *dname);

class PluginControll
{
public:
    byte myID;
    byte potentiometer[NUM_PLUGIN_PRESETS][16];
    byte presetNr = 0;

    const char *name;
    PluginControll(const char *Name, byte ID)
    {
        name = Name;
        myID = ID;

    }
    virtual ~PluginControll() = default;
    virtual const char *get_Name() { return name; }
    virtual byte get_ID() { return myID; }

    virtual void setup() = 0;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) = 0;
    virtual void noteOff(byte notePlayed, byte voice) = 0;
    virtual void set_parameters(byte row) = 0;
    virtual void draw_plugin() = 0;

    virtual void set_presetNr();
    virtual byte get_Potentiometer(byte XPos, byte YPos, const char *name);
    
};
#endif // PLUGIN_CLASS