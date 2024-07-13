#include <Arduino.h>
#include "project_functions.h"

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
void buttons_SetCursor(byte maxY)
{

    if (buttonPressed[BUTTON_LEFT]) //..move the cursor left
    {
        pixelTouchX = constrain(pixelTouchX - 1, 0, 320);
        buttonPressed[BUTTON_LEFT] = false;
    }
    if (buttonPressed[BUTTON_RIGHT]) //..move the cursor right
    {
        pixelTouchX = constrain(pixelTouchX + 1, 0, 320);
        buttonPressed[BUTTON_RIGHT] = false;
    }
    if (buttonPressed[BUTTON_UP]) //..move the cursor up
    {
        gridTouchY = constrain(gridTouchY - 1, 0, maxY);
        buttonPressed[2] = false;
    }

    if (buttonPressed[BUTTON_DOWN]) //..move the cursor down
    {
        gridTouchY = constrain(gridTouchY + 1, 0, maxY);
        buttonPressed[BUTTON_DOWN] = false;
    }
}
void buttons_SetPlayStatus()
{
    if (buttonPressed[BUTTON_PLAY]) //..set Play status to play
    {
        Masterclock.setPlayStatus(true);
        Serial.println("Play");
        buttonPressed[BUTTON_PLAY] = false;
    }
    if (buttonPressed[BUTTON_STOP]) //..set Play status to stop
    {
        Masterclock.setPlayStatus(false);
        for (int i = 0; i < NUM_TRACKS; i++)
        {
            allTracks[i]->internal_clock = 0;
            allTracks[i]->internal_clock_bar = 0;
        }

        Serial.println("Stop");
        buttonPressed[BUTTON_STOP] = false;
    }
}

void buttons_SelectTrack()
{
    // select tracks
    if (buttonPressed[BUTTON_TRACK])
    {
        for (int i = 0; i <= TRACK_8_PAGE; i++)
        {
            if (buttonPressed[i])
            {
                active_track = i;
                show_active_track();
                encoder_function = INPUT_FUNCTIONS_FOR_SEQUENCER;
                change_plugin_row = true;
                allTracks[active_track]->drawStepSequencerStatic();
                allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
                allTracks[active_track]->draw_notes_in_grid();
                buttonPressed[i] = false;
            }
        }
    }
}
void buttons_SelectSequencerMode()
{ // select active plugin from choosen track
    if (buttonPressed[BUTTON_FX])
    {
        for (int i = 0; i < BUTTONS_PER_ROW; i++)
        {
            if (buttonPressed[i])
            {
                show_active_track();
                allTracks[i]->draw_sequencer_modes(allTracks[i]->parameter[SET_SEQ_MODE]);
                encoder_function = INPUT_FUNCTIONS_FOR_SEQUENCER_MODES;
                Serial.println("SeqMode selected");
                // buttonPressed[BUTTON_PLUGIN] = false;
                buttonPressed[i] = false;
            }
        }
    }
}
void buttons_SelectPlugin()
{ // select active plugin from choosen track
    if (buttonPressed[BUTTON_PLUGIN])
    {
        for (int i = 0; i < BUTTONS_PER_ROW; i++)
        {
            if (buttonPressed[i])
            {
                active_track = i;
                show_active_track();
                change_plugin_row = true;
                allTracks[active_track]->draw_MIDI_CC_screen();
                encoder_function = INPUT_FUNCTIONS_FOR_PLUGIN;
                // Serial.printf("plugin selected Track: %d on channel: %d\n", i,allTracks[i]->parameter[SET_MIDICH_OUT] );
                //  buttonPressed[BUTTON_PLUGIN] = false;
                buttonPressed[i] = false;
            }
        }
    }
}
void buttons_save_track()
{ //  save track stuff fromSD
    if (buttonPressed[BUTTON_ENTER])
    {
        if (pixelTouchX >= 13 * STEP_FRAME_W && pixelTouchX <= 14 * STEP_FRAME_W && gridTouchY == 0)
        {
            allTracks[active_track]->save_track();
            // Serial.println("saved track");
            buttonPressed[BUTTON_ENTER] = false;
        }
    }
}
void buttons_load_track()
{ // load track stuff fromSD
    if (buttonPressed[BUTTON_ENTER])
    {
        if (pixelTouchX >= 15 * STEP_FRAME_W && gridTouchY == 0)
        {

            allTracks[active_track]->load_track();
            // Serial.println("loaded track");
            buttonPressed[BUTTON_ENTER] = false;
        }
    }
}
void buttons_set_track_recordState()
{
    if (buttonPressed[BUTTON_REC])
    {
        if (!allTracks[active_track]->get_recordState())
            allTracks[active_track]->set_recordState(true);
        else
            allTracks[active_track]->set_recordState(false);
        buttonPressed[BUTTON_REC] = false;
    }
}

void buttons_save_all()
{ //  save track stuff fromSD
    if (buttonPressed[BUTTON_ENTER])
    {
        if (pixelTouchX >= 13 * STEP_FRAME_W && pixelTouchX <= 14 * STEP_FRAME_W && gridTouchY == 0)
        {
            for (int i = 0; i < NUM_TRACKS; i++)
                allTracks[i]->save_track();
            // Serial.println("saved track");
            buttonPressed[BUTTON_ENTER] = false;
        }
    }
}
void buttons_load_all()
{ // load track stuff fromSD
    if (buttonPressed[BUTTON_ENTER])
    {
        if (pixelTouchX >= 15 * STEP_FRAME_W && gridTouchY == 0)
        {
            for (int i = 0; i < NUM_TRACKS; i++)
                allTracks[i]->load_track();
            // Serial.println("loaded track");
            buttonPressed[BUTTON_ENTER] = false;
        }
    }
}
void buttons_SelectArranger()
{
    if (buttonPressed[BUTTON_SONG])
    {
        encoder_function = INPUT_FUNCTIONS_FOR_ARRANGER;
        unsigned long currentTime = millis();
        if (currentTime - buttonPressStartTime[BUTTON_SONG] >= longPressDuration)
        {

            arrangerpage = SONGMODE_PAGE_11;
            gridSongMode(arrangerpage);
        }
        else if (buttonPressed[BUTTON_LEFT])
        {
            arrangerpage = SONGMODE_PAGE_1;
            buttonPressed[BUTTON_LEFT] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_RIGHT])
        {
            arrangerpage = SONGMODE_PAGE_2;
            buttonPressed[BUTTON_RIGHT] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_UP])
        {
            arrangerpage = SONGMODE_PAGE_3;
            buttonPressed[BUTTON_UP] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_DOWN])
        {
            arrangerpage = SONGMODE_PAGE_4;
            buttonPressed[BUTTON_DOWN] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_ROW])
        {
            arrangerpage = SONGMODE_PAGE_5;
            buttonPressed[BUTTON_ROW] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_REC])
        {
            arrangerpage = SONGMODE_PAGE_6;
            buttonPressed[BUTTON_REC] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_PLAY])
        {
            arrangerpage = SONGMODE_PAGE_7;
            buttonPressed[BUTTON_PLAY] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_STOP])
        {
            arrangerpage = SONGMODE_PAGE_8;
            buttonPressed[BUTTON_STOP] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        if (buttonPressed[BUTTON_TRACK])
        {
            arrangerpage = SONGMODE_PAGE_9;
            buttonPressed[BUTTON_TRACK] = false;
            buttonPressed[BUTTON_SONG] = false;
            gridSongMode(arrangerpage);
        }
        show_active_songpage();
        Serial.println(arrangerpage);
    }
}
void buttons_Set_potRow()
{
    if (buttonPressed[BUTTON_ROW])
    {
        change_plugin_row = true;
        lastPotRow++;
        if (lastPotRow >= 4)
        {
            lastPotRow = 0;
        }
        tft.fillRect(70, 0, 10, 16, ILI9341_DARKGREY);
        // buttonPressed[BUTTON_ROW] = false;
        Serial.printf("potrwo=%d\n", lastPotRow);
    }
}
void buttons_SetNoteOnTick(int x, byte y)
{
    if (pixelTouchX >= SEQ_GRID_LEFT && pixelTouchX <= SEQ_GRID_RIGHT && gridTouchY >= SEQ_GRID_TOP && gridTouchY <= SEQ_GRID_BOTTOM)
    {
        if (buttonPressed[BUTTON_ENTER])
        {

            allTracks[active_track]->set_note_on_tick();
            buttonPressed[BUTTON_ENTER] = false;
        }
    }
}
void clock_to_notes()
{
    if (Masterclock.is_playing())
    {
        if (Masterclock.clock_is_on_tick)
        {
            // Serial.println(Masterclock.MIDItick);
            for (int t = 0; t < NUM_TRACKS; t++)
            {
                allTracks[t]->play_sequencer_mode(Masterclock.MIDItick, Masterclock.start_of_loop, Masterclock.end_of_loop);
            }
        }
    }
}
void input_behaviour()
{
    otherCtrlButtons = (buttonPressed[BUTTON_TRACK] ||
                        buttonPressed[BUTTON_PLUGIN] ||
                        buttonPressed[BUTTON_SONG] ||
                        buttonPressed[BUTTON_MIXER] ||
                        buttonPressed[BUTTON_FX]);

    buttons_SelectTrack();
    buttons_SelectArranger();
    buttons_SelectSequencerMode();
    buttons_SelectPlugin();
    // if none of the bottom buttons are pressed with the upper ones
    if (!otherCtrlButtons)
    {

        buttons_Set_potRow();
        buttons_SetPlayStatus();
    }
    // if we are in one of the sequencer pages
    if (encoder_function == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
        buttons_save_track();
        buttons_load_track();
        buttons_set_track_recordState();
        buttons_SetCursor(14);
        buttons_SetNoteOnTick(pixelTouchX, gridTouchY);
         
        if (buttonPressed[BUTTON_ROW])
        {
            change_plugin_row = true;

            allTracks[active_track]->draw_stepSequencer_parameters(lastPotRow);
            buttonPressed[BUTTON_ROW] = false;
        }
        allTracks[active_track]->set_stepSequencer_parameters(lastPotRow);
    }
    // if we are in one of the Arrangerpages
    if (encoder_function == INPUT_FUNCTIONS_FOR_ARRANGER)
    {
        if (buttonPressed[BUTTON_ROW])
        {
            change_plugin_row = true;
            allTracks[active_track]->draw_arranger_parameters(lastPotRow);
            buttonPressed[BUTTON_ROW] = false;
        }
        buttons_save_all();
        buttons_load_all();
        buttons_SetCursor(8);

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
            Masterclock.set_tempo(1);         // Encoder: 1
            Masterclock.set_start_of_loop(2); // Encoder: 2
            Masterclock.set_end_of_loop(3);   // Encoder: 3
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
    if (encoder_function == INPUT_FUNCTIONS_FOR_SEQUENCER_MODES)
    {
        if (buttonPressed[BUTTON_ROW])
        {
            tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
            buttonPressed[BUTTON_ROW] = false;
        }
        // if Shift button is NOT pressed
        if (!buttonPressed[BUTTON_SHIFT])
        {
            /* for (int i = 0; i < NUM_TRACKS; i++)
             {
                 allTracks[i]->set_seq_mode_parameters(lastPotRow);
             }*/
            allTracks[active_track]->set_seq_mode_parameters(lastPotRow);
        }
    }
    if (encoder_function == INPUT_FUNCTIONS_FOR_PLUGIN)
    {
        allTracks[active_track]->set_MIDI_CC(lastPotRow);
        buttonPressed[BUTTON_ROW] = false;
    }
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
void draw_Value(byte index, byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int value, int color, bool drawRect, bool drawFilling)
{

    int xPos = XPos * STEP_FRAME_W;
    byte yPos = YPos * STEP_FRAME_H;

    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_DARKGREY);
    tft.setCursor(xPos + offest_X, yPos + offset_Y);
    tft.fillRect(xPos, yPos, 2 * STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
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
    tft.fillRect(xPos, yPos, 2 * STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
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