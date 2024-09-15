#include <Arduino.h>
#include "project_functions.h"

// #include <fx_List.h>
#include <FX/Output.h>
#include <ownLibs/myClock.h>
#include "hardware/trellis_main.h"
extern Output MasterOut;
void draw_mixer();
void set_mixer(byte row);
void set_mixer_gain(byte XPos, byte YPos, const char *name, byte trackn);
void draw_mixer_FX_page1();
void draw_mixer_FX_page2();
void set_mixer_FX_page1(byte row);
void set_mixer_FX_page2(byte row);
void set_mixer_dry(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX1(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX2(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX3(byte XPos, byte YPos, const char *name, byte trackn);
void neotrellis_assign_start_buffer();
void neotrellis_start_clock();
void neotrellis_stop_clock();
void neotrellis_set_potRow();
void neotrellis_SetCursor(byte maxY);




void encoder_SetCursor(byte deltaX, byte maxY)

{

    if (enc_moved[0])
    {
        pixelTouchX = constrain(pixelTouchX + encoded[0] * deltaX, 0, 304);
        // Serial.printf("encoder: %d + deltaX: %d = pixelTouchX %d\n", encoded[0], deltaX, pixelTouchX);
        // enc_moved[0] = false;
    }
    if (enc_moved[1])
    {
        gridTouchY = constrain(gridTouchY + encoded[1], 0, maxY);
        // enc_moved[1] = false;
    }
}

void clock_to_notes(int _tick)
{

    // Serial.println(Masterclock.MIDItick);
    for (int t = 0; t < NUM_TRACKS; t++)
    {
        allTracks[t]->play_sequencer_mode(_tick, myClock.startOfLoop, myClock.endOfLoop);
    }
    // Serial.printf("LoopStart: %d, LoopEnd: %d\n", myClock.startOfLoop, myClock.endOfLoop);
}
void input_behaviour()
{

    neotrellis_start_clock();
    neotrellis_stop_clock();
    neotrellis_set_potRow();
    // if we are in one of the sequencer pages
    if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
       
        neotrellis_SetCursor(14);

        if (neotrellisPressed[TRELLIS_POTROW])
        {
            change_plugin_row = true;
            mytft->draw_stepSequencer_parameters(lastPotRow);
            neotrellisPressed[TRELLIS_POTROW] = false;
        }
        allTracks[active_track]->set_stepSequencer_parameters(lastPotRow);
    }
    // if we are in one of the Arrangerpages
    if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
    {
        if (neotrellisPressed[TRELLIS_POTROW])
        {
            change_plugin_row = true;
            mytft->draw_arranger_parameters(lastPotRow);
            neotrellisPressed[TRELLIS_POTROW] = false;
        }

        neotrellis_SetCursor(8);

        switch (lastPotRow)
        {
        case 0:
            // gridTouchY = 0;
            encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
            allTracks[gridTouchY - 1]->set_clip_to_play(2, pixelTouchX);
            allTracks[gridTouchY - 1]->set_note_offset(3, pixelTouchX);
            break;
        case 1:
            allTracks[gridTouchY - 1]->set_barVelocity(0, pixelTouchX);
            myClock.set_tempo(1);
            myClock.set_start_of_loop(2); // Encoder: 2
            myClock.set_end_of_loop(3);   // Encoder: 3
            break;
        case 2:
            allTracks[gridTouchY - 1]->set_play_presetNr_ccChannel(2, 2);
            allTracks[gridTouchY - 1]->set_play_presetNr_ccValue(3, 2);
            break;
        case 3:
        default:
            break;
        }
    }
    if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER_MODES)
    {
        if (neotrellisPressed[TRELLIS_POTROW])
        {
            tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
            neotrellisPressed[TRELLIS_POTROW] = false;
        }
        // if Shift button is NOT pressed
        if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        {
            /* for (int i = 0; i < NUM_TRACKS; i++)
             {
                 allTracks[i]->set_seq_mode_parameters(lastPotRow);
             }*/
            allTracks[active_track]->set_seq_mode_parameters(lastPotRow);
        }
    }
    if (activeScreen == INPUT_FUNCTIONS_FOR_PLUGIN)
    {

        if (allTracks[active_track]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
            allTracks[active_track]->set_MIDI_CC(lastPotRow);
        else if (allTracks[active_track]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
            MasterOut.set_parameters(allTracks[active_track]->parameter[SET_MIDICH_OUT] - 49, lastPotRow);
        neotrellisPressed[TRELLIS_POTROW] = false;
    }
    if (activeScreen == INPUT_FUNCTIONS_FOR_MIXER1)
        set_mixer(lastPotRow);
    if (activeScreen == INPUT_FUNCTIONS_FOR_MIXER2)
        set_mixer_FX_page1(lastPotRow);
    if (activeScreen == INPUT_FUNCTIONS_FOR_MIXER3)
        set_mixer_FX_page2(lastPotRow);
    if (activeScreen == INPUT_FUNCTIONS_FOR_FX1)
        fx_1.set_parameters(lastPotRow);
    if (activeScreen == INPUT_FUNCTIONS_FOR_FX2)
        fx_2.set_parameters(lastPotRow);
    //if (activeScreen == INPUT_FUNCTIONS_FOR_PERFORM)
       // trellisMain->set_perform_page(lastPotRow);
}



