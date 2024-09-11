

#include <Arduino.h>
#include "project_variables.h"
#include "SD.h"
File myFile;

// extern Clock Masterclock;
int pixelTouchX = 0;
int gridTouchY = 0;
byte lastPotRow = 0;
byte activeScreen = 0;
bool change_plugin_row = false;

unsigned long infoboxTimeAtCall = 0;
unsigned long infoboxTimeAtPress = 0;
int infoboxWaitingTime = 1000;
bool infoboxShow = false;
bool infoboxClear = false;

byte active_track;
byte arrangerpage;
const char FLASHMEM *songNames[MAX_SONGS]{"Marshmallow", "KittyPitty", "DragonPunch", "Snozzle", "Wildbeast", "Worldpeace", "Jumanji", "WeAreApes", "MegaHit"};

byte trellisScreen = 0;
int trackColor[9]{ILI9341_RED, ILI9341_PINK, ILI9341_OLIVE, ILI9341_YELLOW, ILI9341_BLUE, 9365295, ILI9341_CYAN, ILI9341_GREEN, ILI9341_WHITE};
int trellisTrackColor[9]{TRELLIS_RED, TRELLIS_PURPLE, TRELLIS_OLIVE, TRELLIS_YELLOW, TRELLIS_BLUE, 9365295, TRELLIS_AQUA, TRELLIS_GREEN, 900909};
 int trellisControllBuffer[TRELLIS_CONTROL_X_DIM][TRELLIS_CONTROL_Y_DIM];
 int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];

bool neotrellisPressed[X_DIM * Y_DIM];
bool trellisShowClockPixel[Y_DIM];
byte trellisPianoTrack;
byte trellisPerformIndex[NUM_STEPS];
byte performCC[16]{7, 77, 24, 85, 84, // Volume, EnvDepth, Data H, Rev Send, Del Send,
                   109, 119, 5, 83,   // LFO1, LFO2, notelength, amprelease
                   30, 92, 74, 75,
                   128, 128, 128};
bool trellisRecall;
int encoder_colour[NUM_ENCODERS] = {ILI9341_BLUE, ILI9341_RED, ILI9341_GREEN, ILI9341_WHITE};
// songmode
int phraseSegmentLength = 16;
// mixer

const char FLASHMEM *CCnames[129]{"CC0", "CC1", "CC2", "CC3", "CC4", "CC5", "CC6", "CC7", "CC8", "CC9",
                                  "CC10", "CC11", "CC12", "CC13", "CC14", "CC15", "CC16", "CC17", "CC18", "CC19",
                                  "CC20", "CC21", "CC22", "CC23", "CC24", "CC25", "CC26", "CC27", "CC28", "CC29",
                                  "CC30", "CC31", "CC32", "CC33", "CC34", "CC35", "CC36", "CC37", "CC38", "CC39",
                                  "CC40", "CC41", "CC42", "CC43", "CC44", "CC45", "CC46", "CC47", "CC48", "CC49",
                                  "CC50", "CC51", "CC52", "CC53", "CC54", "CC55", "CC56", "CC57", "CC58", "CC59",
                                  "CC60", "CC61", "CC62", "CC63", "CC64", "CC65", "CC66", "CC67", "CC68", "CC69",
                                  "CC70", "CC71", "CC72", "CC73", "CC74", "CC75", "CC76", "CC77", "CC78", "CC79",
                                  "CC80", "CC81", "CC82", "CC83", "CC84", "CC85", "CC86", "CC87", "CC88", "CC89",
                                  "CC90", "CC91", "CC92", "CC93", "CC94", "CC95", "CC96", "CC97", "CC98", "CC99",
                                  "CC100", "CC101", "CC102", "CC103", "CC104", "CC105", "CC106", "CC107", "CC108", "CC109",
                                  "CC110", "CC111", "CC112", "CC113", "CC114", "CC115", "CC116", "CC117", "CC118", "CC119",
                                  "CC120", "CC121", "CC122", "CC123", "CC124", "CC125", "CC126", "CC127", "none"};

const char FLASHMEM *channelOutNames[MAX_OUTPUTS + 1]{"CV", "SR1", "SR2", "SR3", "SR4", "SR5", "SR6", "SR7", "SR8",
                                                      "SR9", "SR10", "SR11", "SR12", "SR13", "SR14", "SR15", "SR16",
                                                      "UD1", "UD2", "UD3", "UD4", "UD5", "UD6", "UD7", "UD8",
                                                      "UD9", "UD10", "UD11", "UD12", "UD13", "UD14", "UD15", "UD16",
                                                      "Ua1", "Ua2", "Ua3", "Ua4", "Ua5", "Ua6", "Ua7", "Ua8",
                                                      "Ua9", "Ua10", "Ua11", "Ua12", "Ua13", "Ua14", "Ua15", "Ua16",
                                                      "Strg", "1OSC", "FM2", "mDrm", "Drum", "Adtv", "Boom", "dTun", "rDrm", "SF2", "Ext"};

const char FLASHMEM *noteNames[12]{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const char FLASHMEM *seqModname[5]{"Step", "Rand", "Drop", "BitRd", "PotS"};
char _trackname[20];

// plugins
float *note_frequency;
int tuning = 440;
const char *filterName[4]{"LPF", "BPF", "HPF", "LPF2"};