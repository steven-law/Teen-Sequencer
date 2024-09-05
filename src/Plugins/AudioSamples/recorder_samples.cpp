#include "Plugins/AudioSamples/AudioSamples.h"

PROGMEM const AudioSynthWavetable::sample_data recorder_samples[1] = {
	{
		(int16_t*)sample_0_recorder_recorderax2, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * WAVETABLE_CENTS_SHIFT(33) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(92) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1359 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1352 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1352 - 1) << (32 - 11)) - (((uint32_t)1298 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-5.2)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(6.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(11848.78 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(100.02 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-3.4)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(260.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(5.0 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(9) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(-9)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

PROGMEM const uint32_t sample_0_recorder_recorderax2[768] = {
0x00000000,0x02e10679,0x043ffe86,0x006b01bd,0xffea001a,0x023a031b,0xff5ffee7,0x04fb03d5,
0xfe33ff10,0x013302c3,0x04c00142,0xffcd0211,0x06d2034d,0x008d037a,0x044d02fa,0x004c0062,
0x0637088b,0xff92fd40,0x06c7047b,0xff17ff39,0x017a05ac,0x035801c3,0x017a0219,0x025d0003,
0x0187041d,0x05f4028b,0xfd8202a3,0x02d90032,0x03e801e0,0x00ec0499,0xff43fd13,0x05e10511,
0xfbefff0a,0x01d00069,0x033d0192,0x01560433,0xfe84fd9b,0x0972048e,0xfd26010d,0x06c5066c,
0x0410027d,0x04210520,0x029e0209,0x008f02ee,0x040a023e,0xfc37fefd,0x05970310,0xfa2dfd25,
0x055201dd,0xfe8102e7,0xffe0fcc3,0x05ed05c0,0xfe3efda2,0x0356036d,0xfffd0268,0x030eff26,
0x027d048f,0x015801d6,0x01a9021f,0xff1cff7a,0x02be007e,0x0362051a,0xfd52fc31,0x03a805a4,
0x043a0183,0x02b20274,0x01f303da,0x064902da,0xfd8e037d,0x029a0013,0x0031fef9,0xfed503bc,
0xff48fc34,0xffb1ff39,0xfef80022,0xff13ff2d,0x0070ff5b,0x00f10128,0x022a0117,0xffd4004b,
0x07bf0489,0xfe7b0461,0x09800168,0xff5e0788,0x01fafdfe,0x042f0682,0xfd80fc5f,0x0068fffb,
0xfb0b00c1,0x0556fed6,0x00c10359,0x00b4fe9f,0x03270346,0x026a0392,0xfeab00d6,0x010cfd4a,
0x00cb0423,0xff35fe1e,0xfdd3fe91,0x0001ffb8,0x012d002e,0xfcf2fe7c,0x017afffe,0x00c3006e,
0x0067012d,0x04050182,0x055d0519,0x044404e8,0x05a50574,0x03b80391,0x01dd0497,0xfd1ffe70,
0xffb5fe01,0xfb9dff56,0xfdcef9a8,0xfebc0068,0x0099feda,0xffe100bc,0x0185008b,0x02140279,
0xfe2efe66,0x029701ab,0x01f7017b,0x035004ae,0x0084fe7d,0x032105d5,0x00f5ff2c,0xfe290105,
0xfda7fc61,0x009400ff,0xfe09fe0a,0x04e2fffc,0x03b20715,0x08e50542,0x04620726,0x033603b1,
0x00b30154,0xfb9aff75,0xf966f911,0xfd7cfb51,0xfa40fc79,0x0231fcfb,0xfd7d01cc,0xfffdfc94,
0x0163041d,0xfe5efc51,0x064304ae,0xfeb9ff39,0x0b2107ed,0x03080660,0x08b6053f,0x04b1087d,
0xfe0fff99,0xf971fdd7,0xfcadf96e,0xf9a8faea,0xfce5fb9d,0x038eff86,0x054d04c6,0x0777073d,
0x02d804ee,0x01e10356,0xfc52fe8b,0xfc30fb77,0xfb46fbf2,0xfe94fc0f,0x00f500b8,0x002c0132,
0x0106ff45,0xfdd500cf,0xfbf3fc37,0xfef9fd21,0x008efe77,0x0a0407aa,0x0914074b,0x0db90d38,
0x08dc0ae3,0x01ad05ea,0xfa95ff3a,0xf38ef585,0xf550f42c,0xf446f4a2,0xfd30f777,0x04b80282,
0x046b0404,0x028204ad,0x063002b9,0xfbc8030e,0x00e6fc78,0xfd32ff86,0x03890132,0x04d1025e,
0x065e06c8,0x02ac0522,0xfbcdffc7,0xf48ff6ac,0xf852f759,0xfb33f827,0xfff3fe55,0x0c080591,
0x0dee0d95,0x0d1f0dbc,0x07ed0b8c,0xfd570322,0xf43ef816,0xf12ff0e7,0xf28ef387,0xf63df2a3,
0x0081faae,0x04520443,0x049d05b9,0xfdf3ff73,0xfba7fc7a,0x01cc0061,0x03b80032,0x0914081b,
0x099e092c,0x0acf09f3,0x08ae0b41,0xfec303e5,0xf3cff8e8,0xec59ecc3,0xf0c5f3d8,0xf96eefaa,
0xfcfbfcf0,0x10670669,0x0ae30b5e,0x09341092,0x053b05b0,0xfa5dfeac,0xf953f987,0xf66cf718,
0xfabcf938,0xff87fc3c,0x03b40390,0x025500d6,0xf9420134,0xf2cff2bb,0xfbeaf8dc,0x0619ff68,
0x09790733,0x0fa70f7e,0x0e660df0,0x100d0e8c,0x02950d9f,0xf9e5fdce,0xe8f3eed6,0xef05ed47,
0xe83fe8b9,0xf5c4f014,0x058afcbf,0x0b4a081b,0x060a0c21,0x06ee05f0,0xfad1fe14,0x06f00559,
0x05cb00d1,0x05db08d3,0x063e0621,0x0706058b,0x01750670,0xf417f993,0xeec3f04c,0xed52ee6e,
0xf9f5f368,0xff39f802,0x0cf20c34,0x0e690c00,0x14e6123a,0x0d961211,0xfe3206f4,0xf172f5ea,
0xf3f5f3ed,0xecf5f00e,0xf0a7ead1,0x00bafc0d,0x02910107,0xfe9c0013,0xf882fdf1,0xfc05f5fb,
0x064f032d,0x0f800ae5,0x12d41334,0x0b4b0df4,0x0def0aba,0x0ae01086,0xf77800fd,0xeda6f3a7,
0xe48ee47d,0xea2ce966,0xebe9e78f,0xfcdcf693,0x0ca50352,0x0fa90f4d,0x0c6d0f79,0x056d0bc0,
0x033c0061,0x01d50453,0x037b03ca,0xfd44fc7d,0x00da01e6,0xff51021f,0xf306f395,0xe68ef224,
0xf2ace9eb,0xf952f1fb,0x02120276,0x175e09e8,0x0cad1432,0x156f1199,0x1668149a,0x041713bf,
0xf5cbf96c,0xed18ee52,0xe7ceec1a,0xdf73e3f8,0xf46ae536,0x027ffec5,0xfd97014e,0x026d0034,
0xfbd5fd19,0x0e540546,0x13b210f3,0x1a5318d8,0x0de513a0,0x09830ab3,0x0af30cca,0xf3a80176,
0xe5c9ea00,0xe555e3ab,0xe4d8e61e,0xeac8e4d9,0xfd59f560,0x0c8903bb,0x15a4126c,0x15a7179a,
0x16cb16e6,0x02570d17,0x0131ffdb,0xfc160035,0xf2b3f569,0xfc3ff6e9,0xf577fcfb,0xebc0ec52,
0xe765ec42,0xf13ae9c9,0xf903f4c6,0x0f340397,0x1c681807,0x1bf11ca2,0x1330170c,0x18cd15ef,
0x0ad21321,0xf3a9006e,0xe6aae86e,0xe031e8bf,0xdb5ada7d,0xe2d1dd06,0xfc9bedaa,0x00700378,
0x044a010e,0x0a030638,0x1816107e,0x1d1f1b8d,0x1c161e7f,0x13451796,0x0b420d71,0xfe4b06ce,
0xe74afb25,0xe170dc51,0xd9cad8f8,0xdebfe140,0xeff8e1c2,0xff5ffa75,0x167d0af5,0x1da21c16,
0x1e451af3,0x1ea02202,0x0d79171a,0xfdb9052d,0xf525f91d,0xe8feef26,0xe915e616,0xea7cee5f,
0xe3e4def0,0xe5a3ec9e,0xf13be6c4,0xffbef67e,0x245c151c,0x251a2051,0x23b62bee,0x1d851ff1,
0x155617cc,0x0a07136c,0xe959f8cc,0xdd1de39d,0xd3a2d446,0xd216d2e7,0xdd23d58c,0xf2b5e800,
0x0363fea2,0x0c8c0549,0x1e76147f,0x256c2401,0x27cd28cb,0x1c6421d0,0x11ed1892,0x08960bd5,
0xf2e90258,0xcb01dadd,0xd282ce27,0xd13ccdec,0xdf8cdaa4,0xecf3e467,0x154c01ff,0x25d31b41,
0x29ab2ab8,0x2a052b47,0x2250269f,0x0fe41b0f,0xf8ff030f,0xec22f552,0xdb6ddee3,0xd2e7d931,
0xd796d24b,0xde12dafc,0xea15e4ec,0xef49eb1d,0x1ccbfdf0,0x331235fc,0x36da30b5,0x2e74345f,
0x22ff296c,0x17fb1b09,0xfb2813a9,0xd7e3e0f0,0xc760d1ec,0xc4cec07a,0xcccac9e4,0xe10ad4cd,
0xf663eda1,0x095a00b3,0x2309112e,0x36713399,0x37bd36d4,0x254a2ff2,0x1b9c200a,0x0c7013c9,
0xfe910642,0xca0aeb1e,0xc0c6b7b0,0xc271c5a9,0xd565cb2a,0xe5d3dd06,0xfb5aeee1,0x2d2215a8,
0x34c42df4,0x3b5c3fcf,0x331a36e7,0x1c8e2858,0x0c62143d,0xf307fed7,0xd98de826,0xc406cf5e,
0xbb5bb98d,0xd060c303,0xdf30dbe4,0xea88e2f5,0x0355f398,0x41722977,0x41783bf4,0x3f3f461a,
0x34b93a43,0x20802a19,0x0b7c194a,0xebd30075,0xbbe2c912,0xb38abbd2,0xc039b538,0xd56dc910,
0xe72ae03f,0xfefcf1e4,0x18200b42,0x46472e34,0x430f4768,0x399f4615,0x22622a55,0x12171871,
0x04fc0d42,0xdb03f73f,0xa9a9b725,0xb7d0b560,0xc3d4b6d6,0xd9add172,0xedf3e1ad,0x1a6d024d,
0x39422af4,0x4aa24806,0x495548a4,0x31bd40dc,0x1642236a,0x072d0e4d,0xe376fa03,0xbf69d0b6,
0xa5eeaa15,0xbc10b1b3,0xd4ffc581,0xe19de08b,0xf146e743,0x2eb709bc,0x464540ad,0x52094e71,
0x43094a24,0x2d7c3b8a,0x15281fb4,0xfc1f0e1c,0xc689e3c2,0xae85b1c0,0xac3baad3,0xc11fb383,
0xdf26d2e4,0xebd5e4b1,0x0fb5f9b4,0x3cdc2689,0x548451a6,0x4f26500f,0x314d41ff,0x1c1027fc,
0x0bae107d,0xeeee0663,0xaa7ec593,0xacb5a4f7,0xaec4a9b7,0xcb23bf64,0xdf8bd74d,0xf997eb0d,
0x349f1511,0x4dd83e83,0x56a0594e,0x497b55b2,0x29a338e0,0x0e1d19ee,0xfd1e06be,0xcdceea90,
0x9abaaa1b,0xab58a73b,0xbf21aba5,0xd971d13f,0xe647e028,0x0d45f122,0x482334db,0x587b4e4e,
0x520f5969,0x3d3b48f5,0x20aa304f,0x06d20f5a,0xe5fd00f4,0xa5afc262,0xa94da1a1,0xa5c4a2e0,
0xce7cbbd1,0xe1efd8d9,0xfab2ea77,0x2e52108e,0x57bc4865,0x5b1d5e8d,0x4965559c,0x2c773954,
0x14211f88,0x034909a8,0xc2c5ef19,0x9a689c2d,0xa249ad56,0xba3ca1a2,0xd37cc7c4,0xe84bdf5d,
0x16b7f981,0x45a030af,0x5fb05b18,0x5d8d5dff,0x3cef4f5c,0x1bb92beb,0x06d310be,0xebb9fa11,
0x9dc2d1bf,0xabc68dba,0x9f77a497,0xcd82bcc0,0xe089d820,0xf320e4fb,0x3b1910d3,0x519d4cd2,
0x60946038,0x4e165788,0x32f4422e,0x10e02105,0x01b009c9,0xbff4e382,0x9b00a861,0x9dfb9def,
0xb627a0c5,0xd73bcb1c,0xea06e0a9,0x105ef6b5,0x4f5f33f3,0x6116599d,0x5a406440,0x409f4dc2,
0x23d130b4,0x078713b9,0xe94b03bd,0x9f77be47,0xa1889aff,0xa1b89de9,0xc4a3b25c,0xda97d181,
0xf7efe7a4,0x334e0ff1,0x5d2d534c,0x698a600d,0x512f63ec,0x32d34298,0x1159210d,0x02790690,
0xc35bed42,0x90989cfe,0x9d629eef,0xb55a9c3b,0xd430c816,0xe528e01a,0x12e2f6e0,0x513b3050,
0x5c3b5e30,0x64c66408,0x4497544d,0x23c635ad,0x0cc71445,0xe74efeb2,0x9a43c714,0xa2c29255,
0x98f5983d,0xc50fb267,0xe22bd22d,0xf3e8e8ef,0x337713b6,0x661250cd,0x6581649a,0x542a659d,
0x3427421f,0x128b2613,0x06bf0b30,0xc0d1e266,0x91199cce,0x9a7aa570,0xb13299e7,0xd13fc3a6,
0xe7f6dcea,0x0b4ff26c,0x53fa339c,0x639f6116,0x68c16b6a,0x46c156a7,0x244835a0,0x0ad4155b,
0xec0d01a3,0x9585c2ac,0x9ee68ebd,0x9fe3958a,0xbf14b002,0xde7ad46f,0xf211e5d3,0x32fe0c86,
0x61ce5426,0x6bba622d,0x55db6a08,0x36414618,0x14f02497,0x01aa0ad7,0xc3c8ebf6,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};
