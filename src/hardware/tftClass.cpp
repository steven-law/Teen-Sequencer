#include "hardware/tftClass.h"

tftClass::tftClass(ILI9341_t3n *display)
{
    tft = display;
}

tftClass::~tftClass()
{
}

void tftClass::tft_setup(int dly)
{
    // initialize the TFT- and Touchscreen

    tft->begin();
    tft->setRotation(3);
    tft->fillScreen(ILI9341_BLACK);
    tft->setFrameBuffer(tft_frame_buffer);
    tft->useFrameBuffer(true);
    tft->initDMASettings();
    tft->updateScreenAsync();
    delay(dly);
    // tft->updateChangedAreasOnly(true);
    tft->setTextColor(ILI9341_WHITE);
    tft->setFont(Arial_8);
    tft->setCursor(0, 3);
    Serial.println("Initializing Screen");
    tft->println("Initializing Screen");
    tft->updateScreenAsync();
    delay(dly);
}
void tftClass::tftUpdate(int _pixelOnX, int _pixelOnY, byte _activeTrack, byte _activePage)
{
    pixelOnX = _pixelOnX;
    pixelOnY = _pixelOnY;
    activeTrack = _activeTrack;
    activePage = _activePage;
}

void tftClass::clearWorkSpace()
{                                                                                                          // clear the whole grid from Display
    tft->fillRect(STEP_FRAME_W, STEP_FRAME_H, STEP_FRAME_W * 21, STEP_FRAME_H * 13 + 4, ILI9341_DARKGREY); // Xmin, Ymin, Xlength, Ylength, color
                                                                                                           // tft->fillRect(STEP_FRAME_W, STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H * 12, ILI9341_DARKGREY);
}
void tftClass::startUpScreen()
{

    // static Display rendering
    tft->fillScreen(ILI9341_DARKGREY);

    tft->setFont(Arial_9);

    // songmode button
    tft->setTextColor(ILI9341_BLACK);
    tft->fillRect(1, 1, 15, TRACK_FRAME_H, ILI9341_MAGENTA); // Xmin, Ymin, Xlength, Ylength, color
    tft->setCursor(4, 3);
    tft->print("S");

    // Drumtrack button
    tft->fillRect(1, STEP_FRAME_H + 8, 15, TRACK_FRAME_H, trackColor[0]); // Xmin, Ymin, Xlength, Ylength, color
    tft->setCursor(4, TRACK_FRAME_H + 8);
    tft->print("D");

    // other tracks buttons
    for (int otherTracks = 2; otherTracks <= 8; otherTracks++)
    {
        tft->fillRect(1, TRACK_FRAME_H * otherTracks, 15, TRACK_FRAME_H, trackColor[otherTracks - 1]); // Xmin, Ymin, Xlength, Ylength, color
        tft->setCursor(4, TRACK_FRAME_H * otherTracks + 6);
        tft->print(otherTracks);
    }
    // Mixer button
    tft->fillRect(1, STEP_FRAME_H * 13 + 8, 15, TRACK_FRAME_H, ILI9341_LIGHTGREY); // Xmin, Ymin, Xlength, Ylength, color
    tft->setCursor(3, STEP_FRAME_H * 13 + 14);
    tft->print("M");

    // scale Select
    tft->drawRect(STEP_FRAME_W * POSITION_SCALE_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // arrangment Select
    tft->drawRect(STEP_FRAME_W * POSITION_ARR_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // record button & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_RECORD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft->fillCircle(STEP_FRAME_W * POSITION_RECORD_BUTTON + 7, 7, 6, ILI9341_LIGHTGREY);

    // Play button & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_PLAY_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);                                                                     // PLAY RECT FRAME
    tft->fillTriangle(STEP_FRAME_W * POSITION_PLAY_BUTTON + 12, 3, STEP_FRAME_W * POSITION_PLAY_BUTTON + 12, 13, STEP_FRAME_W * POSITION_PLAY_BUTTON + 22, 8, ILI9341_GREEN); // x1, y1, x2, y2, x3, y3

    // stop button & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_STOP_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft->fillRect(STEP_FRAME_W * POSITION_STOP_BUTTON + 3, 3, 10, 10, ILI9341_LIGHTGREY);

    // barcounter & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_BAR_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // tempo button & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_BPM_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // save button & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_SAVE_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);
    tft->fillRect(STEP_FRAME_W * POSITION_SAVE_BUTTON + 1, 1, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 2, ILI9341_ORANGE);
    tft->setTextColor(ILI9341_BLACK);
    tft->setFont(Arial_9);
    tft->setCursor(STEP_FRAME_W * POSITION_SAVE_BUTTON + 2, 3);
    tft->print("SAV");

    // load button & Rectangle
    tft->drawRect(STEP_FRAME_W * POSITION_LOAD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft->fillRect(STEP_FRAME_W * POSITION_LOAD_BUTTON + 1, 1, STEP_FRAME_W - 2, STEP_FRAME_H - 2, ILI9341_GREENYELLOW);
    tft->setTextColor(ILI9341_BLACK);
    tft->setFont(Arial_9);
    tft->setCursor(STEP_FRAME_W * POSITION_LOAD_BUTTON + 4, 3);
    tft->print("L");
}

void tftClass::drawPot(int XPos, byte YPos, int dvalue, const char *dname)
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

    draw_value_box(YPos, xPos, yPos, 4, -3, dvalue, "NO_NAME", color, 2, false, false);
    draw_value_box(YPos, xPos, yPos + 1, 0, 3, NO_VALUE, dname, color, 2, false, false);

    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_DARKGREY);
    tft->setCursor(STEP_FRAME_W * xPos, STEP_FRAME_H * (yPos + 1) + 3);
    tft->print(dname_old[XPos]);

    tft->setTextColor(ILI9341_WHITE);
    tft->setCursor(STEP_FRAME_W * xPos, STEP_FRAME_H * (yPos + 1) + 3);
    tft->print(dname);

    tft->fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos_old[XPos]) + 2.25), STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos_old[XPos]) + 2.25), 4, ILI9341_DARKGREY);
    tft->drawCircle(STEP_FRAME_W * (xPos + 1), STEP_FRAME_H * yPos, 16, ILI9341_LIGHTGREY);
    tft->fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos[XPos]) + 2.25), STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos[XPos]) + 2.25), 4, color);
    circlePos_old[XPos] = circlePos[XPos];

    dname_old[XPos] = dname;
}
void tftClass::drawEnvelope(byte YPos, byte attack, byte decay, byte sustain, byte release)
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

    tft->drawLine(envStart, yPos, old_attackEnd, envTop, ILI9341_DARKGREY);
    tft->drawLine(old_attackEnd, envTop, old_decayEnd + old_attackEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft->drawLine(old_decayEnd + old_attackEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft->drawLine(old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd + old_releaseEnd, yPos, ILI9341_DARKGREY);

    byte attackEnd = map(attack, 0, 127, 0, 50) + envStart;
    byte decayEnd = map(decay, 0, 127, 0, 30);
    byte sustainLevel = yPos - map(sustain, 0, 127, 0, 32);
    byte sustainEnd = 30;
    byte releaseEnd = map(release, 0, 127, 0, 50);

    tft->drawLine(envStart, yPos, attackEnd, envTop, colorA);
    tft->drawLine(attackEnd, envTop, decayEnd + attackEnd, sustainLevel, colorD);
    tft->drawLine(decayEnd + attackEnd, sustainLevel, decayEnd + attackEnd + sustainEnd, sustainLevel, colorS);
    tft->drawLine(decayEnd + attackEnd + sustainEnd, sustainLevel, decayEnd + attackEnd + sustainEnd + releaseEnd, yPos, colorR);

    draw_value_box(YPos, 14, ypos - 1, 4, 4, attack, "NO_NAME", colorA, 2, true, false);
    draw_value_box(YPos, 16, ypos - 1, 4, 4, decay, "NO_NAME", colorD, 2, true, false);
    draw_value_box(YPos, 14, ypos, 4, 4, sustain, "NO_NAME", colorS, 2, true, false);
    draw_value_box(YPos, 16, ypos, 4, 4, release, "NO_NAME", colorR, 2, true, false);

    old_attackEnd = attackEnd;
    old_decayEnd = decayEnd;
    old_sustainLevel = sustainLevel;
    old_sustainEnd = sustainEnd;
    old_releaseEnd = releaseEnd;
}
void tftClass::draw_value_box(byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int _value, const char *name, int color, byte _size, bool drawRect, bool drawFilling)
{

    int xPos = XPos * STEP_FRAME_W;
    byte yPos = YPos * STEP_FRAME_H;

    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_DARKGREY);
    tft->setCursor(xPos + offest_X, yPos + offset_Y);
    tft->fillRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
    // tft->print(old_value[index]);
    if (lastPotRow != lastPRow)
        color = ILI9341_LIGHTGREY;
    if (drawRect)
        tft->drawRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, color);
    if (drawFilling)
        tft->fillRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, color);

    tft->setTextColor(ILI9341_WHITE);
    tft->setCursor(xPos + offest_X, yPos + offset_Y);
    if (name != "NO_NAME")
        tft->print(name);
    if (_value != NO_VALUE)
        tft->print(_value);
}

void tftClass::show_active_page_info(const char *_pagename, byte _pagenumber)
{
    draw_value_box(0, 1, 0, 2, 3, NO_VALUE, _pagename, ILI9341_WHITE, 2, false, false);
    draw_value_box(0, 2, 0, 6, 3, _pagenumber, "NO_NAME", ILI9341_WHITE, 2, false, false);
}

// songmode
void tftClass::drawsongmodepageselector()
{
    // draw 16 rects of 16x16px in the 13th row
    for (int pages = 2; pages < 18; pages++)
    {
        // drawActiveRect(pages, 13, 1, 1, selectPage == pages + 8, "", ILI9341_LIGHTGREY);
        tft->drawRect(STEP_FRAME_W * pages, STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
        tft->setFont(Arial_8);
        tft->setTextColor(ILI9341_WHITE);
        tft->setCursor(STEP_FRAME_W * pages + 3, STEP_FRAME_H * 13 + 8);
        tft->print((pages - 2));
    }
}
void tftClass::gridSongMode(int songpageNumber)
{ // static Display rendering
    // int page_phrase_start = songpageNumber * 16;
    // int page_phrase_end = (songpageNumber + 1) * 16;
    clearWorkSpace();
    drawsongmodepageselector();
    // drawActiveRect(18, 3, 2, 2, false, "clear", ILI9341_RED);

    // vertical pointer Lines
    int shownLines = 257 / phraseSegmentLength;
    for (int f = 0; f < shownLines; f++)
    {                                                                                                 // do this for all phrases
        tft->drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 360); //(x, y-start, y-length, color)
        if (f % 4 == 0)
        {
            tft->drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 370); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < NUM_TRACKS; i++)
        draw_arrangment_lines(3, songpageNumber);
    change_plugin_row = true;
    draw_arranger_parameters(lastPotRow);

    show_active_page_info("Song:", songpageNumber);
}
void tftClass::draw_arrangment_lines(byte n, byte b) // b= active page
{
    for (int i = 0; i < 16; i++)
    {
        draw_arrangment_line(n, i + (BARS_PER_PAGE * (b - SONGMODE_PAGE_1)));
        // Serial.printf("active page = %d, which bar = %d\n", b, i + (16 * (b - SONGMODE_PAGE_1)));
    }
}
void tftClass::draw_arrangment_line(byte _trackNr, byte _arrangerpage, byte _bar, byte clipToPlayAtBar, byte velocityAtBar) // b= 0-255; which bar
{
    int minY = map(velocityAtBar, 0, 127, 0, 10);

    if (clipToPlayAtBar == MAX_CLIPS - 1)
    {
        for (int thickness = -10; thickness < 10; thickness++)
        {
            tft->drawFastHLine(((_bar - (16 * _arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((_trackNr + 1) * TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, ILI9341_DARKGREY); //(x-start, y, length, color)
        }
        trellis_set_main_buffer(_arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (_bar % 16), _trackNr, TRELLIS_BLACK);
    }
    else
    {
        // for other clips
        for (int thickness = -minY; thickness < minY; thickness++)
        {
            tft->drawFastHLine(((_bar - (16 * _arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((_trackNr + 1) * TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, trackColor[_trackNr] + (clipToPlayAtBar * 20)); //(x-start, y, length, color)
        }
        trellis_set_main_buffer(_arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (_bar % 16), _trackNr, trellisTrackColor[_trackNr] + (clipToPlayAtBar * 20));

        draw_clipNr_arranger(n, _bar);
        draw_offset_arranger(n, _bar);
    }
}
void tftClass::draw_clipNr_arranger(byte n, byte b)
{
    // draw clipnumber in the arranger
    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_BLACK);
    tft->setCursor((b - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + 2, (my_Arranger_Y_axis)*TRACK_FRAME_H + 6);
    tft->print(clip_to_play[b]);
}
void tftClass::draw_arranger_parameters(byte lastProw)
{
    if (change_plugin_row)
    {
        tft->fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        if (lastProw == 0)
        {
            draw_clip_to_play(2, 0);
            draw_noteOffset(3, 0);
            // drawOctaveNumber();
            // draw_velocity(3, 0);
        }
        if (lastProw == 1)
        {
            draw_barVelocity(0, 1);
        }
        if (lastProw == 2)
        {
            // draw_barVelocity(0, 1);
            draw_play_presetNr_ccChannel(2, 2);
            draw_play_presetNr_ccValue(3, 2);
            // drawOctaveNumber();
            // draw_velocity(3, 0);
        }
    }
}


// stepsequncer
void tftClass::drawStepSequencerStatic()
{
    clearWorkSpace();
    draw_Notenames();
    drawOctaveTriangle();
    draw_Clipselector();
    // draw the Main Grid
    for (int i = 0; i < 17; i++)
    { // vert Lines
        int step_Frame_X = i * 12;
        tft->drawFastVLine(step_Frame_X + STEP_FRAME_W * 2, STEP_FRAME_H, GRID_LENGTH_VERT, ILI9341_WHITE); //(x, y-start, length, color)
        if (i % 4 == 0)
        {
            tft->drawFastVLine((i * 12) + 32, STEP_FRAME_H, STEP_FRAME_H * 12, ILI9341_LIGHTGREY); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < 13; i++)
    { // hor lines
        int step_Frame_Y = i * 16;
        tft->drawFastHLine(STEP_FRAME_W * 2, step_Frame_Y + STEP_FRAME_H, NUM_STEPS * 12, ILI9341_WHITE); //(x-start, y, length, color)
    }
    // tft->asyncUpdateActive();
}

void tftClass::drawOctaveTriangle()
{
    // draw Octavebuttons
    int leftmost = STEP_FRAME_W * OCTAVE_CHANGE_LEFTMOST;
    int rightmost = STEP_FRAME_W * OCTAVE_CHANGE_RIGHTMOST;
    int UP_topmost = STEP_FRAME_H * OCTAVE_CHANGE_UP_TOPMOST;
    int UP_bottommost = STEP_FRAME_H * OCTAVE_CHANGE_UP_BOTTOMMOST;
    int DOWN_topmost = STEP_FRAME_H * OCTAVE_CHANGE_DOWN_TOPMOST;
    int DOWN_bottommost = STEP_FRAME_H * OCTAVE_CHANGE_DOWN_BOTTOMMOST;
    tft->fillRect(leftmost + 1, STEP_FRAME_H * 2, STEP_FRAME_W * 2, STEP_FRAME_H * 3, ILI9341_DARKGREY);
    tft->fillTriangle(leftmost + 1, UP_bottommost, rightmost, UP_bottommost, leftmost + STEP_FRAME_W, UP_topmost, ILI9341_LIGHTGREY);        // octave arrow up
    tft->fillTriangle(leftmost + 1, DOWN_topmost, rightmost - 2, DOWN_topmost, leftmost + STEP_FRAME_W, DOWN_bottommost, ILI9341_LIGHTGREY); // x1, y1, x2, y2, x3, y3
}
void tftClass::draw_Notenames()
{
    for (int n = 0; n < MAX_VOICES; n++)
    { // hor notes
        tft->fillRect(STEP_FRAME_W, STEP_FRAME_H * n + STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H, trackColor[activeTrack]);
        tft->setCursor(20, STEP_FRAME_H * n + 20);
        tft->setFont(Arial_8);
        tft->setTextColor(ILI9341_BLACK);
        tft->setTextSize(1);
        tft->print(noteNames[n]);
    }
}
void tftClass::draw_Clipselector()
{

    for (int ClipNr = 0; ClipNr < 8; ClipNr++)
    {
        tft->fillRect(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 1, STEP_FRAME_H * 13 + 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, trackColor[activeTrack] + (ClipNr * 20));
        tft->setCursor(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 4, STEP_FRAME_H * 13 + 4);
        tft->setFont(Arial_8);
        tft->setTextColor(ILI9341_BLACK);
        tft->setTextSize(1);
        tft->print("Clip ");
        tft->print(ClipNr);
    }
}

void tftClass::draw_note_on_tick(byte _note, byte _when, byte _clipNr, byte _velocity, int _color)
{
    if (activePage == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
        byte Note = _note % NOTES_PER_OCTAVE;
        int stepcolor = trackColor[activeTrack] + (_clipNr * 20);
        int minY = map(_velocity, 0, 127, 0, 4);
        int maxY = map(_velocity, 0, 127, 0, 5);

        // Serial.printf("draw velocity: %d tick: %d for note: %d on voice: %d\n", velo, dr_X, note, i);

        for (int w = -minY; w < maxY; w++)
        {
            tft->drawPixel((_when * 2) + 32, Note + w + 8, _color);
            tft->drawPixel((_when * 2) + 32 + 1, Note + w + 8, _color);
        }
    }
}
void tftClass::draw_notes_in_grid()
{
    // clear_notes_in_grid();
    for (int i = 0; i < MAX_TICKS; i++)
    {
        // clear_notes_on_tick(i);
        draw_note_on_tick(i);
    }
}
