#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "project_variables.h"
#include "project_functions.h"
#include <Plugins/Plugin_10.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();
extern const char *filterName[4];

void Plugin_10::setup()
{

    dc.amplitude(1);

    waveform.setInstrument(Flute_100kbyte);
    waveform.amplitude(1);

    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(1);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(4);
    // ladder[i].frequency(2000);
    // ladder[i].resonance(1);
    // ladder[i].octaveControl(4);

    fMixer.gain(0, 1);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);

    Aenv.delay(0);
    Aenv.attack(0);
    Aenv.hold(0);
    Aenv.decay(0);
    Aenv.sustain(1);
    Aenv.release(200);

    // mixer.gain(0, 1);

    MixGain.gain(1);
    SongVol.gain(1);
}
void Plugin_10::noteOn(byte notePlayed, float velocity, byte voice)
{
    float frequency = note_frequency[notePlayed] * tuning;
    Serial.printf("pl10: notePlayed= %d\n", notePlayed);
    waveform.playFrequency(frequency);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_10::noteOff(byte notePlayed, byte voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_10::set_parameters(byte row)
{
    draw_plugin();
    if (!trellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {

            set_voice_waveform(0, 0, "W~Form");
            set_voice_amplitude(1, 0, "Volume");
        }

        if (row == 1)
        {
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2, "Filt-Frq");
            set_filter_resonance(1, 2, "Resonance", 0, 5.00);
            set_filter_sweep(2, 2, "Sweep");
            set_filter_type(3, 2, filterName[potentiometer[presetNr][11]]);
        }

        if (row == 3)
        {
            set_envelope_ADSR(3, 1000, 500, 2000);
            // set_envelope_attack(0, 3, "Attack", 0, 1000);
            // set_envelope_sustain(2, 3, "Sustain");
            // set_envelope_decay(1, 3, "Decay", 0, 500);
            // set_envelope_release(3, 3, "Release", 0, 2000);
        }
    }
    if (trellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_10::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], "W~Form");
        drawPot(1, 0, potentiometer[presetNr][1], "Volume");

        drawPot(0, 2, potentiometer[presetNr][8], "Filt-Frq");
        drawPot(1, 2, potentiometer[presetNr][9], "Resonance");
        drawPot(2, 2, potentiometer[presetNr][10], "Sweep");
        drawPot(3, 2, potentiometer[presetNr][11], filterName[potentiometer[presetNr][11]]);

        drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                     potentiometer[presetNr][14], potentiometer[presetNr][15]);
        // drawPot(0, 3, potentiometer[presetNr][12], "Attack");
        // drawPot(1, 3, potentiometer[presetNr][13], "Decay");
        // drawPot(2, 3, potentiometer[presetNr][14], "Sustain");
        // drawPot(3, 3, potentiometer[presetNr][15], "Release");

        draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}

void Plugin_10::set_voice_waveform(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_waveform(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_10::assign_voice_waveform(byte value)
{
    byte walveform = map(value, 0, MIDI_CC_RANGE, 0,27);
    switch (walveform)
    {
    case 0:
        waveform.setInstrument(Flute_100kbyte);
        break;
    case 1:
        waveform.setInstrument(BasicFlute1);
        break;
    case 2:
        waveform.setInstrument(FrenchHorns);
        break;
    case 3:
        waveform.setInstrument(Ocarina);
        break;
    case 4:
        waveform.setInstrument(Ocarina);
        break;
    case 5:
        waveform.setInstrument(Pizzicato);
        break;
    case 6:
        waveform.setInstrument(bassoon);
        break;
    case 7:
        waveform.setInstrument(clarinet);
        break;

         case 8:
        waveform.setInstrument(distortiongt);
        break;
         case 9:
        waveform.setInstrument(epiano);
        break;
         case 10:
        waveform.setInstrument(flute);
        break;
         case 11:
        waveform.setInstrument(glockenspiel);
        break;
         case 12:
        waveform.setInstrument(gtfretnoise);
        break;
         case 13:
        waveform.setInstrument(harmonica);
        break;
         case 14:
        waveform.setInstrument(harp);
        break;
         case 15:
        waveform.setInstrument(mutedgtr);
        break;
         case 16:
        waveform.setInstrument(nylonstrgtr);
        break;
         case 17:
        waveform.setInstrument(oboe);
        break;
         case 18:
        waveform.setInstrument(overdrivegt);
        break;
         case 19:
        waveform.setInstrument(piano);
        break;
         case 20:
        waveform.setInstrument(recorder);
        break;
         case 21:
        waveform.setInstrument(standard_DRUMS);
        break;
         case 22:
        waveform.setInstrument(steelstrgtr);
        break;
         case 23:
        waveform.setInstrument(strings);
        break;
         case 24:
        waveform.setInstrument(trombone);
        break;
         case 25:
        waveform.setInstrument(trumpet);
        break;
         case 26:
        waveform.setInstrument(tuba);
        break;
         case 27:
        waveform.setInstrument(vibraphone);
        break;

    default:
        waveform.setInstrument(Pizzicato);
        break;
    }
}

void Plugin_10::set_voice_amplitude(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_10::assign_voice_amplitude(byte value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform.amplitude(ampl);
}

void Plugin_10::set_filter_frequency(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_10::assign_filter_frequency(byte value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder[i].frequency(frequency);
}

void Plugin_10::set_filter_resonance(byte XPos, byte YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_10::assign_filter_resonance(byte value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder[i].resonance(reso);
}
void Plugin_10::set_filter_sweep(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_10::assign_filter_sweep(byte value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    // ladder[i].octaveControl(swp);
}
void Plugin_10::set_filter_type(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos, name), 0, 3));
    }
}
void Plugin_10::selectFilterType(byte mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_10::assign_envelope_attack(byte value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_10::assign_envelope_decay(byte value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_10::assign_envelope_sustain(byte value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_10::assign_envelope_release(byte value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_10::set_envelope_ADSR(byte YPos, int maxA, int maxD, int maxR)
{

    if (enc_moved[0])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][0 + rowIx] = constrain(potentiometer[presetNr][0 + rowIx] + encoded[0], 0, MIDI_CC_RANGE);
        assign_envelope_attack(potentiometer[presetNr][0 + rowIx], maxA);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[1])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][1 + rowIx] = constrain(potentiometer[presetNr][1 + rowIx] + encoded[1], 0, MIDI_CC_RANGE);
        assign_envelope_decay(potentiometer[presetNr][1 + rowIx], maxD);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[2])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][2 + rowIx] = constrain(potentiometer[presetNr][2 + rowIx] + encoded[2], 0, MIDI_CC_RANGE);
        assign_envelope_sustain(potentiometer[presetNr][2 + rowIx]);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[3])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][3 + rowIx] = constrain(potentiometer[presetNr][3 + rowIx] + encoded[3], 0, MIDI_CC_RANGE);
        assign_envelope_release(potentiometer[presetNr][3 + rowIx], maxR);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
}
void Plugin_10::set_envelope_attack(byte XPos, byte YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_attack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_10::set_envelope_decay(byte XPos, byte YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_decay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_10::set_envelope_sustain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {

        assign_envelope_sustain(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
    }
}
void Plugin_10::set_envelope_release(byte XPos, byte YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos, name), max);
    }
}