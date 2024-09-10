#include <Arduino.h>
#include "project_functions.h"

// #include <fx_List.h>
#include <FX/Output.h>
#include <ownLibs/myClock.h>
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
void neotrellis_static();
void trellis_start_clock();
void trellis_stop_clock();
void trellis_set_potRow();
void trellis_SetCursor(byte maxY);
void set_perform_page(byte lastPotRow);



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

void buttons_save_track()
{ //  save track stuff fromSD
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        if (pixelTouchX >= 13 * STEP_FRAME_W && pixelTouchX <= 14 * STEP_FRAME_W && gridTouchY == 0)
        {
            // allTracks[active_track]->save_track();
            //  Serial.println("saved track");
            neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        }
    }
}
void buttons_load_track()
{ // load track stuff fromSD
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        if (pixelTouchX >= 15 * STEP_FRAME_W && gridTouchY == 0)
        {

            // allTracks[active_track]->load_track();
            //  Serial.println("loaded track");
            neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        }
    }
}

void buttons_save_all()
{ //  save track stuff fromSD
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        if (pixelTouchX >= 13 * STEP_FRAME_W && pixelTouchX <= 14 * STEP_FRAME_W && gridTouchY == 0)
        {
            for (int i = 0; i < NUM_TRACKS; i++)
                //  allTracks[i]->save_track();
                // Serial.println("saved track");
                neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        }
    }
}
void buttons_load_all()
{ // load track stuff fromSD
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        if (pixelTouchX >= 15 * STEP_FRAME_W && gridTouchY == 0)
        {
            for (int i = 0; i < NUM_TRACKS; i++)
                // allTracks[i]->load_track();
                // Serial.println("loaded track");
                neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        }
    }
}

void buttons_SetNoteOnTick(int x, byte y)
{
    if (pixelTouchX >= SEQ_GRID_LEFT && pixelTouchX <= SEQ_GRID_RIGHT && gridTouchY >= SEQ_GRID_TOP && gridTouchY <= SEQ_GRID_BOTTOM)
    {
        if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
        {

            allTracks[active_track]->set_note_on_tick((pixelTouchX - SEQ_GRID_LEFT) / 2, gridTouchY);

            neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        }
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

    trellis_start_clock();
    trellis_stop_clock();
    trellis_set_potRow();
    // if we are in one of the sequencer pages
    if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
        buttons_save_track();
        buttons_load_track();
        trellis_SetCursor(14);
        buttons_SetNoteOnTick(pixelTouchX, gridTouchY);

        if (neotrellisPressed[TRELLIS_POTROW])
        {
            change_plugin_row = true;
            allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
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
            allTracks[active_track]->draw_arranger_parameters(lastPotRow);
            neotrellisPressed[TRELLIS_POTROW] = false;
        }

        trellis_SetCursor(8);

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
    if (activeScreen == INPUT_FUNCTIONS_FOR_PERFORM)
        set_perform_page(lastPotRow);
}

void clearWorkSpace()
{                                                                                                         // clear the whole grid from Display
    tft.fillRect(STEP_FRAME_W, STEP_FRAME_H, STEP_FRAME_W * 21, STEP_FRAME_H * 13 + 4, ILI9341_DARKGREY); // Xmin, Ymin, Xlength, Ylength, color
                                                                                                          // tft->fillRect(STEP_FRAME_W, STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H * 12, ILI9341_DARKGREY);
}
void drawPot(int XPos, byte YPos, int dvalue, const char *dname)
{
    enc_moved[XPos] = false;
    // xposition, yposition, value 1-100, value to draw, name to draw, color
    // drawPot Variables
    static float circlePos[4];
    static float circlePos_old[4];
    static const char *dname_old[4];
    // byte fvalue = map(dvalue, 0, 127, min, max);
    int xPos;
    int color;
    if (XPos == 0)
    {
        xPos = 3;
        color = ILI9341_BLUE;
    }
    if (XPos == 1)
    {
        xPos = 7;
        color = ILI9341_RED;
    }
    if (XPos == 2)
    {
        xPos = 11;
        color = ILI9341_GREEN;
    }
    if (XPos == 3)
    {
        xPos = 15;
        color = ILI9341_WHITE;
    }
    if (YPos != lastPotRow)
        color = ILI9341_LIGHTGREY;

    int yPos = (YPos + 1) * 3;

    circlePos[XPos] = dvalue / 63.5;

    draw_Value(XPos, YPos, xPos, yPos, 4, -3, dvalue, color, false, false);
    draw_Text(XPos, YPos, xPos, yPos + 1, 0, 3, dname, color, false, false);

    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_DARKGREY);
    tft.setCursor(STEP_FRAME_W * xPos, STEP_FRAME_H * (yPos + 1) + 3);
    tft.print(dname_old[XPos]);

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(STEP_FRAME_W * xPos, STEP_FRAME_H * (yPos + 1) + 3);
    tft.print(dname);

    tft.fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos_old[XPos]) + 2.25), STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos_old[XPos]) + 2.25), 4, ILI9341_DARKGREY);
    tft.drawCircle(STEP_FRAME_W * (xPos + 1), STEP_FRAME_H * yPos, 16, ILI9341_LIGHTGREY);
    tft.fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos[XPos]) + 2.25), STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos[XPos]) + 2.25), 4, color);
    circlePos_old[XPos] = circlePos[XPos];

    dname_old[XPos] = dname;
}

void drawEnvelope(byte YPos, byte attack, byte decay, byte sustain, byte release)
{
    // int yPos;
    int colorA = ILI9341_BLUE;
    int colorD = ILI9341_RED;
    int colorS = ILI9341_GREEN;
    int colorR = ILI9341_WHITE;

    if (YPos != lastPotRow)
    {
        colorA = ILI9341_LIGHTGREY;
        colorD = ILI9341_LIGHTGREY;
        colorS = ILI9341_LIGHTGREY;
        colorR = ILI9341_LIGHTGREY;
    }
    byte ypos = ((YPos + 1) * 3);
    int yPos = (ypos + 1) * STEP_FRAME_H;
    byte envStart = 48;
    byte envTop = yPos - 32;

    static byte old_attackEnd;
    static byte old_decayEnd;
    static byte old_sustainLevel;
    static byte old_sustainEnd;
    static byte old_releaseEnd;

    tft.drawLine(envStart, yPos, old_attackEnd, envTop, ILI9341_DARKGREY);
    tft.drawLine(old_attackEnd, envTop, old_decayEnd + old_attackEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft.drawLine(old_decayEnd + old_attackEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft.drawLine(old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd + old_releaseEnd, yPos, ILI9341_DARKGREY);

    byte attackEnd = map(attack, 0, 127, 0, 50) + envStart;
    byte decayEnd = map(decay, 0, 127, 0, 30);
    byte sustainLevel = yPos - map(sustain, 0, 127, 0, 32);
    byte sustainEnd = 30;
    byte releaseEnd = map(release, 0, 127, 0, 50);

    tft.drawLine(envStart, yPos, attackEnd, envTop, colorA);
    tft.drawLine(attackEnd, envTop, decayEnd + attackEnd, sustainLevel, colorD);
    tft.drawLine(decayEnd + attackEnd, sustainLevel, decayEnd + attackEnd + sustainEnd, sustainLevel, colorS);
    tft.drawLine(decayEnd + attackEnd + sustainEnd, sustainLevel, decayEnd + attackEnd + sustainEnd + releaseEnd, yPos, colorR);

    draw_Value(0, YPos, 14, ypos - 1, 4, 4, attack, colorA, true, false);
    draw_Value(1, YPos, 16, ypos - 1, 4, 4, decay, colorD, true, false);
    draw_Value(2, YPos, 14, ypos, 4, 4, sustain, colorS, true, false);
    draw_Value(3, YPos, 16, ypos, 4, 4, release, colorR, true, false);

    old_attackEnd = attackEnd;
    old_decayEnd = decayEnd;
    old_sustainLevel = sustainLevel;
    old_sustainEnd = sustainEnd;
    old_releaseEnd = releaseEnd;
}
void draw_Value(byte index, byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int value, int color, bool drawRect, bool drawFilling)
{

    int xPos = XPos * STEP_FRAME_W;
    byte yPos = YPos * STEP_FRAME_H;

    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_DARKGREY);
    tft.setCursor(xPos + offest_X, yPos + offset_Y);
    tft.fillRect(xPos + offest_X, yPos + offset_Y, 2 * STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
    // tft.print(old_value[index]);
    if (lastPotRow != lastPRow)
        color = ILI9341_LIGHTGREY;
    if (drawRect)
        tft.drawRect(xPos, yPos, 2 * STEP_FRAME_W, STEP_FRAME_H, color);
    if (drawFilling)
        tft.fillRect(xPos, yPos, 2 * STEP_FRAME_W, STEP_FRAME_H, color);

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(xPos + offest_X, yPos + offset_Y);
    tft.print(value);
}
void draw_Text(byte index, byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, const char *name, int color, bool drawRect, bool drawFilling)
{

    int xPos = XPos * STEP_FRAME_W;
    byte yPos = YPos * STEP_FRAME_H;

    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_DARKGREY);
    tft.setCursor(xPos + offest_X, yPos + offset_Y);
    tft.fillRect(xPos + offest_X, yPos + offset_Y, 2 * STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
    // tft.print(old_value[index]);
    if (lastPotRow != lastPRow)
        color = ILI9341_LIGHTGREY;
    if (drawRect)
        tft.drawRect(xPos, yPos, 2 * STEP_FRAME_W, STEP_FRAME_H, color);
    if (drawFilling)
        tft.fillRect(xPos, yPos, 2 * STEP_FRAME_W, STEP_FRAME_H, color);

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(xPos + offest_X, yPos + offset_Y);
    tft.print(name);
   
}
void show_active_track()
{
    draw_Text(3, 0, 1, 0, 2, 3, "Trk:", ILI9341_WHITE, false, false);
    draw_Value(3, 0, 2, 0, 6, 3, active_track + 1, ILI9341_WHITE, false, false);
}
void show_active_songpage()
{
    draw_Text(3, 0, 1, 0, 2, 3, "Page:", ILI9341_WHITE, false, false);
    draw_Value(3, 0, 3, 0, 0, 3, arrangerpage + 1, ILI9341_WHITE, false, false);
}
void drawsongmodepageselector()
{
    // draw 16 rects of 16x16px in the 13th row
    for (int pages = 2; pages < 18; pages++)
    {
        // drawActiveRect(pages, 13, 1, 1, selectPage == pages + 8, "", ILI9341_LIGHTGREY);
        tft.drawRect(STEP_FRAME_W * pages, STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
        tft.setFont(Arial_8);
        tft.setTextColor(ILI9341_WHITE);
        tft.setCursor(STEP_FRAME_W * pages + 3, STEP_FRAME_H * 13 + 8);
        tft.print((pages - 1));
    }
}
void gridSongMode(int songpageNumber)
{ // static Display rendering
    // int page_phrase_start = songpageNumber * 16;
    // int page_phrase_end = (songpageNumber + 1) * 16;
    clearWorkSpace();
    drawsongmodepageselector();
    // drawActiveRect(18, 3, 2, 2, false, "clear", ILI9341_RED);

    // vertical pointer Lines
    int shownLines = 257 / phraseSegmentLength;
    for (int f = 0; f < shownLines; f++)
    {                                                                                                // do this for all phrases
        tft.drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 360); //(x, y-start, y-length, color)
        if (f % 4 == 0)
        {
            tft.drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 370); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < NUM_TRACKS; i++)
        allTracks[i]->draw_arrangment_lines(3, arrangerpage);
    change_plugin_row = true;
    allTracks[active_track]->draw_arranger_parameters(lastPotRow);
    show_active_songpage();
}

void drawActiveRect(int xPos, byte yPos, byte xsize, byte ysize, bool state, const char *name, int color)
{
    if (state)
    {
        tft.fillRect(STEP_FRAME_W * xPos, STEP_FRAME_H * yPos, STEP_FRAME_W * xsize, STEP_FRAME_W * ysize, color);
        tft.drawRect(STEP_FRAME_W * xPos, STEP_FRAME_H * yPos, STEP_FRAME_W * xsize, STEP_FRAME_W * ysize, color);
        tft.setFont(Arial_8);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(STEP_FRAME_W * xPos + 4, STEP_FRAME_H * yPos + 3);
        tft.print(name);
    }
    if (!state)
    {
        tft.fillRect(STEP_FRAME_W * xPos, STEP_FRAME_H * yPos, STEP_FRAME_W * xsize, STEP_FRAME_W * ysize, ILI9341_DARKGREY);
        tft.drawRect(STEP_FRAME_W * xPos, STEP_FRAME_H * yPos, STEP_FRAME_W * xsize, STEP_FRAME_W * ysize, color);
        tft.setFont(Arial_8);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(STEP_FRAME_W * xPos + 4, STEP_FRAME_H * yPos + 3);
        tft.print(name);
    }
}
void draw_sequencer_option(byte x, const char *nameshort, int value, byte enc, const char *pluginName)
{

    int color;
    byte y = 6 + (enc * 2);
    if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
        color = y - 1;
    else
        color = active_track;

    // show function
    tft.setCursor(STEP_FRAME_W * x + 2, STEP_FRAME_H * (y - 1) + 6);
    tft.setFont(Arial_8);
    tft.setTextColor(trackColor[color]);
    tft.setTextSize(1);
    tft.print(nameshort);
    // show value
    tft.drawRect(STEP_FRAME_W * x, STEP_FRAME_H * y, STEP_FRAME_W * 2, STEP_FRAME_H, encoder_colour[enc]);
    tft.fillRect(STEP_FRAME_W * x + 1, STEP_FRAME_H * y + 1, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 2, ILI9341_DARKGREY);
    tft.setCursor(STEP_FRAME_W * x + 8, STEP_FRAME_H * y + 3);
    tft.setFont(Arial_10);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(1);
    if (pluginName != 0)
    {
        tft.setCursor(STEP_FRAME_W * x + 2, STEP_FRAME_H * y + 4);
        tft.setFont(Arial_8);
        tft.print(pluginName);
    }
    else
        tft.print(value);
}
void startUpScreen()
{

    // static Display rendering
    tft.fillScreen(ILI9341_DARKGREY);

    tft.setFont(Arial_9);

    // songmode button
    tft.setTextColor(ILI9341_BLACK);
    tft.fillRect(1, 1, 15, TRACK_FRAME_H, ILI9341_MAGENTA); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(4, 3);
    tft.print("S");

    // Drumtrack button
    tft.fillRect(1, STEP_FRAME_H + 8, 15, TRACK_FRAME_H, trackColor[0]); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(4, TRACK_FRAME_H + 8);
    tft.print("D");

    // other tracks buttons
    for (int otherTracks = 2; otherTracks <= 8; otherTracks++)
    {
        tft.fillRect(1, TRACK_FRAME_H * otherTracks, 15, TRACK_FRAME_H, trackColor[otherTracks - 1]); // Xmin, Ymin, Xlength, Ylength, color
        tft.setCursor(4, TRACK_FRAME_H * otherTracks + 6);
        tft.print(otherTracks);
    }
    // Mixer button
    tft.fillRect(1, STEP_FRAME_H * 13 + 8, 15, TRACK_FRAME_H, ILI9341_LIGHTGREY); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(3, STEP_FRAME_H * 13 + 14);
    tft.print("M");

    // scale Select
    tft.drawRect(STEP_FRAME_W * POSITION_SCALE_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // arrangment Select
    tft.drawRect(STEP_FRAME_W * POSITION_ARR_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // record button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_RECORD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillCircle(STEP_FRAME_W * POSITION_RECORD_BUTTON + 7, 7, 6, ILI9341_LIGHTGREY);

    // Play button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_PLAY_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);                                                                     // PLAY RECT FRAME
    tft.fillTriangle(STEP_FRAME_W * POSITION_PLAY_BUTTON + 12, 3, STEP_FRAME_W * POSITION_PLAY_BUTTON + 12, 13, STEP_FRAME_W * POSITION_PLAY_BUTTON + 22, 8, ILI9341_GREEN); // x1, y1, x2, y2, x3, y3

    // stop button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_STOP_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillRect(STEP_FRAME_W * POSITION_STOP_BUTTON + 3, 3, 10, 10, ILI9341_LIGHTGREY);

    // barcounter & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_BAR_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // tempo button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_BPM_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // save button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_SAVE_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillRect(STEP_FRAME_W * POSITION_SAVE_BUTTON + 1, 1, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 2, ILI9341_ORANGE);
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(Arial_9);
    tft.setCursor(STEP_FRAME_W * POSITION_SAVE_BUTTON + 2, 3);
    tft.print("SAV");

    // load button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_LOAD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillRect(STEP_FRAME_W * POSITION_LOAD_BUTTON + 1, 1, STEP_FRAME_W - 2, STEP_FRAME_H - 2, ILI9341_GREENYELLOW);
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(Arial_9);
    tft.setCursor(STEP_FRAME_W * POSITION_LOAD_BUTTON + 4, 3);
    tft.print("L");
}
