#ifndef PL_FUNCTIONS_H
#define PL_FUNCTIONS_H

#include <Arduino.h>
#include "project_variables.h"
#include "Track.h"
#include "clock.h"

void encoder_SetCursor(byte deltaX, byte maxY);
void buttons_SetCursor(byte maxY);
void buttons_SetPlayStatus();
void buttons_SelectTrack();
void buttons_SelectSequencerMode();
void buttons_SelectPlugin();
void buttons_save_track();
void buttons_load_track();
void buttons_set_track_recordState();
void mouse(byte deltaX, byte maxY);

void buttons_save_all();
void buttons_load_all();
void buttons_SelectArranger();
void buttons_Set_potRow();
void buttons_SetNoteOnTick(int x, byte y);
void clock_to_notes();
void input_behaviour();

void clearWorkSpace();
void drawPot(int XPos, byte YPos, int dvalue, const char *dname);
void draw_Value(byte index, byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int value, int color, bool drawRect, bool drawFilling);
void draw_Text(byte index, byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y,  const char *name, int color, bool drawRect, bool drawFilling);
void show_active_track();
void show_active_songpage();
void drawsongmodepageselector();
void gridSongMode(int songpageNumber);


void startUpScreen();
#endif