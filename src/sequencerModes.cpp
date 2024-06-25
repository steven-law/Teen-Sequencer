#include <Arduino.h>
#include "project_variables.h"
#include "Track.h"

void Track::play_SeqMode0(byte cloock)
{

    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (array[clip_to_play[internal_clock_bar]][cloock][v] < NO_NOTE)
        {
            if (!note_is_on[v])
            {
                Serial.println("send Note");
                noteToPlay[v] = array[clip_to_play[internal_clock_bar]][cloock][v] + noteOffset[internal_clock_bar];
                byte Velo = velocity[clip_to_play[internal_clock_bar]][cloock][v];
                note_is_on[v] = true;
                noteOn(noteToPlay[v], Velo, MIDI_channel_out);
            }
        }
        if (array[clip_to_play[internal_clock_bar]][cloock][v] == NO_NOTE)
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                noteOff(noteToPlay[v], 0, MIDI_channel_out);

                // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, v, noteToPlay[v]);
            }
        }
    }
}

void Track::play_SeqMode1(byte cloock)
{

    if (array[clip_to_play[internal_clock_bar]][cloock][0] < NO_NOTE)
    {
        if (!note_is_on[0])
        {
            noteToPlay[0] = random(0, 11) + (random(SeqMod_1_Poti[0], SeqMod_1_Poti[1]+1) * 12) + noteOffset[internal_clock_bar];
            byte Velo = random(SeqMod_1_Poti[2],SeqMod_1_Poti[3]);
            note_is_on[0] = true;
            noteOn(noteToPlay[0], Velo, MIDI_channel_out); // Send a Note (pitch 42, velo 127 on channel 1)
                                                           // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        }
    }

    if (array[clip_to_play[internal_clock_bar]][cloock][0] == NO_NOTE)
    {
        if (note_is_on[0])
        {
            note_is_on[0] = false;
            noteOff(noteToPlay[0], 0, MIDI_channel_out); // Send a Note (pitch 42, velo 127 on channel 1)
                                                         // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        }
    }
}
void Track::set_SeqMode1_parameters(byte row)
{
    draw_SeqMode1();
    if (row == 0)
    {
        set_SeqMode1_value(0, 0, "Oct -", 0, 0);
        set_SeqMode1_value(1, 0, "Oct +", 0, 0);
        set_SeqMode1_value(2, 0, "Vol -", 0, 0);
        set_SeqMode1_value(3, 0, "Vol +", 0, 0);
    }
}
void Track::set_SeqMode1_value(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod_1_Poti[n] = constrain(SeqMod_1_Poti[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod_1_Poti[n], name);
    }
}
void Track::draw_SeqMode1()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod_1_Poti[0], "Oct -");
        drawPot(1, 0, SeqMod_1_Poti[1], "Oct +");
        drawPot(2, 0, SeqMod_1_Poti[2], "Vol -");
        drawPot(3, 0, SeqMod_1_Poti[3], "Vol +");
    }
}

void Track::play_SeqMode2(byte cloock)
{
    static byte maxValIndex;
    static byte analogReadArray[12];
    byte cc23 = SeqMod_2_Poti[2];
    byte cc24 = SeqMod_2_Poti[3];
    byte thisOctave;

    if (array[clip_to_play[internal_clock_bar]][cloock][0] < NO_NOTE)
    {
        if (!note_is_on[0])
        {
            maxVal = 0;
            if (SeqMod_2_Poti[2] <= SeqMod_2_Poti[3])
            {
                for (int i = 0; i < 12; i++)
                {
                    if (analogReadArray[i] > maxVal)
                    {
                        maxVal = analogReadArray[i];
                        maxValIndex = i;
                        thisOctave = random(cc23, cc24);
                    }
                }
            }

            if (SeqMod_2_Poti[2] > SeqMod_2_Poti[3])
            {
                for (int i = 11; i >= 0; i--)
                {
                    if (analogReadArray[i] > maxVal)
                    {
                        maxVal = analogReadArray[i];
                        maxValIndex = i;
                        thisOctave = random(cc24, cc23);
                    }
                }
            }

            Serial.print("octave:");
            Serial.println(thisOctave);
            note_is_on[0] = true;
            noteToPlay[0] = (maxValIndex) + (thisOctave * 12) + noteOffset[internal_clock_bar];
            byte Velo = velocity[clip_to_play[internal_clock_bar]][cloock][0];
            noteOn(noteToPlay[0], Velo, MIDI_channel_out);
            // Serial.print(track[i].notePlayed[0]);
            // Serial.print("--");

            analogReadArray[maxValIndex] = (analogReadArray[maxValIndex] - SeqMod_2_Poti[0]);
            // Serial.println(maxValIndex);
        }
    }
    // NoteOff
    if (array[clip_to_play[internal_clock_bar]][cloock][0] == NO_NOTE)
    {
        if (note_is_on[0])
        {

            note_is_on[0] = false;
            noteOff(noteToPlay[0], 0, MIDI_channel_out);
            // Serial.println(track[i].notePlayed[0]);
        }
    }

    if (analogReadArray[maxValIndex] <= SeqMod_2_Poti[1])
    {
        for (int i = 0; i < 12; i++)
        {
            analogReadArray[i] = SeqMod_2_Poti[i + 4];
        }
    }
}
void Track::set_SeqMode2_parameters(byte row)
{
    draw_SeqMode2();
    if (row == 0)
    {
        set_SeqMode2_value(0, 0, "Drop", 0, 0);
        set_SeqMode2_value(1, 0, "Rst @", 0, 0);
        set_SeqMode2_value(2, 0, "Oct -", 0, 0);
        set_SeqMode2_value(3, 0, "Oct +", 0, 0);
    }
    if (row == 1)
    {
        set_SeqMode2_value(0, 1, "C", 0, 0);
        set_SeqMode2_value(1, 1, "C#", 0, 0);
        set_SeqMode2_value(2, 1, "D", 0, 0);
        set_SeqMode2_value(3, 1, "D#", 0, 0);
    }
    if (row == 2)
    {
        set_SeqMode2_value(0, 2, "E", 0, 0);
        set_SeqMode2_value(1, 2, "F", 0, 0);
        set_SeqMode2_value(2, 2, "F#", 0, 0);
        set_SeqMode2_value(3, 2, "G", 0, 0);
    }
    if (row == 3)
    {
        set_SeqMode2_value(0, 3, "G#", 0, 0);
        set_SeqMode2_value(1, 3, "A", 0, 0);
        set_SeqMode2_value(2, 3, "A#", 0, 0);
        set_SeqMode2_value(3, 3, "B", 0, 0);
    }
}
void Track::set_SeqMode2_value(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod_2_Poti[n] = constrain(SeqMod_2_Poti[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod_2_Poti[n], name);
    }
}
void Track::draw_SeqMode2()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod_2_Poti[0], "Drop");
        drawPot(1, 0, SeqMod_2_Poti[1], "Rst @");
        drawPot(2, 0, SeqMod_2_Poti[2], "Oct -");
        drawPot(3, 0, SeqMod_2_Poti[3], "Oct +");

        drawPot(0, 1, SeqMod_2_Poti[4], "C");
        drawPot(1, 1, SeqMod_2_Poti[5], "C#");
        drawPot(2, 1, SeqMod_2_Poti[6], "D");
        drawPot(3, 1, SeqMod_2_Poti[7], "D#");

        drawPot(0, 2, SeqMod_2_Poti[8], "E");
        drawPot(1, 2, SeqMod_2_Poti[9], "F");
        drawPot(2, 2, SeqMod_2_Poti[10], "F#");
        drawPot(3, 2, SeqMod_2_Poti[11], "G");

        drawPot(0, 3, SeqMod_2_Poti[12], "G#");
        drawPot(1, 3, SeqMod_2_Poti[13], "A");
        drawPot(2, 3, SeqMod_2_Poti[14], "A#");
        drawPot(3, 3, SeqMod_2_Poti[15], "B");
    }
}

void Track::play_SeqMode3(byte cloock)
{
    byte seq3_clock = cloock / 6;
    if (seq3_clock == 8)
        seq3_clock = 0;
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (bitRead(SeqMod_3_Poti[v], seq3_clock))
        {
            if (!note_is_on[v])
            {
                noteToPlay[v] = v + (octave * 12) + noteOffset[internal_clock_bar];
                byte Velo = velocity[clip_to_play[internal_clock_bar]][cloock][v];
                note_is_on[v] = true;
                noteOn(noteToPlay[v], Velo, MIDI_channel_out); // Send a Note (pitch 42, velo 127 on channel 1)
                                                               // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }

        if (!bitRead(SeqMod_3_Poti[v], seq3_clock))
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                // Masterout->noteOff(noteToPlay[v], VELOCITY_NOTE_ON, MIDI_channel_out, v); // Send a Note (pitch 42, velo 127 on channel 1)
                //  Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }
    }
}
void Track::set_SeqMode3_parameters(byte row)
{
    draw_SeqMode3();
    if (row == 0)
    {
        set_SeqMode3_value(0, 0, noteNames[0], 0, 0);
        set_SeqMode3_value(1, 0, noteNames[1], 0, 0);
        set_SeqMode3_value(2, 0, noteNames[2], 0, 0);
        set_SeqMode3_value(3, 0, noteNames[3], 0, 0);

        set_SeqMode3_value(0, 1, noteNames[4], 0, 0);
        set_SeqMode3_value(1, 1, noteNames[5], 0, 0);
        set_SeqMode3_value(2, 1, noteNames[6], 0, 0);
        set_SeqMode3_value(3, 1, noteNames[7], 0, 0);

        set_SeqMode3_value(0, 2, noteNames[8], 0, 0);
        set_SeqMode3_value(1, 2, noteNames[9], 0, 0);
        set_SeqMode3_value(2, 2, noteNames[10], 0, 0);
        set_SeqMode3_value(3, 2, noteNames[11], 0, 0);
    }
}
void Track::set_SeqMode3_value(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod_3_Poti[n] = constrain(SeqMod_3_Poti[n] + encoded[XPos], 0, MIDI_CC_RANGE * 2);
        drawPot(XPos, YPos, SeqMod_3_Poti[n], name);
    }
}
void Track::draw_SeqMode3()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod_3_Poti[0], noteNames[0]);
        drawPot(1, 0, SeqMod_3_Poti[1], noteNames[1]);
        drawPot(2, 0, SeqMod_3_Poti[2], noteNames[2]);
        drawPot(3, 0, SeqMod_3_Poti[3], noteNames[3]);

        drawPot(0, 1, SeqMod_3_Poti[4], noteNames[4]);
        drawPot(1, 1, SeqMod_3_Poti[5], noteNames[5]);
        drawPot(2, 1, SeqMod_3_Poti[6], noteNames[6]);
        drawPot(3, 1, SeqMod_3_Poti[7], noteNames[7]);

        drawPot(0, 2, SeqMod_3_Poti[8], noteNames[8]);
        drawPot(1, 2, SeqMod_3_Poti[9], noteNames[9]);
        drawPot(2, 2, SeqMod_3_Poti[10], noteNames[10]);
        drawPot(3, 2, SeqMod_3_Poti[11], noteNames[11]);
    }
}