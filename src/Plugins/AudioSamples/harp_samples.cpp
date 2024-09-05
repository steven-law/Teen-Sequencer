#include "Plugins/AudioSamples/AudioSamples.h"

PROGMEM const AudioSynthWavetable::sample_data harp_samples[1] = {
	{
		(int16_t*)sample_0_harp_pluckharp, // sample
		true, // LOOP
		12, // LENGTH_BITS
		(1 << (32 - 12)) * WAVETABLE_CENTS_SHIFT(25) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(89) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)3477 - 1) << (32 - 12), // MAX_PHASE
		((uint32_t)3473 - 1) << (32 - 12), // LOOP_PHASE_END
		(((uint32_t)3473 - 1) << (32 - 12)) - (((uint32_t)3409 - 1) << (32 - 12)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-6.0)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(6.20 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(4289.57 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(2506.78 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-100.0)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(5.4 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

PROGMEM const uint32_t sample_0_harp_pluckharp[1792] = {
0x00000000,0x17bc1494,0x29dc3287,0x134c12fe,0x270c0b11,0x1ae41fa4,0xf60cfe7e,0xfb68e65e,
0xfa06e827,0xf40df78e,0x0fd81085,0xd081fa20,0xc95ec9cb,0xd0ead69b,0xd0e8e0c9,0x0df2f9fd,
0x0edb0f93,0x2c210f09,0x184f1e14,0x2df81041,0x3e802890,0x45024085,0x1d842ca5,0xf01b1fed,
0xe5b5fb00,0xd744e013,0xefd7f38c,0xaefad5cf,0xa317aba3,0xb2c7a931,0xccebae97,0x152cf26e,
0x24891b46,0x50c142bb,0x455a5106,0x3bb03f28,0x3eef49a7,0x3d795196,0x1afa32a2,0xe32cf077,
0xbbaedac5,0xc31aad2c,0xd6c9d1cc,0xbc0aba43,0xc048af1a,0xcf4bdc70,0xf25ce0e1,0x16a21623,
0x282c1563,0x433547b7,0x2d4b42ad,0x39a130f1,0x455f2cc1,0x3ff337eb,0x10c22904,0xf420f4d1,
0xc239da45,0xc97cb69f,0xc7c2d5a6,0xa3a4c315,0xbdfda7fc,0xc34bc69e,0xfad8d072,0x16450fbb,
0x478b2404,0x57304a68,0x538347c6,0x4e4a4c30,0x498858fd,0x41d348be,0xf5151d5a,0xd382e749,
0x9b25b8d7,0xb9d8a379,0xbe71b232,0xa306a4b6,0xc4eba1e3,0xcdefc8f9,0x1703efab,0x2ada2855,
0x552c4987,0x53786bb7,0x46e357b2,0x42144123,0x370c3f15,0x3690431a,0xfc260a68,0xda99e681,
0x9a8da4b1,0xb301a82c,0xbc4dc039,0x9e96af41,0xc523ba34,0xc90ec8fd,0x121ffcf7,0x2b4e2191,
0x60c54db8,0x62465dfc,0x4e4a5aad,0x3d0a4056,0x2e242ab5,0x1b09343f,0xe93b0002,0xcc9af02a,
0xa05dafca,0xbe3fb80e,0xc4cecb13,0xba0cb0cd,0xceefc354,0xdc58c652,0x146af3cc,0x3051131b,
0x59a74f6f,0x5e5e6443,0x46bf58e8,0x2cef438c,0x2e582b19,0x0b812de5,0xf33bf4b1,0xc8e0e3b5,
0xac3e9db5,0xc35ea81e,0xbb56c959,0xc15cad18,0xd0d4cb55,0xe485cf1a,0x116f08e4,0x419e2371,
0x65005939,0x67b666a3,0x53645d9c,0x30c43b80,0x2dca1eec,0xfd57187e,0xe179e1dc,0xb0ebd5e6,
0x9c29a10d,0xc7abaec2,0xbbb5cc5b,0xc93cc130,0xd2c2d6ff,0xf849d29f,0x16bb03c2,0x4459238e,
0x678f54ea,0x63405fce,0x4b1f5c30,0x217839fe,0x27c52d8c,0xf13b1bdd,0xe3aff168,0xb466d168,
0x9e329f93,0xcc92b796,0xc476c469,0xd173c1a9,0xce7cd8f8,0xf21ee130,0x0d120402,0x3db4234d,
0x5b215aee,0x5ce863b3,0x3eae5b51,0x1ee32a3a,0x29d52205,0xfb170a9b,0xe8eef57f,0xbaffd788,
0xaf81a307,0xcf58cc11,0xc164cdb9,0xd302d4b1,0xd59fd88e,0xee38e722,0x0036fc9f,0x321612a9,
0x54314217,0x62414cbf,0x3f7c525a,0x291524df,0x28c2332b,0x0a7c11a3,0xf0a9fe11,0xba73e4f3,
0xbc6bb41b,0xd201d04f,0xc1b2bf17,0xc80bc891,0xd235c477,0xeddbd779,0x00a1edc3,0x306b1430,
0x4c4342d6,0x618c56be,0x3bdc5a8d,0x37542f27,0x28333cb2,0x0ccd1be8,0xfd0efca8,0xbbd5d9fe,
0xc0fdad2c,0xcc5bce82,0xcb91bfcf,0xc26ac790,0xcc03c88e,0xe09adc8b,0xf4fcf2a0,0x27f51247,
0x44aa3dfc,0x65715540,0x37ed574b,0x3cc73147,0x2f8133de,0x14671f43,0x02510d4d,0xbf2de547,
0xc34bb627,0xc1f3d22a,0xc192c1a3,0xb898c030,0xc7d3bc44,0xe0ddd1a2,0xfb9aeaf9,0x300c1255,
0x4cc141c6,0x728c62bd,0x3cc2580a,0x38363f16,0x29c238c8,0x0c541963,0xf7520519,0xb2aad5cf,
0xc4c5b06d,0xc21dc38e,0xc63dbfbf,0xbf5fbcad,0xd012c3f2,0xe545d94e,0x008df121,0x349a1a8b,
0x511043cd,0x716471c3,0x433d5353,0x3c6c3cd7,0x250033d2,0x0c86156b,0xf56c0103,0xaf1ac955,
0xbd3eafdb,0xb696bc1b,0xbd7bbdfd,0xbabfbab2,0xd1afc576,0xeb30dc5b,0x0b29f7a4,0x3aae25f3,
0x61774388,0x6d4473cc,0x433b5471,0x39d23e37,0x1e56310e,0x0609112a,0xec200654,0xb03ec81a,
0xbbdfb4b6,0xbd48bc1d,0xc315c228,0xbfe4beea,0xd0bec908,0xe659da81,0x0650f032,0x2bb720d4,
0x5a8340db,0x68647080,0x43405222,0x3c4941bf,0x241334c8,0x11a914d2,0xf3460c4a,0xb760cae9,
0xb8d2b5db,0xb70bb7d1,0xbc86bd86,0xbd8bbb07,0xcf8bc950,0xe34adcd5,0x0ea2f485,0x2e621e53,
0x6239452e,0x69ea7217,0x480b5128,0x3f1d4015,0x1aba2f99,0x0eb91037,0xe7a30785,0xb563c6dd,
0xb543b38a,0xb84ab517,0xbecebe8a,0xc616bf24,0xd99dcc94,0xe85cddbe,0x12e1fd27,0x2fd71da1,
0x66524849,0x63c37117,0x447651ca,0x3c2f42a3,0x127327de,0x0e9f0de3,0xe1110067,0xb5d8c3aa,
0xb499b283,0xbb22b302,0xc1f8be37,0xc938c2ed,0xdcfcd6dc,0xef68e010,0x14c707d8,0x2fdb2006,
0x64264db2,0x5ca56b2f,0x3dcf47fc,0x35713d94,0x0d821d51,0x0d840de2,0xe113fd93,0xba67c866,
0xb66db704,0xbdf6bad1,0xc644c3fe,0xd10fc70f,0xdef5debf,0xf4ffe3f8,0x13cb09f9,0x2eb519e3,
0x5d8d48da,0x521862a0,0x36a93df3,0x2aab375a,0x0a8d137f,0x0d550f3c,0xe55aff15,0xc30ad0d9,
0xc0cbbee6,0xc6acc2b4,0xc9dccb4d,0xd819ca43,0xdd9bdd67,0xf599e236,0x0be206e2,0x2b0d152a,
0x5ac444ea,0x4c0c5e32,0x35ca3a11,0x249235a3,0x0d161015,0x0fc410ef,0xe954fe26,0xc5fed42f,
0xc237c1bd,0xc918c2db,0xc719ca78,0xd891cd32,0xdab1dcf7,0xf885e4ac,0x093204b5,0x2ca814f8,
0x5e7947df,0x4bd85d92,0x3cba3d6b,0x1fd0349a,0x0cda0ddd,0x0a7f1008,0xe727fb1c,0xc740d38e,
0xc23ac489,0xc9b6c6c6,0xc660c815,0xd9eacf53,0xda1cd91f,0xf9f9e7af,0x06ec025a,0x2cb11443,
0x5dbb4951,0x46405887,0x3bef3e2d,0x1b4c3209,0x11600ffd,0x0d3b13e4,0xebb7fef7,0xcda2d8d9,
0xc720c6ae,0xc9a7c984,0xc4aac48e,0xd4b2cec3,0xd858d423,0xf6bbe7b4,0x013cfde0,0x28e71024,
0x5b2948ff,0x48e85567,0x40e24351,0x1da7324b,0x14971317,0x0e6a14de,0xeb48fdf4,0xccd6d9b4,
0xc870c76b,0xc823cb96,0xc6bdc3c8,0xd253cf6c,0xd9aad262,0xf787e91a,0x00c7fc8f,0x294a0dbb,
0x57004814,0x47a0507c,0x401a44f5,0x1d733045,0x178c169a,0x0f4e18ff,0xecffff85,0xcc62da8f,
0xca2fc6de,0xc548c9a5,0xc8c3c2d1,0xd02aceb6,0xdae5d18a,0xf663e9c8,0xfcf8fa59,0x2bb70d65,
0x53ee4a20,0x49bf4ef3,0x400147af,0x1e532ef9,0x1bc818a1,0x110e1ae4,0xef4500e5,0xcbeddb6d,
0xcae9c8df,0xbfeec654,0xc63dc060,0xcaf4ca98,0xdaa3d00d,0xf6cfea86,0xfc52f86b,0x32531083,
0x54834d3d,0x4eda5157,0x40d64a72,0x1d352d36,0x1ae61927,0x0cc4185f,0xecb8ff15,0xcaa2d796,
0xc9b0caba,0xbedfc357,0xc806c206,0xccedca6d,0xde4ad26e,0xf981eedb,0xfd24f849,0x352a142c,
0x51fb4ba4,0x4eb451cb,0x3e064ab8,0x1afc2a4e,0x18641702,0x09921378,0xea50fda1,0xcc66d53d,
0xca54cccf,0xc0dec293,0xc98ec554,0xce92cbe8,0xe0f5d468,0xf962f227,0xfd39f690,0x36301770,
0x51624aa8,0x50c5521b,0x3ced4b1c,0x1b532905,0x17bd182f,0x0971112d,0xe6d0fc03,0xcca9d36c,
0xc654cc90,0xbf5ebf59,0xc7b6c425,0xcd17ca82,0xe422d49d,0xf8b7f52a,0x01bbf5c3,0x3aa81cf2,
0x53614c80,0x5419554b,0x3c494cf5,0x1ba227ae,0x14301865,0x07680ef6,0xe1c9f852,0xccebd10e,
0xc355cb1d,0xc004be27,0xc86bc441,0xcdc2ca78,0xe910d6d4,0xf704f748,0x0475f5f0,0x3bef2095,
0x54104c1b,0x55ba5765,0x39324d0f,0x1b6c25f1,0x11f11643,0x067f0e2c,0xde80f4a7,0xcdb7d0b2,
0xc0c5c8cf,0xbf86bd1a,0xc8b3c47c,0xce53ca92,0xef1edb80,0xf80efa68,0x0881f8a0,0x3c5c23e4,
0x53be4b5a,0x56c65788,0x36c94ac5,0x1a2e2562,0x0ed91300,0x026d0bb9,0xdb13ef3b,0xcd4ad0e8,
0xc093c794,0xc17cbe55,0xcb6dc79c,0xd1bdcc00,0xf40be182,0xf7eafb79,0x0b29f9dc,0x3b4324cd,
0x52084910,0x557557fa,0x33ee46f8,0x175223c1,0x0dec102d,0x00980bcc,0xdc70ed3b,0xcf1bd3ed,
0xc10dc7e4,0xc31ebedd,0xca5dc936,0xd2e9ca4c,0xf639e4e1,0xf6fffac3,0x0cb3fb48,0x39de25e9,
0x522646f8,0x540f58d6,0x332044b4,0x1567228a,0x0e900ead,0xfdd70b39,0xdd49eb83,0xd00cd5b5,
0xc1b2c8e1,0xc67bc0b0,0xc97acb1d,0xd55aca3c,0xf73be86c,0xf5cdf90f,0x0df8fadd,0x377025f1,
0x52e4455a,0x51ec588b,0x329f4358,0x12e320fd,0x0eba0e28,0xfadb095b,0xde6deaad,0xd17dd727,
0xc15bc901,0xc8a2c206,0xc856cb13,0xd900caca,0xf8a5ebfb,0xf5cff8fd,0x1109fce7,0x358e25f7,
0x5369456b,0x5008579b,0x31e042c7,0x116c1edb,0x0eca0eab,0xf80406a8,0xdde1e932,0xd1aed79f,
0xc152c7af,0xc9a2c393,0xc5ecc98f,0xdb48cb55,0xf95dee61,0xf63ef858,0x13bd0020,0x35d825fa,
0x55014773,0x4fef5731,0x312f4345,0x111c1cdc,0x0d8d0fa9,0xf55403a8,0xdd53e706,0xd062d834,
0xc14cc622,0xca63c597,0xc4dac7d5,0xddcacc72,0xfa26f118,0xf82cf7d9,0x152f0393,0x363d2512,
0x54db48c3,0x4f725617,0x2e7042c4,0x113d1a70,0x0c021006,0xf2b7018b,0xdd8be5a9,0xced7d804,
0xc26ec4b1,0xcae9c7e3,0xc517c69d,0xe23bcea0,0xfadcf4ef,0xfa5ff81a,0x14c405f8,0x36ba242e,
0x53e84944,0x4fcb5547,0x2bbc41cd,0x11131912,0x0a610f27,0xf016fe5d,0xde06e479,0xcd57d7f1,
0xc44bc42d,0xca1cc97c,0xc450c524,0xe547d01a,0xfab5f6bd,0xfd9bf945,0x15150836,0x37fe24a2,
0x53364970,0x50d455b7,0x2aaf40b7,0x114518f5,0x08940f10,0xeda6fba8,0xdde5e392,0xcb73d695,
0xc5c6c47f,0xca2dca78,0xc480c467,0xe79fd1ed,0xfa37f6b7,0xffa2face,0x13f40852,0x37902415,
0x523647c7,0x511056a3,0x2ad5408a,0x12a319d4,0x07dc0fb5,0xecddfa5f,0xde7be454,0xca03d517,
0xc62bc43f,0xc87bca5a,0xc3a5c215,0xe954d3c1,0xfae8f6b7,0x027afdae,0x153109c8,0x3883264c,
0x52e9479b,0x502f570d,0x28793e6c,0x121c1886,0x04db0e60,0xea67f65b,0xdcdae393,0xc8a5d2bb,
0xc818c4d3,0xc891cc26,0xc67ec210,0xecded8b9,0xfcf6f843,0x046c008d,0x16970a58,0x37c72749,
0x52a546fa,0x4e1e565b,0x25793b20,0x10e616a2,0x00ca0c63,0xe93df2a9,0xdc3ee35d,0xc90cd1ed,
0xcb21c6af,0xc7b2cddf,0xc918c1b8,0xeeb8dc3d,0xff28f949,0x051402d9,0x17cf0b36,0x370d2746,
0x527e4660,0x4c4955bb,0x239e38ae,0x11c9164f,0xfe660b68,0xe954f16d,0xdb57e363,0xc87bd0bc,
0xccd1c7b3,0xc528cd53,0xcafdc0fd,0xef24de54,0x0159fa2b,0x0603041e,0x19390cf5,0x36b72782,
0x52ff4686,0x4aca5550,0x21d53671,0x11c6167a,0xfb640931,0xe918efff,0xdacee340,0xc90ed01c,
0xcfa6ca7d,0xc407cd53,0xcd9cc1b9,0xefa7dfb1,0x02d6fbd4,0x06b604ac,0x19230ddb,0x352c2637,
0x51c4457c,0x481453c6,0x20a5334f,0x12241770,0xfa5f07ba,0xe9c0f01f,0xda94e3e3,0xca33cf8b,
0xd19bcd73,0xc33ecc82,0xcf99c37e,0xf09be077,0x034efd2a,0x073904be,0x19260e8b,0x34a7257e,
0x51dc453a,0x458452e4,0x1ff230a1,0x10c017b8,0xf8630541,0xe998ef16,0xd94be3ab,0xcbf6ced0,
0xd315d0dc,0xc400cbce,0xd16dc5ca,0xf274e1e8,0x0436feb6,0x088c0584,0x18cd0f59,0x33ca2494,
0x51d04505,0x42b6517b,0x20242ec5,0x0fa71806,0xf6d90378,0xe9baee80,0xd755e2c1,0xcd97cdd1,
0xd265d2d0,0xc416ca42,0xd240c6bf,0xf410e30b,0x04ee0023,0x0a1b06ba,0x19261075,0x33c2244f,
0x525b45e0,0x40424ffa,0x20ef2df2,0x0eed185a,0xf5a601dd,0xea16ee76,0xd559e1e1,0xcfd8cddf,
0xd157d3fd,0xc429c971,0xd2a0c70a,0xf529e411,0x0550005e,0x0b6007a7,0x192f1164,0x33b823ca,
0x520546b6,0x3dc04d75,0x20e62d1c,0x0d8d182a,0xf4a7000f,0xea94eeda,0xd41be081,0xd2cbcf75,
0xd166d55f,0xc506c9a7,0xd3f7c7db,0xf694e5e6,0x063c014a,0x0c6c08ec,0x182511bc,0x330a220e,
0x4fcd4676,0x3b1149ea,0x20a52bd8,0x0c4317b6,0xf48ffe8a,0xeb38f077,0xd449df5a,0xd551d1e8,
0xd1b3d66e,0xc55eca38,0xd4b9c83b,0xf673e677,0x060900c3,0x0d380943,0x17541214,0x347421c3,
0x4f024788,0x3a0a4823,0x21352be0,0x0ada17a2,0xf4b8fceb,0xea2df165,0xd429dd69,0xd68cd384,
0xd16fd674,0xc4c7c9d7,0xd560c82d,0xf6bae71c,0x064c00e9,0x0e3109ee,0x15e211a1,0x3571218e,
0x4dc747d5,0x39ba46bb,0x23202ce3,0x0a6918d8,0xf654fd00,0xe8b8f249,0xd462dbad,0xd6e4d4a7,
0xd094d607,0xc359c867,0xd51fc772,0xf665e6cb,0x066c0097,0x0f820b53,0x15e411ab,0x37ae231a,
0x4cff48af,0x38ea4595,0x24002d3b,0x086017fe,0xf6eefc50,0xe651f199,0xd479da11,0xd73ed52b,
0xd069d63d,0xc3b0c81f,0xd705c8d7,0xf7d5e8cc,0x07eb0173,0x10750d69,0x15711104,0x383e23c8,
0x4b0347d8,0x3761432d,0x23d42d53,0x066c161e,0xf71bfc17,0xe3d9f003,0xd477d8d6,0xd797d58e,
0xd02dd67d,0xc405c7c8,0xd8b7c9e7,0xf8ccea77,0x0a3b0285,0x11980ffc,0x166d1123,0x39b325b4,
0x49b34809,0x36904146,0x22d82d69,0x046c1376,0xf682fbb3,0xe1aeedf7,0xd497d7e4,0xd84ad621,
0xcf77d682,0xc3b3c705,0xd9f4ca6c,0xf953eb8c,0x0c8a03aa,0x11d411f5,0x172710ff,0x3ab826e8,
0x47ed47ba,0x36ab3fc9,0x22302e4f,0x042911e0,0xf60dfc4d,0xdff5ec31,0xd479d70f,0xd8d7d690,
0xcf45d6ac,0xc3b8c6ad,0xdb04cb43,0xf8fbebb2,0x0e48046f,0x11ba12d6,0x17f21113,0x3ba42842,
0x46174718,0x37613eae,0x20e12ecd,0x040b107c,0xf545fc90,0xdea4eaa7,0xd476d651,0xd91fd717,
0xce93d678,0xc393c5d9,0xdbffcc29,0xf971ebc2,0x10ea068e,0x12bb148c,0x199b121f,0x3cfc2a5f,
0x43df4626,0x37593d97,0x1e862dd1,0x03230e83,0xf344fb6e,0xdc66e82c,0xd40bd4e4,0xd96dd744,
0xce74d69b,0xc4b6c5e0,0xddcace72,0xfb0dec96,0x135d094a,0x13981612,0x1b191303,0x3d9b2c5a,
0x41bc4468,0x37093ce2,0x1c272c29,0x023f0cf6,0xf18cfa4b,0xdaa2e617,0xd431d420,0xda5ad80e,
0xce5ed734,0xc626c5e3,0xdeabd078,0xfc67ed19,0x14e00b2c,0x13571697,0x1ba312ac,0x3c9d2d3f,
0x3f894182,0x36b33c59,0x1b2c2af7,0x02bf0cef,0xf157faac,0xda38e578,0xd52dd461,0xdb82d94b,
0xcdbdd792,0xc757c5d0,0xde7bd151,0xfd4eed0f,0x15af0c71,0x12d316a8,0x1cfc1297,0x3ba92e74,
0x3e4e3f65,0x35e03c17,0x19d92961,0x02460c44,0xf015f9e4,0xd91be400,0xd59ed41d,0xdcedda6e,
0xcda4d7e4,0xc93dc6ee,0xdef0d288,0xfee5edd6,0x17270e51,0x124d16f8,0x1e5112e6,0x39c72ecf,
0x3d3e3d03,0x35233ba4,0x19632872,0x02e40c9e,0xef6cfa12,0xd81ce2c1,0xd5a6d37f,0xdcfddb34,
0xcc4ed68a,0xc97ac6b8,0xde6cd22f,0xffc4edf7,0x18730fda,0x12dc175b,0x219f1532,0x399f30a6,
0x3d7e3ca0,0x34853bc6,0x18bd2774,0x02520c22,0xed61f8f6,0xd649e09f,0xd58fd22d,0xdcaedbd4,
0xcc07d572,0xcacfc7d9,0xdf54d327,0x019def3c,0x196011dc,0x12c916e8,0x231316ab,0x37cc303c,
0x3c813b04,0x332c3ad5,0x18232640,0x02ac0c44,0xececf8cc,0xd593e006,0xd6add20d,0xdc8bdce8,
0xcc3bd4bb,0xcb94c8b8,0xdf98d389,0x0334efeb,0x19fb13b7,0x1379168e,0x249318ad,0x367d2ff9,
0x3b5f39c3,0x3120394a,0x16862421,0x01930b55,0xeb94f76f,0xd473dea2,0xd8b3d297,0xdd1fdea9,
0xcdb4d564,0xcd2dca86,0xe06cd4ac,0x053ff12b,0x19841520,0x13d91599,0x24f319c9,0x34d62ee0,
0x3a3c3866,0x2fb13808,0x16452300,0x01720b72,0xeae3f722,0xd376dd44,0xd9ffd30b,0xdcc7df28,
0xce2bd556,0xcdc6cb3f,0xe050d4bb,0x0724f21c,0x1902160b,0x14d91558,0x25931b65,0x33dd2e4a,
0x3979378a,0x2e7636f5,0x168e2272,0x01f10bdb,0xea92f797,0xd30fdc31,0xdb3ed41e,0xdc5fdf50,
0xce27d50c,0xcda6cb77,0xdf93d3d3,0x0814f2b8,0x17b515ab,0x15f514fc,0x264b1cd3,0x339f2e2f,
0x39b437a9,0x2e0a36ba,0x16ed227d,0x023d0c0e,0xe922f77a,0xd218da2c,0xdb3ed450,0xdb96de9d,
0xce89d4b4,0xce09cc58,0xe04dd3a0,0x0a16f4d4,0x17b51655,0x17b515e7,0x26d61e69,0x32e12dcf,
0x38973700,0x2c83350f,0x15fd2181,0x01fe0b6c,0xe753f6ad,0xd1c7d868,0xdb9ed4d8,0xdb78de98,
0xcfafd4eb,0xcea1cdc7,0xe1a4d3f3,0x0b9ef70e,0x1779165d,0x18f8167f,0x27371f81,0x328d2d6a,
0x37aa36ab,0x2b6c33b7,0x1532209d,0x020d0b3e,0xe548f5d2,0xd153d6ce,0xdb6ed4e6,0xdab1de05,
0xd074d4c9,0xce8ece65,0xe38ad442,0x0d5bf9c3,0x181616f8,0x1aa817bf,0x276e20e1,0x321f2d03,
0x363c35da,0x2a0b3221,0x141d1f26,0x01e20b06,0xe388f486,0xd1aad5f8,0xdc3ed5e3,0xdacede3d,
0xd1bcd5b1,0xce32ceef,0xe4cfd45f,0x0db2fb27,0x17c91675,0x1ba71831,0x2737215e,0x31c82ca7,
0x35493529,0x295b3162,0x14411e80,0x024d0bf9,0xe247f39d,0xd1afd54a,0xdc3fd657,0xda4cdd92,
0xd278d61b,0xcd91ceb5,0xe648d4bd,0x0e66fca9,0x183e1690,0x1d6919a7,0x279a2268,0x31912ceb,
0x34163437,0x27983002,0x138d1cd3,0x01390bc3,0xe04af19e,0xd1c1d424,0xdc88d6f1,0xdb04dd8d,
0xd423d7e4,0xce3acf7a,0xe89ad65c,0x0ef4fe76,0x18811663,0x1e531aa8,0x277c22ae,0x30a12c8a,
0x32b432db,0x25952e6c,0x136c1b7a,0x00b50be0,0xdf0af06c,0xd214d3a4,0xdbddd731,0xdb04dc99,
0xd494d8b7,0xce83cf4e,0xeaa7d7c1,0x0fc90031,0x19ac16db,0x1fd81c65,0x286f23cb,0x30422cf0,
0x31e7323d,0x23a62cea,0x12c71a3a,0xfefe0b12,0xdcbdedf8,0xd221d26c,0xdb87d75c,0xdc35dca0,
0xd58cda53,0xcf7ccfd8,0xecd1d994,0x0fcb0176,0x19fc1694,0x20231cf9,0x284d23ec,0x2f442c4b,
0x30c8316e,0x224d2b3c,0x136f19eb,0xfed50ba1,0xdbf6ed1e,0xd2f3d2aa,0xdb32d7b7,0xdd00dcaa,
0xd575db02,0xcfb6cf77,0xee8adae1,0x0fdf0264,0x1aa216c1,0x20b61da9,0x284d246c,0x2e412b82,
0x2ef93047,0x203328ca,0x134518f1,0xfdf20b76,0xdb2febba,0xd425d33e,0xdbbad862,0xdeeaddea,
0xd63ddc92,0xd0a2cfcf,0xf00cdc97,0x0f6302bf,0x1a851665,0x20c01d8d,0x27c02477,0x2daa2aae,
0x2dbd2f99,0x1f5c275e,0x1409191c,0xfd430bee,0xda69ea5b,0xd44ad36a,0xdb17d7d0,0xdf71de05,
0xd5dcdcd6,0xd14ccf84,0xf15ade1e,0x0f910335,0x1b3b16e2,0x21f61e79,0x27ee255c,0x2d922a91,
0x2c762f14,0x1e6125cd,0x14841900,0xfc560c04,0xda46e92d,0xd47fd402,0xdad8d770,0xe00bde61,
0xd50bdcdf,0xd17dcecb,0xf186ded3,0x0f3d02d3,0x1b6b16c8,0x231a1f4a,0x289a2642,0x2e4d2b41,
0x2bad2f20,0x1dac249f,0x151e1934,0xfaf50bc3,0xd9a3e7c0,0xd3cdd3bf,0xda63d66f,0xe0c3dec2,
0xd4d3dd19,0xd306cf2c,0xf2fce0c5,0x0fc803c8,0x1bf1171c,0x23f42028,0x286f2677,0x2dbe2b0b,
0x29732dce,0x1be82236,0x14e6189e,0xf9630ac0,0xd9b8e6c1,0xd3f4d43b,0xdb0ed683,0xe223e02f,
0xd513ddb5,0xd4b1cffe,0xf402e24f,0x0fdf0444,0x1c25170f,0x243e20b7,0x2844265f,0x2d722afe,
0x27bc2cda,0x1b032067,0x151018d5,0xf81a09ea,0xd9abe60a,0xd396d434,0xdb56d62e,0xe2dde118,
0xd4ecddad,0xd5ecd0b8,0xf4e6e382,0x0fc904b4,0x1c6a16e8,0x246d2133,0x28262636,0x2d3f2af2,
0x26472c0d,0x1aff1f15,0x158919bc,0xf77c098d,0xd9fde5f9,0xd333d44b,0xdbc6d5dc,0xe330e1fb,
0xd4b5dd57,0xd698d11d,0xf52ae41f,0x0f1d045d,0x1c8c167a,0x2483217f,0x28612640,0x2d7e2b75,
0x24d32b73,0x1af91df6,0x15af1a54,0xf6fa08f3,0xda5be604,0xd2bcd451,0xdc44d595,0xe345e2a0,
0xd4acdceb,0xd76ad195,0xf5d3e512,0x0f0e048f,0x1d3816b6,0x24e9221b,0x28d32693,0x2d9b2bfb,
0x232b2a76,0x1abb1cc6,0x14fb1a4f,0xf5bc07a8,0xd9e5e554,0xd1aad382,0xdc91d516,0xe345e310,
0xd50bdcdd,0xd8d2d288,0xf725e6b7,0x0f850536,0x1e6317a2,0x259d2322,0x293e26f2,0x2d4b2c61,
0x213428fa,0x1a911b8e,0x14621a61,0xf52e06b1,0xd9aee523,0xd0c6d2b4,0xdcd4d4cc,0xe2b8e30b,
0xd4e6dc42,0xd9a0d2cd,0xf796e796,0x0f960523,0x1f30181d,0x26852426,0x2aa527ee,0x2da32dc9,
0x1fc227e9,0x1a791ad3,0x13731a10,0xf42c0566,0xd91ee4c3,0xd011d1c2,0xdd54d4e6,0xe2dee365,
0xd55fdc79,0xdb2ad3ea,0xf86be914,0x0fbd056b,0x1f701845,0x26482435,0x2af827df,0x2cce2e01,
0x1df0260d,0x1a1819cc,0x1269198a,0xf3e50468,0xd91fe502,0xd039d181,0xde4bd5bb,0xe33ae41b,
0xd5e2dcb1,0xdc78d4ec,0xf8e8ea20,0x0fdf058d,0x201718bc,0x2665248c,0x2b9f2828,0x2bdd2e4c,
0x1c222419,0x197d18ca,0x111718a5,0xf3590336,0xd891e4ba,0xd035d0e6,0xdef8d641,0xe389e49b,
0xd676dce9,0xde06d62b,0xf98feb59,0x100c05bd,0x208e1932,0x267524b1,0x2ca028c8,0x2b202eb5,
0x1b1e22c4,0x19821887,0x10281833,0xf2f9028c,0xd797e41d,0xcf73cfcc,0xdecbd5dd,0xe34ae468,
0xd6fadcce,0xdf9ed778,0xfa58ecb1,0x108d0632,0x211619f4,0x26a424c7,0x2da1298a,0x2a3a2ef6,
0x19f82146,0x1934181b,0x0f371763,0xf2c701fb,0xd6d0e3ac,0xcf46cf34,0xdf1ed613,0xe329e499,
0xd7aedcdb,0xe13ad8c3,0xfb18ee06,0x10f90678,0x21351a67,0x268a2487,0x2e532a30,0x28f82edc,
0x18c21f97,0x18691776,0x0ded161b,0xf2270137,0xd60ee2f6,0xcf72cec9,0xdff5d6b2,0xe3c3e55f,
0xd91adda6,0xe341dac5,0xfbf6ef9f,0x11a6071f,0x21301aff,0x264123ff,0x2e4c2a44,0x26d42dc9,
0x172a1d51,0x17411669,0x0d1b14d0,0xf22d00f9,0xd602e2e3,0xcfebcef2,0xe0e0d794,0xe3ede5cf,
0xda02de05,0xe4cddc2e,0xfc62f09a,0x120a0765,0x20ca1b36,0x2652236e,0x2ec82aff,0x25902d56,
0x167f1c0b,0x168015f0,0x0c8b13e9,0xf21000bb,0xd5bce2a6,0xd048cee0,0xe17cd854,0xe3e9e611,
0xda82de1a,0xe5a6dd12,0xfc22f0d8,0x12160738,0x20171aee,0x267f22df,0x2f962be8,0x24e12d33,
0x168f1ba4,0x164c1606,0x0c8a13b8,0xf21d00eb,0xd54be269,0xd038ce8a,0xe169d868,0xe33ae58f,
0xdac4ddb4,0xe682ddf2,0xfc74f14b,0x12e907dc,0x20161b5f,0x273a22f3,0x2fe12cea,0x237d2c61,
0x15d21a8f,0x152e1522,0x0bd3129b,0xf1b80075,0xd4fee1fa,0xd0edcea1,0xe25cd97b,0xe3b4e638,
0xdc13de88,0xe779df6c,0xfc78f195,0x12f80800,0x1f521ae4,0x27a7229e,0x2ffa2da7,0x224e2b81,
0x154719c7,0x14501471,0x0b4c11dc,0xf16f0022,0xd477e188,0xd10dce61,0xe283d9cd,0xe3a5e626,
0xdd6fdf03,0xe90be13a,0xfd84f2ad,0x13870910,0x1ec31aca,0x280a225a,0x2fa12e18,0x21072a5e,
0x145418ad,0x1351137a,0x0ac210ff,0xf138ffdc,0xd449e148,0xd1a7ce8e,0xe31eda8d,0xe3b5e63e,
0xdea5df99,0xe9bfe26b,0xfdd2f2fd,0x13890973,0x1ded1a1d,0x28aa222f,0x2f842e9d,0x207529be,
0x1407186c,0x12d31303,0x0a9910b6,0xf102ffcd,0xd3d7e0d2,0xd1ecce70,0xe319dae7,0xe36ce5ed,
0xdfa5dff4,0xea8be393,0xfe68f39b,0x13ad09f7,0x1d201980,0x292d2211,0x2f112ee8,0x1fa228dd,
0x13a317d2,0x12481279,0x0a86104f,0xf116ffe5,0xd3ede0d9,0xd2b3cee8,0xe365dbb7,0xe358e5c6,
0xe086e06d,0xead4e44e,0xfec3f3b9,0x134f0a32,0x1c7c18a1,0x29bc222f,0x2e962f1d,0x1ef4281e,
0x1327174b,0x11c31202,0x0a88100a,0xf0e7ffea,0xd3d6e098,0xd32bcf14,0xe350dc0e,0xe340e54d,
0xe1aae10e,0xeb8ae55e,0xff7bf45e,0x12fb0aaf,0x1c1117ce,0x2a6d2292,0x2e732f6a,0x1e9d27ba,
0x12ed1713,0x116c11b1,0x0a440fc3,0xf04aff89,0xd322dfc9,0xd33cced3,0xe2c2dbec,0xe2dbe47e,
0xe264e16a,0xec0fe60c,0x0064f4fd,0x13320b5f,0x1ca517e1,0x2bc123d7,0x2ea53050,0x1e402788,
0x1229168c,0x105110b0,0x09780ec8,0xef85fed5,0xd2a5def7,0xd3b5cedb,0xe2aadc4b,0xe331e458,
0xe393e261,0xecbbe6ec,0x0151f5e0,0x12d50bcd,0x1c8a1747,0x2c562450,0x2e583086,0x1d8d26ea,
0x113715c4,0x0f540f92,0x09090e12,0xef14fe77,0xd28bdea0,0xd480cf54,0xe2bedcdd,0xe3ace458,
0xe4d6e379,0xedb4e7e0,0x025af706,0x12720c29,0x1c6516c4,0x2c83248e,0x2db43049,0x1cb72615,
0x105114f7,0x0e760e9c,0x089f0d74,0xee9cfe14,0xd287de22,0xd526cfd8,0xe283dd12,0xe3d1e403,
0xe591e428,0xee8ee888,0x0332f822,0x12800c90,0x1cc916e3,0x2cc024f4,0x2dc5305b,0x1cd92634,
0x1080151b,0x0ecb0ee4,0x08d00dbb,0xeedbfe48,0xd28bde6f,0xd49ecf77,0xe2aedcdb,0xe3b0e450,
0xe4dce379,0xedc5e7ea,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,
};