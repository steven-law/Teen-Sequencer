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
void tftClass::tftUpdate(int _pixelOnX, int _pixelOnY, byte _activeTrack, byte _activePage, byte _lastPotRow)
{
    pixelOnX = _pixelOnX;
    pixelOnY = _pixelOnY;
    activeTrack = _activeTrack;
    activePage = _activePage;
    actualPotRow = _lastPotRow;
    if (activePage == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
        cursorOnTick = pixelOnX / 2 - SEQ_GRID_LEFT;
        cursorOnNote = pixelOnY - 1;
    }
}
void tftClass::tftUpdateClock(byte _currentTick, byte _currentBar, byte _startLoop, byte _endLoop)
{

    currentTick = _currentTick;
    currentBar = _currentBar;
    loopStart = _startLoop;
    loopEnd = _endLoop;
}


void tftClass::show()
{

    if (updateClock)
    {
        drawstepPosition();
        drawbarPosition();
        updateClock = false;
    }
    tft->fillRect(70, lastPotRow * 4, 10, 3, ILI9341_ORANGE);

    tft->updateScreenAsync();
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

    draw_value_box(YPos, xPos, yPos, 4, -3, dvalue, NO_NAME, color, 2, false, false);
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

    draw_value_box(YPos, 14, ypos - 1, 4, 4, attack, NO_NAME, colorA, 2, true, false);
    draw_value_box(YPos, 16, ypos - 1, 4, 4, decay, NO_NAME, colorD, 2, true, false);
    draw_value_box(YPos, 14, ypos, 4, 4, sustain, NO_NAME, colorS, 2, true, false);
    draw_value_box(YPos, 16, ypos, 4, 4, release, NO_NAME, colorR, 2, true, false);

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
    if (name != NO_NAME)
        tft->print(name);
    if (_value != NO_VALUE)
        tft->print(_value);
}

void tftClass::show_active_page_info(const char *_pagename, byte _pagenumber)
{
    draw_value_box(0, 1, 0, 2, 3, NO_VALUE, _pagename, ILI9341_WHITE, 2, false, false);
    draw_value_box(0, 2, 0, 6, 3, _pagenumber, NO_NAME, ILI9341_WHITE, 2, false, false);
}

// infobox
void tftClass::get_infobox_background()
{
    infoboxTimeAtPress = millis();
    if (infoboxShow && infoboxTimeAtPress - infoboxTimeAtCall >= infoboxWaitingTime)
    {
        infoboxClear = true;
        infoboxShow = false;
        // infoboxTimeAtPress=0;
    }
}
void tftClass::set_infobox_background(int _DisplayTime)
{
    infoboxWaitingTime = _DisplayTime;
    tft->fillRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_BLACK);
    tft->drawRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_WHITE);
    tft->setFont(Arial_10);
    tft->setTextColor(ILI9341_WHITE);
    tft->setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET);
    infoboxTimeAtCall = millis();
    infoboxShow = true;
}
void tftClass::set_infobox_next_line(byte _lineNumber)
{
    tft->setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET + (20 * _lineNumber));
}
void tftClass::reset_infobox_background()
{
    // tft.updateScreenAsync();
    if (infoboxClear)
    {
        infoboxClear = false;

        clearWorkSpace();
        change_plugin_row = true;
    }
}
// cursor
void tftClass::tftUpdateCursor(int X, int Y, int dX, int dY)
{
    pixelOnX = X;
    pixelOnY = Y;
    cursorDeltaX = dX;
    cursorDeltaY = dY;
    // showCoordinates();
    moveCursor();
    // tft->updateScreenAsync();
}
void tftClass::moveCursor()
{
    static int last_xPos;
    static int last_yPos;

    if (pixelOnX != last_xPos || pixelOnY != last_yPos)
    {
        int arranger_offset = 0;
        if (cursorDeltaY == TRACK_FRAME_H)
            if (pixelOnY >= 1)
                arranger_offset = 4;
        for (int pixel = 0; pixel < 16; pixel++)
        {
            tft->drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 1 + arranger_offset, tftRAM[0][pixel]);  // draw upper line X1
            tft->drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 15 + arranger_offset, tftRAM[1][pixel]); // draw bottom line X2
            tft->drawPixel((cursorDeltaX * last_xPos) + 1, pixel + (cursorDeltaY * last_yPos) + arranger_offset, tftRAM[2][pixel]);  // draw left line Y1
            tft->drawPixel((cursorDeltaX * last_xPos) + 15, pixel + (cursorDeltaY * last_yPos) + arranger_offset, tftRAM[3][pixel]); // draw right line Y2
        }
        for (int pixel = 0; pixel < 16; pixel++)
        {
            tftRAM[0][pixel] = tft->readPixel(pixel + (cursorDeltaX * pixelOnX), (cursorDeltaY * pixelOnY) + 1 + arranger_offset);  // save upper line
            tftRAM[1][pixel] = tft->readPixel(pixel + (cursorDeltaX * pixelOnX), (cursorDeltaY * pixelOnY) + 15 + arranger_offset); // save bottom line
            tftRAM[2][pixel] = tft->readPixel((cursorDeltaX * pixelOnX) + 1, pixel + (cursorDeltaY * pixelOnY) + arranger_offset);  // save left line
            tftRAM[3][pixel] = tft->readPixel((cursorDeltaX * pixelOnX) + 15, pixel + (cursorDeltaY * pixelOnY) + arranger_offset); // save right line
        }

        tft->drawRect((cursorDeltaX * pixelOnX) + 1, (cursorDeltaY * pixelOnY) + 1 + arranger_offset, STEP_FRAME_W - 1, STEP_FRAME_H - 1, ILI9341_WHITE);

        last_xPos = pixelOnX;
        last_yPos = pixelOnY;
    }
}
// clock
void tftClass::drawstepPosition()
{

    // draw the songpointer positions
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        for (int stepwidth = 1; stepwidth <= 16; stepwidth++)
        {
            tft->drawFastHLine(currentTick * stepwidth + STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_GREEN);
            tft->drawFastHLine((currentTick - 1) * stepwidth + STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
        }
        if (currentTick == 0)
        {
            tft->drawFastHLine(STEP_FRAME_W * 17, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
        }
    }
    // Serial.print(current);
}
void tftClass::drawbarPosition()
{

    // draw phrasenumber
    tft->fillRect(STEP_FRAME_W * POSITION_BAR_BUTTON + 1, 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, ILI9341_DARKGREY);
    tft->setTextColor(ILI9341_WHITE);
    tft->setFont(Arial_9);
    tft->setCursor(STEP_FRAME_W * POSITION_BAR_BUTTON + 4, 3);
    tft->print(currentBar);

    // drawbarPosition
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        tft->drawPixel(currentBar + STEP_FRAME_W * 2, (SONG_POSITION_POINTER_Y + songPointerThickness), ILI9341_GREEN);

        tft->drawFastHLine((currentBar)*phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_GREEN);
        tft->drawFastHLine((currentBar - 1) * phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_DARKGREY);
    }
    if (currentBar == loopStart)
    {
        // pixelphrase = 0;
        tft->fillRect(STEP_FRAME_W * (loopEnd + 2), BAR_POSITION_POINTER_Y, STEP_FRAME_W, POSITION_POINTER_THICKNESS + 1, ILI9341_DARKGREY);
        // tft->fillRect(STEP_FRAME_W * 2, GRID_POSITION_POINTER_Y, STEP_FRAME_W * 1, 4, ILI9341_GREEN);
    }
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
void tftClass::draw_arrangment_line(byte _trackNr, byte _bar) // b= 0-255; which bar
{

    int minY = map(allTracks[_trackNr]->barVelocity[_bar], 0, 127, 0, 10);

    if (allTracks[_trackNr]->clip_to_play[_bar] == MAX_CLIPS - 1)
    {
        for (int thickness = -10; thickness < 10; thickness++)
        {
            tft->drawFastHLine(((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, ILI9341_DARKGREY); //(x-start, y, length, color)
        }
        trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (_bar % 16), allTracks[_trackNr]->my_Arranger_Y_axis - 1, TRELLIS_BLACK);
    }
    else
    {
        // for other clips
        for (int thickness = -minY; thickness < minY; thickness++)
        {
            tft->drawFastHLine(((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, trackColor[allTracks[_trackNr]->my_Arranger_Y_axis - 1] + (allTracks[activeTrack]->clip_to_play[_bar] * 20)); //(x-start, y, length, color)
        }
        trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (_bar % 16), allTracks[_trackNr]->my_Arranger_Y_axis - 1, trellisTrackColor[allTracks[_trackNr]->my_Arranger_Y_axis - 1] + (allTracks[_trackNr]->clip_to_play[_bar] * 20));

        draw_clipNr_arranger(_trackNr, _bar);
        draw_offset_arranger(_trackNr, _bar);
    }
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
void tftClass::draw_clip_to_play(byte _trackNr, byte _bar)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 5, 0, 4, NO_VALUE, "Clip", encoder_colour[_trackNr], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 5, 0, 4, allTracks[_trackNr]->clip_to_play[_bar], NO_NAME, encoder_colour[_trackNr], 2, false, false);

    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "clNr", clip_to_play[b], n, 0);
}
void tftClass::draw_offset_arranger(byte _trackNr, byte _bar)
{
    int xoffset;
    if (allTracks[_trackNr]->noteOffset[_bar] < 0)
        xoffset = 1;
    else
        xoffset = 5;
    // draw clipnumber in the arranger
    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_BLACK);
    tft->setCursor((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + xoffset, (_trackNr)*TRACK_FRAME_H + 11);
    tft->print(allTracks[_trackNr]->noteOffset[_bar]);
}
void tftClass::draw_clipNr_arranger(byte _trackNr, byte _bar)
{
    // draw clipnumber in the arranger
    tft->setFont(Arial_8);
    tft->setTextColor(ILI9341_BLACK);
    tft->setCursor((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + 2, (allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + 6);
    tft->print(allTracks[_trackNr]->clip_to_play[_bar]);
}
void tftClass::draw_play_presetNr_ccValue(byte _trackNr, byte lastProw)
{
    draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 5, 0, 4, NO_VALUE, "vl-Set", encoder_colour[_trackNr], 2, false, false);
    draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 6, 0, 4, allTracks[_trackNr]->play_presetNr_ccValue[allTracks[_trackNr]->bar_to_edit], NO_NAME, encoder_colour[_trackNr], 2, false, false);
}
void tftClass::draw_play_presetNr_ccChannel(byte _trackNr, byte lastProw)
{
    draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 5, 0, 4, NO_VALUE, "cc-Set", encoder_colour[_trackNr], 2, false, false);
    draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 6, 0, 4, allTracks[_trackNr]->play_presetNr_ccChannel[allTracks[_trackNr]->bar_to_edit], NO_NAME, encoder_colour[_trackNr], 2, false, false);
}
void tftClass::draw_barVelocity(byte _trackNr, int _bar)
{

    draw_value_box(1, SEQUENCER_OPTIONS_VERY_RIGHT, (0 * 2) + 5, 0, 4, NO_VALUE, "Velo", encoder_colour[_trackNr], 2, false, false);
    draw_value_box(1, SEQUENCER_OPTIONS_VERY_RIGHT, (0 * 2) + 6, 0, 4, allTracks[_trackNr]->barVelocity[_bar], NO_NAME, encoder_colour[_trackNr], 2, false, false);

    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "ofSet", noteOffset[b], n, 0);
}
void tftClass::draw_noteOffset(byte _trackNr, int _bar)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 5, 0, 4, NO_VALUE, "Trns", encoder_colour[_trackNr], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 6, 0, 4, allTracks[_trackNr]->noteOffset[_bar], NO_NAME, encoder_colour[_trackNr], 2, false, false);

    // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "ofSet", noteOffset[b], n, 0);
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

void tftClass::draw_note_on_tick(byte _note, byte _when)
{

    if (activePage == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
        int _color;
        byte Note = _note % NOTES_PER_OCTAVE;

        // Serial.printf("draw velocity: %d tick: %d for note: %d on voice: %d\n", velo, dr_X, note, i);
        byte note = allTracks[activeTrack]->clip[allTracks[activeTrack]->parameter[SET_CLIP2_EDIT]].tick[_when].voice[Note];
        byte velo = allTracks[activeTrack]->clip[allTracks[activeTrack]->parameter[SET_CLIP2_EDIT]].tick[_when].velo[Note];
        if (note == NO_NOTE)
            _color = ILI9341_BLACK;
        else
            trackColor[activeTrack] + (allTracks[activeTrack]->parameter[SET_CLIP2_EDIT] * 20);
        if (note >= allTracks[activeTrack]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE && note < (allTracks[activeTrack]->parameter[SET_OCTAVE] + 1) * NOTES_PER_OCTAVE)
        {
            int minY = map(velo, 0, 127, 0, 4);
            int maxY = map(velo, 0, 127, 0, 5);
            for (int w = -minY; w < maxY; w++)
            {
                tft->drawPixel((_when * 2) + 32, note + w + 8, _color);
                tft->drawPixel((_when * 2) + 32 + 1, note + w + 8, _color);
            }
        }
    }
}
void tftClass::draw_notes_in_grid()
{
    // clear_notes_in_grid();
    for (int i = 0; i < MAX_TICKS; i++)
    {
        for (int v = 0; v < MAX_VOICES; v++)
        {
            // clear_notes_on_tick(i);
            draw_note_on_tick(i, v);
        }
    }
}
void tftClass::draw_stepSequencer_parameter_text(byte lastPRow, byte XPos, byte YPos, const char *text, const char *name)

{
    // change_plugin_row=true;
    byte index = XPos + (YPos * NUM_ENCODERS);
    Serial.printf("Drawing text at index %d, name %s, text %s\n", index, name, text);
    draw_value_box(lastPRow, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 5, 0, 4, NO_VALUE, name, encoder_colour[XPos], 2, false, false);
    draw_value_box(lastPRow, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 6, 4, 4, NO_VALUE, text, encoder_colour[XPos], 2, false, false);

    // draw_Text(XPos, YPos, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 6, 4, 4, text, encoder_colour[XPos], true, false);
}
void tftClass::draw_stepSequencer_parameter_value(byte lastPRow, byte XPos, byte YPos, byte value, const char *name)
{
    byte index = XPos + (YPos * NUM_ENCODERS);
    draw_value_box(lastPRow, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 5, 4, 4, NO_VALUE, name, encoder_colour[XPos], 2, false, false);
    draw_value_box(lastPRow, SEQUENCER_OPTIONS_VERY_RIGHT, (XPos * 2) + 6, 4, 4, value, NO_NAME, encoder_colour[XPos], 2, false, false);
}
void tftClass::draw_stepSequencer_parameters(byte lastProw)
{
    if (change_plugin_row)
    {
        tft->fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        if (lastProw == 0)
        {
            draw_stepSequencer_parameter_value(0, 0, 0, allTracks[activeTrack]->parameter[0], "Tick");
            draw_stepSequencer_parameter_value(0, 1, 0, allTracks[activeTrack]->parameter[1], "Note");
            draw_stepSequencer_parameter_value(0, 2, 0, allTracks[activeTrack]->parameter[2], CCnames[allTracks[activeTrack]->parameter[2]]);
            draw_stepSequencer_parameter_value(0, 3, 0, allTracks[activeTrack]->parameter[3], "Velo");
        }
        if (lastProw == 1)
        {
            draw_stepSequencer_parameter_value(1, ENCODER_SEQUENCE_LENGTH, 1, allTracks[activeTrack]->parameter[4], "seqL");
            draw_stepSequencer_parameter_value(1, ENCODER_STEP_DIVISION, 1, allTracks[activeTrack]->parameter[5], "sDiv");
            draw_stepSequencer_parameter_value(1, ENCODER_STEP_LENGTH, 1, allTracks[activeTrack]->parameter[6], "stpL");
            draw_stepSequencer_parameter_value(1, ENCODER_OCTAVE, 1, allTracks[activeTrack]->parameter[7], "Oct");
        }
        if (lastProw == 2)
        {
            draw_stepSequencer_parameter_text(2, ENCODER_SEQ_MODE, 2, seqModname[allTracks[activeTrack]->parameter[8]], "sMod");
            draw_stepSequencer_parameter_text(2, ENCODER_MIDICH_OUT, 2, channelOutNames[allTracks[activeTrack]->parameter[9]], "MCh");
            draw_stepSequencer_parameter_value(2, ENCODER_CLIP2_EDIT, 2, allTracks[activeTrack]->parameter[11], "Clip");
        }
    }
}

void tftClass::draw_edit_presetNr_ccValue(byte n, byte lastProw)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 5, 0, 4, NO_VALUE, "vl-Set", encoder_colour[activeTrack], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 5, 0, 4, allTracks[activeTrack]->edit_presetNr_ccValue, NO_NAME, encoder_colour[activeTrack], 2, false, false);
}
void tftClass::draw_edit_presetNr_ccChannel(byte n, byte lastProw)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 5, 0, 4, NO_VALUE, "cc-Set", encoder_colour[activeTrack], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 5, 0, 4, allTracks[activeTrack]->edit_presetNr_ccChannel, NO_NAME, encoder_colour[activeTrack], 2, false, false);
}
void tftClass::draw_MIDI_CC_screen()
{

    if (change_plugin_row)
    {
        clearWorkSpace();
        change_plugin_row = false;
        draw_MIDI_CC(0, 0);
        draw_MIDI_CC(1, 0);
        draw_MIDI_CC(2, 0);
        draw_MIDI_CC(3, 0);

        draw_MIDI_CC(0, 1);
        draw_MIDI_CC(1, 1);
        draw_MIDI_CC(2, 1);
        draw_MIDI_CC(3, 1);

        draw_MIDI_CC(0, 2);
        draw_MIDI_CC(1, 2);
        draw_MIDI_CC(2, 2);
        draw_MIDI_CC(3, 2);

        draw_MIDI_CC(0, 3);
        draw_MIDI_CC(1, 3);
        draw_MIDI_CC(2, 3);
        draw_MIDI_CC(3, 3);

        draw_edit_presetNr_ccChannel(2, 0);
        draw_edit_presetNr_ccValue(3, 0);
    }
}
void tftClass::draw_MIDI_CC(byte XPos, byte YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    drawPot(XPos, YPos, allTracks[activeTrack]->CCvalue[allTracks[activeTrack]->edit_presetNr_ccValue][n], CCnames[allTracks[activeTrack]->CCchannel[allTracks[activeTrack]->edit_presetNr_ccChannel][n]]);
}

// mixer
void tftClass::draw_mixer()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[0]->mixGainPot, "Tr D");
        draw_value_box(actualPotRow, 3, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[0]->muted);
        draw_value_box(actualPotRow, 4, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[0]->soloed);
        drawPot(1, 0, allTracks[1]->mixGainPot, "Tr 2");
        draw_value_box(actualPotRow, 7, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[1]->muted);
        draw_value_box(actualPotRow, 8, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[1]->soloed);
        drawPot(2, 0, allTracks[2]->mixGainPot, "Tr 3");
        draw_value_box(actualPotRow, 11, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[2]->muted);
        draw_value_box(actualPotRow, 12, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[2]->soloed);
        drawPot(3, 0, allTracks[3]->mixGainPot, "Tr 4");
        draw_value_box(actualPotRow, 15, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[3]->muted);
        draw_value_box(actualPotRow, 16, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[3]->soloed);

        drawPot(0, 2, allTracks[4]->mixGainPot, "Tr 5");
        draw_value_box(actualPotRow, 3, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[4]->muted);
        draw_value_box(actualPotRow, 4, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[4]->soloed);
        drawPot(1, 2, allTracks[5]->mixGainPot, "Tr 6");
        draw_value_box(actualPotRow, 7, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[5]->muted);
        draw_value_box(actualPotRow, 8, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[5]->soloed);
        drawPot(2, 2, allTracks[6]->mixGainPot, "Tr 7");
        draw_value_box(actualPotRow, 11, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[6]->muted);
        draw_value_box(actualPotRow, 12, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[6]->soloed);
        drawPot(3, 2, allTracks[7]->mixGainPot, "Tr 8");
        draw_value_box(actualPotRow, 15, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, false, allTracks[7]->muted);
        draw_value_box(actualPotRow, 16, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, false, allTracks[7]->soloed);
    }
}
void tftClass::draw_mixer_FX_page1()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[0]->mixDryPot, "Dry D");
        drawPot(1, 0, allTracks[1]->mixDryPot, "Dry 2");
        drawPot(2, 0, allTracks[2]->mixDryPot, "Dry 3");
        drawPot(3, 0, allTracks[3]->mixDryPot, "Dry 4");

        drawPot(0, 1, allTracks[0]->mixFX1Pot, "FX1 D");
        drawPot(1, 1, allTracks[1]->mixFX1Pot, "FX1 2");
        drawPot(2, 1, allTracks[2]->mixFX1Pot, "FX1 3");
        drawPot(3, 1, allTracks[3]->mixFX1Pot, "FX1 4");

        drawPot(0, 2, allTracks[0]->mixFX2Pot, "FX2 D");
        drawPot(1, 2, allTracks[1]->mixFX2Pot, "FX2 2");
        drawPot(2, 2, allTracks[2]->mixFX2Pot, "FX2 3");
        drawPot(3, 2, allTracks[3]->mixFX2Pot, "FX2 4");

        drawPot(0, 3, allTracks[0]->mixFX3Pot, "FX3 D");
        drawPot(1, 3, allTracks[1]->mixFX3Pot, "FX3 2");
        drawPot(2, 3, allTracks[2]->mixFX3Pot, "FX3 3");
        drawPot(3, 3, allTracks[3]->mixFX3Pot, "FX3 4");
    }
}
void tftClass::draw_mixer_FX_page2()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[4]->mixDryPot, "Dry 5");
        drawPot(1, 0, allTracks[5]->mixDryPot, "Dry 6");
        drawPot(2, 0, allTracks[6]->mixDryPot, "Dry 7");
        drawPot(3, 0, allTracks[7]->mixDryPot, "Dry 8");

        drawPot(0, 1, allTracks[4]->mixFX1Pot, "FX1 5");
        drawPot(1, 1, allTracks[5]->mixFX1Pot, "FX1 6");
        drawPot(2, 1, allTracks[6]->mixFX1Pot, "FX1 7");
        drawPot(3, 1, allTracks[7]->mixFX1Pot, "FX1 8");

        drawPot(0, 2, allTracks[4]->mixFX2Pot, "FX1 5");
        drawPot(1, 2, allTracks[5]->mixFX2Pot, "FX1 6");
        drawPot(2, 2, allTracks[6]->mixFX2Pot, "FX1 7");
        drawPot(3, 2, allTracks[7]->mixFX2Pot, "FX1 8");

        drawPot(0, 3, allTracks[4]->mixFX3Pot, "FX1 5");
        drawPot(1, 3, allTracks[5]->mixFX3Pot, "FX1 6");
        drawPot(2, 3, allTracks[6]->mixFX3Pot, "FX1 7");
        drawPot(3, 3, allTracks[7]->mixFX3Pot, "FX1 8");
    }
}

tftClass Display(&tft);
tftClass *mytft=&Display;