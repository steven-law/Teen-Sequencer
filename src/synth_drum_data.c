/* Audio Library for Teensy, FM Drum
 * Copyright (c) 2023, Matt Kuebrich / mxmxmx
 * 
 * This is a tweaked version of mxmxmx's Teensy port of the FM Drum mode from the Mutable Instruments Peaks eurorack module.
 * https://github.com/mxmxmx/minitrash/blob/master/soft/libraries/Audio/synth_fm_drum.cpp
 * 
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Copyright 2013 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Sine FM drum - similar to the BD/SD in Anushri.

#include <stdint.h>
 
const uint32_t lut_env_increments[] = {
  178956970, 162203921, 147263779, 133914742,
  121965179, 111249129, 101622525, 92960022,
  85152324, 78103929, 71731214, 65960813,
  60728233, 55976673, 51656020, 47721976,
  44135321, 40861270, 37868923, 35130786,
  32622357, 30321772, 28209484, 26268003,
  24481648, 22836346, 21319451, 19919582,
  18626484, 17430905, 16324491, 15299685,
  14349647, 13468178, 12649652, 11888959,
  11181454, 10522909, 9909470, 9337624,
  8804164, 8306157, 7840925, 7406012,
  6999170, 6618338, 6261623, 5927288,
  5613733, 5319488, 5043200, 4783621,
  4539601, 4310076, 4094068, 3890668,
  3699040, 3518407, 3348049, 3187303,
  3035548, 2892213, 2756766, 2628711,
  2507588, 2392971, 2284460, 2181685,
  2084301, 1991984, 1904435, 1821372,
  1742534, 1667676, 1596567, 1528995,
  1464759, 1403670, 1345553, 1290243,
  1237585, 1187435, 1139655, 1094119,
  1050706, 1009303, 969803, 932108,
  896122, 861758, 828932, 797565,
  767584, 738918, 711503, 685274,
  660174, 636148, 613143, 591109,
  569999, 549770, 530379, 511787,
  493956, 476850, 460436, 444683,
  429558, 415035, 401085, 387683,
  374804, 362424, 350523, 339078,
  328069, 317479, 307287, 297478,
  288035, 278942, 270184, 261748,
  253620, 245786, 238236, 230956,
  223936, 217166, 210635, 204334,
  198253, 192383, 186717, 181245,
  175961, 170858, 165927, 161162,
  156558, 152107, 147805, 143644,
  139621, 135729, 131964, 128321,
  124796, 121384, 118081, 114883,
  111787, 108788, 105883, 103069,
  100343,  97700,  95139,  92657,
   90250,  87917,  85654,  83459,
   81330,  79265,  77261,  75316,
   73428,  71596,  69817,  68090,
   66413,  64784,  63202,  61666,
   60173,  58722,  57312,  55942,
   54610,  53315,  52056,  50832,
   49641,  48484,  47357,  46262,
   45196,  44159,  43149,  42167,
   41211,  40280,  39374,  38492,
   37632,  36796,  35981,  35187,
   34414,  33660,  32926,  32211,
   31513,  30834,  30172,  29526,
   28896,  28282,  27684,  27100,
   26531,  25975,  25434,  24905,
   24390,  23886,  23395,  22916,
   22449,  21992,  21546,  21111,
   20687,  20272,  19867,  19471,
   19085,  18708,  18339,  17979,
   17627,  17283,  16947,  16619,
   16298,  15985,  15678,  15378,
   15085,  14799,  14519,  14245,
   13977,  13716,  13459,  13209,
   12964,  12724,  12490,  12260,
   12036,  11816,  11601,  11390,
   11184,
};

const int16_t wav_overdrive[] = {
  -32767, -32767, -32767, -32767,
  -32767, -32767, -32767, -32767,
  -32766, -32766, -32766, -32766,
  -32766, -32766, -32766, -32766,
  -32766, -32766, -32766, -32766,
  -32766, -32765, -32765, -32765,
  -32765, -32765, -32765, -32765,
  -32765, -32765, -32765, -32765,
  -32764, -32764, -32764, -32764,
  -32764, -32764, -32764, -32764,
  -32763, -32763, -32763, -32763,
  -32763, -32763, -32763, -32763,
  -32762, -32762, -32762, -32762,
  -32762, -32762, -32761, -32761,
  -32761, -32761, -32761, -32761,
  -32760, -32760, -32760, -32760,
  -32760, -32759, -32759, -32759,
  -32759, -32759, -32758, -32758,
  -32758, -32758, -32757, -32757,
  -32757, -32757, -32756, -32756,
  -32756, -32756, -32755, -32755,
  -32755, -32754, -32754, -32754,
  -32753, -32753, -32753, -32752,
  -32752, -32752, -32751, -32751,
  -32751, -32750, -32750, -32749,
  -32749, -32749, -32748, -32748,
  -32747, -32747, -32746, -32746,
  -32745, -32745, -32744, -32744,
  -32743, -32743, -32742, -32742,
  -32741, -32741, -32740, -32740,
  -32739, -32738, -32738, -32737,
  -32736, -32736, -32735, -32734,
  -32734, -32733, -32732, -32732,
  -32731, -32730, -32729, -32728,
  -32728, -32727, -32726, -32725,
  -32724, -32723, -32722, -32721,
  -32720, -32719, -32718, -32717,
  -32716, -32715, -32714, -32713,
  -32712, -32711, -32710, -32709,
  -32707, -32706, -32705, -32704,
  -32702, -32701, -32700, -32698,
  -32697, -32695, -32694, -32692,
  -32691, -32689, -32688, -32686,
  -32684, -32683, -32681, -32679,
  -32678, -32676, -32674, -32672,
  -32670, -32668, -32666, -32664,
  -32662, -32660, -32658, -32655,
  -32653, -32651, -32649, -32646,
  -32644, -32641, -32639, -32636,
  -32633, -32631, -32628, -32625,
  -32622, -32619, -32617, -32614,
  -32610, -32607, -32604, -32601,
  -32597, -32594, -32591, -32587,
  -32584, -32580, -32576, -32572,
  -32568, -32564, -32560, -32556,
  -32552, -32548, -32543, -32539,
  -32534, -32530, -32525, -32520,
  -32515, -32510, -32505, -32500,
  -32495, -32489, -32484, -32478,
  -32473, -32467, -32461, -32455,
  -32448, -32442, -32436, -32429,
  -32422, -32416, -32409, -32402,
  -32394, -32387, -32380, -32372,
  -32364, -32356, -32348, -32340,
  -32331, -32323, -32314, -32305,
  -32296, -32287, -32277, -32268,
  -32258, -32248, -32237, -32227,
  -32216, -32206, -32195, -32183,
  -32172, -32160, -32148, -32136,
  -32124, -32111, -32098, -32085,
  -32072, -32058, -32044, -32030,
  -32016, -32001, -31986, -31971,
  -31955, -31939, -31923, -31907,
  -31890, -31873, -31855, -31838,
  -31819, -31801, -31782, -31763,
  -31743, -31723, -31703, -31682,
  -31661, -31640, -31618, -31596,
  -31573, -31550, -31526, -31502,
  -31478, -31453, -31427, -31401,
  -31375, -31348, -31320, -31293,
  -31264, -31235, -31205, -31175,
  -31145, -31113, -31082, -31049,
  -31016, -30983, -30948, -30913,
  -30878, -30842, -30805, -30767,
  -30729, -30690, -30650, -30610,
  -30569, -30527, -30484, -30440,
  -30396, -30351, -30305, -30258,
  -30211, -30162, -30113, -30063,
  -30012, -29959, -29906, -29853,
  -29798, -29742, -29685, -29627,
  -29568, -29508, -29447, -29385,
  -29321, -29257, -29191, -29125,
  -29057, -28988, -28918, -28846,
  -28774, -28700, -28625, -28548,
  -28470, -28391, -28311, -28229,
  -28146, -28061, -27975, -27887,
  -27798, -27708, -27616, -27522,
  -27427, -27331, -27232, -27132,
  -27031, -26928, -26823, -26717,
  -26609, -26499, -26387, -26274,
  -26158, -26041, -25923, -25802,
  -25679, -25555, -25428, -25300,
  -25170, -25038, -24904, -24767,
  -24629, -24489, -24346, -24202,
  -24056, -23907, -23756, -23603,
  -23448, -23291, -23131, -22970,
  -22806, -22640, -22471, -22301,
  -22128, -21952, -21775, -21595,
  -21413, -21228, -21041, -20852,
  -20660, -20466, -20270, -20071,
  -19870, -19666, -19460, -19252,
  -19041, -18828, -18613, -18395,
  -18174, -17951, -17726, -17499,
  -17269, -17036, -16802, -16565,
  -16325, -16083, -15839, -15593,
  -15344, -15093, -14840, -14584,
  -14327, -14067, -13805, -13540,
  -13274, -13005, -12735, -12462,
  -12187, -11910, -11632, -11351,
  -11068, -10784, -10498, -10210,
   -9920,  -9628,  -9335,  -9040,
   -8744,  -8446,  -8146,  -7845,
   -7543,  -7239,  -6934,  -6628,
   -6320,  -6012,  -5702,  -5391,
   -5079,  -4766,  -4453,  -4138,
   -3823,  -3507,  -3190,  -2873,
   -2555,  -2237,  -1918,  -1599,
   -1279,   -960,   -640,   -320,
       0,    320,    640,    960,
    1279,   1599,   1918,   2237,
    2555,   2873,   3190,   3507,
    3823,   4138,   4453,   4766,
    5079,   5391,   5702,   6012,
    6320,   6628,   6934,   7239,
    7543,   7845,   8146,   8446,
    8744,   9040,   9335,   9628,
    9920,  10210,  10498,  10784,
   11068,  11351,  11632,  11910,
   12187,  12462,  12735,  13005,
   13274,  13540,  13805,  14067,
   14327,  14584,  14840,  15093,
   15344,  15593,  15839,  16083,
   16325,  16565,  16802,  17036,
   17269,  17499,  17726,  17951,
   18174,  18395,  18613,  18828,
   19041,  19252,  19460,  19666,
   19870,  20071,  20270,  20466,
   20660,  20852,  21041,  21228,
   21413,  21595,  21775,  21952,
   22128,  22301,  22471,  22640,
   22806,  22970,  23131,  23291,
   23448,  23603,  23756,  23907,
   24056,  24202,  24346,  24489,
   24629,  24767,  24904,  25038,
   25170,  25300,  25428,  25555,
   25679,  25802,  25923,  26041,
   26158,  26274,  26387,  26499,
   26609,  26717,  26823,  26928,
   27031,  27132,  27232,  27331,
   27427,  27522,  27616,  27708,
   27798,  27887,  27975,  28061,
   28146,  28229,  28311,  28391,
   28470,  28548,  28625,  28700,
   28774,  28846,  28918,  28988,
   29057,  29125,  29191,  29257,
   29321,  29385,  29447,  29508,
   29568,  29627,  29685,  29742,
   29798,  29853,  29906,  29959,
   30012,  30063,  30113,  30162,
   30211,  30258,  30305,  30351,
   30396,  30440,  30484,  30527,
   30569,  30610,  30650,  30690,
   30729,  30767,  30805,  30842,
   30878,  30913,  30948,  30983,
   31016,  31049,  31082,  31113,
   31145,  31175,  31205,  31235,
   31264,  31293,  31320,  31348,
   31375,  31401,  31427,  31453,
   31478,  31502,  31526,  31550,
   31573,  31596,  31618,  31640,
   31661,  31682,  31703,  31723,
   31743,  31763,  31782,  31801,
   31819,  31838,  31855,  31873,
   31890,  31907,  31923,  31939,
   31955,  31971,  31986,  32001,
   32016,  32030,  32044,  32058,
   32072,  32085,  32098,  32111,
   32124,  32136,  32148,  32160,
   32172,  32183,  32195,  32206,
   32216,  32227,  32237,  32248,
   32258,  32268,  32277,  32287,
   32296,  32305,  32314,  32323,
   32331,  32340,  32348,  32356,
   32364,  32372,  32380,  32387,
   32394,  32402,  32409,  32416,
   32422,  32429,  32436,  32442,
   32448,  32455,  32461,  32467,
   32473,  32478,  32484,  32489,
   32495,  32500,  32505,  32510,
   32515,  32520,  32525,  32530,
   32534,  32539,  32543,  32548,
   32552,  32556,  32560,  32564,
   32568,  32572,  32576,  32580,
   32584,  32587,  32591,  32594,
   32597,  32601,  32604,  32607,
   32610,  32614,  32617,  32619,
   32622,  32625,  32628,  32631,
   32633,  32636,  32639,  32641,
   32644,  32646,  32649,  32651,
   32653,  32655,  32658,  32660,
   32662,  32664,  32666,  32668,
   32670,  32672,  32674,  32676,
   32678,  32679,  32681,  32683,
   32684,  32686,  32688,  32689,
   32691,  32692,  32694,  32695,
   32697,  32698,  32700,  32701,
   32702,  32704,  32705,  32706,
   32707,  32709,  32710,  32711,
   32712,  32713,  32714,  32715,
   32716,  32717,  32718,  32719,
   32720,  32721,  32722,  32723,
   32724,  32725,  32726,  32727,
   32728,  32728,  32729,  32730,
   32731,  32732,  32732,  32733,
   32734,  32734,  32735,  32736,
   32736,  32737,  32738,  32738,
   32739,  32740,  32740,  32741,
   32741,  32742,  32742,  32743,
   32743,  32744,  32744,  32745,
   32745,  32746,  32746,  32747,
   32747,  32748,  32748,  32749,
   32749,  32749,  32750,  32750,
   32751,  32751,  32751,  32752,
   32752,  32752,  32753,  32753,
   32753,  32754,  32754,  32754,
   32755,  32755,  32755,  32756,
   32756,  32756,  32756,  32757,
   32757,  32757,  32757,  32758,
   32758,  32758,  32758,  32759,
   32759,  32759,  32759,  32759,
   32760,  32760,  32760,  32760,
   32760,  32761,  32761,  32761,
   32761,  32761,  32761,  32762,
   32762,  32762,  32762,  32762,
   32762,  32763,  32763,  32763,
   32763,  32763,  32763,  32763,
   32763,  32764,  32764,  32764,
   32764,  32764,  32764,  32764,
   32764,  32765,  32765,  32765,
   32765,  32765,  32765,  32765,
   32765,  32765,  32765,  32765,
   32766,  32766,  32766,  32766,
   32766,  32766,  32766,  32766,
   32766,  32766,  32766,  32766,
   32766,  32767,  32767,  32767,
   32767,  32767,  32767,  32767,
   32767,
};

const int16_t wav_sine[] = {
       0,    201,    402,    603,
     804,   1005,   1206,   1406,
    1607,   1808,   2009,   2209,
    2410,   2610,   2811,   3011,
    3211,   3411,   3611,   3811,
    4011,   4210,   4409,   4608,
    4807,   5006,   5205,   5403,
    5601,   5799,   5997,   6195,
    6392,   6589,   6786,   6982,
    7179,   7375,   7571,   7766,
    7961,   8156,   8351,   8545,
    8739,   8932,   9126,   9319,
    9511,   9703,   9895,  10087,
   10278,  10469,  10659,  10849,
   11038,  11227,  11416,  11604,
   11792,  11980,  12166,  12353,
   12539,  12724,  12909,  13094,
   13278,  13462,  13645,  13827,
   14009,  14191,  14372,  14552,
   14732,  14911,  15090,  15268,
   15446,  15623,  15799,  15975,
   16150,  16325,  16499,  16672,
   16845,  17017,  17189,  17360,
   17530,  17699,  17868,  18036,
   18204,  18371,  18537,  18702,
   18867,  19031,  19194,  19357,
   19519,  19680,  19840,  20000,
   20159,  20317,  20474,  20631,
   20787,  20942,  21096,  21249,
   21402,  21554,  21705,  21855,
   22004,  22153,  22301,  22448,
   22594,  22739,  22883,  23027,
   23169,  23311,  23452,  23592,
   23731,  23869,  24006,  24143,
   24278,  24413,  24546,  24679,
   24811,  24942,  25072,  25201,
   25329,  25456,  25582,  25707,
   25831,  25954,  26077,  26198,
   26318,  26437,  26556,  26673,
   26789,  26905,  27019,  27132,
   27244,  27355,  27466,  27575,
   27683,  27790,  27896,  28001,
   28105,  28208,  28309,  28410,
   28510,  28608,  28706,  28802,
   28897,  28992,  29085,  29177,
   29268,  29358,  29446,  29534,
   29621,  29706,  29790,  29873,
   29955,  30036,  30116,  30195,
   30272,  30349,  30424,  30498,
   30571,  30643,  30713,  30783,
   30851,  30918,  30984,  31049,
   31113,  31175,  31236,  31297,
   31356,  31413,  31470,  31525,
   31580,  31633,  31684,  31735,
   31785,  31833,  31880,  31926,
   31970,  32014,  32056,  32097,
   32137,  32176,  32213,  32249,
   32284,  32318,  32350,  32382,
   32412,  32441,  32468,  32495,
   32520,  32544,  32567,  32588,
   32609,  32628,  32646,  32662,
   32678,  32692,  32705,  32717,
   32727,  32736,  32744,  32751,
   32757,  32761,  32764,  32766,
   32767,  32766,  32764,  32761,
   32757,  32751,  32744,  32736,
   32727,  32717,  32705,  32692,
   32678,  32662,  32646,  32628,
   32609,  32588,  32567,  32544,
   32520,  32495,  32468,  32441,
   32412,  32382,  32350,  32318,
   32284,  32249,  32213,  32176,
   32137,  32097,  32056,  32014,
   31970,  31926,  31880,  31833,
   31785,  31735,  31684,  31633,
   31580,  31525,  31470,  31413,
   31356,  31297,  31236,  31175,
   31113,  31049,  30984,  30918,
   30851,  30783,  30713,  30643,
   30571,  30498,  30424,  30349,
   30272,  30195,  30116,  30036,
   29955,  29873,  29790,  29706,
   29621,  29534,  29446,  29358,
   29268,  29177,  29085,  28992,
   28897,  28802,  28706,  28608,
   28510,  28410,  28309,  28208,
   28105,  28001,  27896,  27790,
   27683,  27575,  27466,  27355,
   27244,  27132,  27019,  26905,
   26789,  26673,  26556,  26437,
   26318,  26198,  26077,  25954,
   25831,  25707,  25582,  25456,
   25329,  25201,  25072,  24942,
   24811,  24679,  24546,  24413,
   24278,  24143,  24006,  23869,
   23731,  23592,  23452,  23311,
   23169,  23027,  22883,  22739,
   22594,  22448,  22301,  22153,
   22004,  21855,  21705,  21554,
   21402,  21249,  21096,  20942,
   20787,  20631,  20474,  20317,
   20159,  20000,  19840,  19680,
   19519,  19357,  19194,  19031,
   18867,  18702,  18537,  18371,
   18204,  18036,  17868,  17699,
   17530,  17360,  17189,  17017,
   16845,  16672,  16499,  16325,
   16150,  15975,  15799,  15623,
   15446,  15268,  15090,  14911,
   14732,  14552,  14372,  14191,
   14009,  13827,  13645,  13462,
   13278,  13094,  12909,  12724,
   12539,  12353,  12166,  11980,
   11792,  11604,  11416,  11227,
   11038,  10849,  10659,  10469,
   10278,  10087,   9895,   9703,
    9511,   9319,   9126,   8932,
    8739,   8545,   8351,   8156,
    7961,   7766,   7571,   7375,
    7179,   6982,   6786,   6589,
    6392,   6195,   5997,   5799,
    5601,   5403,   5205,   5006,
    4807,   4608,   4409,   4210,
    4011,   3811,   3611,   3411,
    3211,   3011,   2811,   2610,
    2410,   2209,   2009,   1808,
    1607,   1406,   1206,   1005,
     804,    603,    402,    201,
       0,   -201,   -402,   -603,
    -804,  -1005,  -1206,  -1406,
   -1607,  -1808,  -2009,  -2209,
   -2410,  -2610,  -2811,  -3011,
   -3211,  -3411,  -3611,  -3811,
   -4011,  -4210,  -4409,  -4608,
   -4807,  -5006,  -5205,  -5403,
   -5601,  -5799,  -5997,  -6195,
   -6392,  -6589,  -6786,  -6982,
   -7179,  -7375,  -7571,  -7766,
   -7961,  -8156,  -8351,  -8545,
   -8739,  -8932,  -9126,  -9319,
   -9511,  -9703,  -9895, -10087,
  -10278, -10469, -10659, -10849,
  -11038, -11227, -11416, -11604,
  -11792, -11980, -12166, -12353,
  -12539, -12724, -12909, -13094,
  -13278, -13462, -13645, -13827,
  -14009, -14191, -14372, -14552,
  -14732, -14911, -15090, -15268,
  -15446, -15623, -15799, -15975,
  -16150, -16325, -16499, -16672,
  -16845, -17017, -17189, -17360,
  -17530, -17699, -17868, -18036,
  -18204, -18371, -18537, -18702,
  -18867, -19031, -19194, -19357,
  -19519, -19680, -19840, -20000,
  -20159, -20317, -20474, -20631,
  -20787, -20942, -21096, -21249,
  -21402, -21554, -21705, -21855,
  -22004, -22153, -22301, -22448,
  -22594, -22739, -22883, -23027,
  -23169, -23311, -23452, -23592,
  -23731, -23869, -24006, -24143,
  -24278, -24413, -24546, -24679,
  -24811, -24942, -25072, -25201,
  -25329, -25456, -25582, -25707,
  -25831, -25954, -26077, -26198,
  -26318, -26437, -26556, -26673,
  -26789, -26905, -27019, -27132,
  -27244, -27355, -27466, -27575,
  -27683, -27790, -27896, -28001,
  -28105, -28208, -28309, -28410,
  -28510, -28608, -28706, -28802,
  -28897, -28992, -29085, -29177,
  -29268, -29358, -29446, -29534,
  -29621, -29706, -29790, -29873,
  -29955, -30036, -30116, -30195,
  -30272, -30349, -30424, -30498,
  -30571, -30643, -30713, -30783,
  -30851, -30918, -30984, -31049,
  -31113, -31175, -31236, -31297,
  -31356, -31413, -31470, -31525,
  -31580, -31633, -31684, -31735,
  -31785, -31833, -31880, -31926,
  -31970, -32014, -32056, -32097,
  -32137, -32176, -32213, -32249,
  -32284, -32318, -32350, -32382,
  -32412, -32441, -32468, -32495,
  -32520, -32544, -32567, -32588,
  -32609, -32628, -32646, -32662,
  -32678, -32692, -32705, -32717,
  -32727, -32736, -32744, -32751,
  -32757, -32761, -32764, -32766,
  -32767, -32766, -32764, -32761,
  -32757, -32751, -32744, -32736,
  -32727, -32717, -32705, -32692,
  -32678, -32662, -32646, -32628,
  -32609, -32588, -32567, -32544,
  -32520, -32495, -32468, -32441,
  -32412, -32382, -32350, -32318,
  -32284, -32249, -32213, -32176,
  -32137, -32097, -32056, -32014,
  -31970, -31926, -31880, -31833,
  -31785, -31735, -31684, -31633,
  -31580, -31525, -31470, -31413,
  -31356, -31297, -31236, -31175,
  -31113, -31049, -30984, -30918,
  -30851, -30783, -30713, -30643,
  -30571, -30498, -30424, -30349,
  -30272, -30195, -30116, -30036,
  -29955, -29873, -29790, -29706,
  -29621, -29534, -29446, -29358,
  -29268, -29177, -29085, -28992,
  -28897, -28802, -28706, -28608,
  -28510, -28410, -28309, -28208,
  -28105, -28001, -27896, -27790,
  -27683, -27575, -27466, -27355,
  -27244, -27132, -27019, -26905,
  -26789, -26673, -26556, -26437,
  -26318, -26198, -26077, -25954,
  -25831, -25707, -25582, -25456,
  -25329, -25201, -25072, -24942,
  -24811, -24679, -24546, -24413,
  -24278, -24143, -24006, -23869,
  -23731, -23592, -23452, -23311,
  -23169, -23027, -22883, -22739,
  -22594, -22448, -22301, -22153,
  -22004, -21855, -21705, -21554,
  -21402, -21249, -21096, -20942,
  -20787, -20631, -20474, -20317,
  -20159, -20000, -19840, -19680,
  -19519, -19357, -19194, -19031,
  -18867, -18702, -18537, -18371,
  -18204, -18036, -17868, -17699,
  -17530, -17360, -17189, -17017,
  -16845, -16672, -16499, -16325,
  -16150, -15975, -15799, -15623,
  -15446, -15268, -15090, -14911,
  -14732, -14552, -14372, -14191,
  -14009, -13827, -13645, -13462,
  -13278, -13094, -12909, -12724,
  -12539, -12353, -12166, -11980,
  -11792, -11604, -11416, -11227,
  -11038, -10849, -10659, -10469,
  -10278, -10087,  -9895,  -9703,
   -9511,  -9319,  -9126,  -8932,
   -8739,  -8545,  -8351,  -8156,
   -7961,  -7766,  -7571,  -7375,
   -7179,  -6982,  -6786,  -6589,
   -6392,  -6195,  -5997,  -5799,
   -5601,  -5403,  -5205,  -5006,
   -4807,  -4608,  -4409,  -4210,
   -4011,  -3811,  -3611,  -3411,
   -3211,  -3011,  -2811,  -2610,
   -2410,  -2209,  -2009,  -1808,
   -1607,  -1406,  -1206,  -1005,
    -804,   -603,   -402,   -201,
       0,
};

const uint16_t lut_env_expo[] = {
       0,   1035,   2054,   3057,
    4045,   5018,   5975,   6918,
    7846,   8760,   9659,  10545,
   11416,  12275,  13120,  13952,
   14771,  15577,  16371,  17152,
   17921,  18679,  19425,  20159,
   20881,  21593,  22294,  22983,
   23662,  24331,  24989,  25637,
   26274,  26902,  27520,  28129,
   28728,  29318,  29899,  30471,
   31034,  31588,  32133,  32670,
   33199,  33720,  34232,  34737,
   35233,  35722,  36204,  36678,
   37145,  37604,  38056,  38502,
   38940,  39371,  39796,  40215,
   40626,  41032,  41431,  41824,
   42211,  42592,  42967,  43336,
   43699,  44057,  44409,  44756,
   45097,  45434,  45764,  46090,
   46411,  46727,  47037,  47344,
   47645,  47941,  48233,  48521,
   48804,  49083,  49357,  49627,
   49893,  50155,  50412,  50666,
   50916,  51162,  51404,  51642,
   51877,  52108,  52335,  52559,
   52780,  52997,  53210,  53421,
   53628,  53831,  54032,  54230,
   54424,  54616,  54804,  54990,
   55173,  55353,  55530,  55704,
   55876,  56045,  56211,  56375,
   56536,  56695,  56851,  57005,
   57157,  57306,  57453,  57597,
   57740,  57880,  58018,  58153,
   58287,  58419,  58548,  58676,
   58801,  58925,  59047,  59167,
   59285,  59401,  59515,  59628,
   59739,  59848,  59955,  60061,
   60165,  60267,  60368,  60468,
   60566,  60662,  60757,  60850,
   60942,  61032,  61121,  61209,
   61295,  61380,  61464,  61546,
   61628,  61707,  61786,  61863,
   61939,  62014,  62088,  62161,
   62233,  62303,  62372,  62441,
   62508,  62574,  62639,  62703,
   62767,  62829,  62890,  62950,
   63010,  63068,  63125,  63182,
   63238,  63293,  63347,  63400,
   63452,  63504,  63554,  63604,
   63654,  63702,  63750,  63797,
   63843,  63888,  63933,  63977,
   64021,  64063,  64105,  64147,
   64188,  64228,  64267,  64306,
   64344,  64382,  64419,  64456,
   64492,  64527,  64562,  64596,
   64630,  64664,  64696,  64729,
   64760,  64792,  64822,  64853,
   64883,  64912,  64941,  64969,
   64997,  65025,  65052,  65079,
   65105,  65131,  65157,  65182,
   65206,  65231,  65255,  65278,
   65302,  65324,  65347,  65369,
   65391,  65412,  65434,  65454,
   65475,  65495,  65515,  65535,
   65535,
};