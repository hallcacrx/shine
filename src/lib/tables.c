/* tables.c
 *
 * Here are MPEG1 Table B.8 and MPEG2 Table B.1 -- Layer III scalefactor bands.
 * Index into this using a method such as:
 *    idx  = fr_ps->header->sampling_frequency + (fr_ps->header->version * 3)
 */

#include "tables.h"

const int slen1_tab[16] = { 0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4 };
const int slen2_tab[16] = { 0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3 };

/* Valid samplerates and bitrates. */
const long samplerates[9] = {
  44100, 48000, 32000, /* MPEG-I */
  22050, 24000, 16000, /* MPEG-II */
  11025, 12000, 8000   /* MPEG-2.5 */
};

const int bitrates[16][4] = {
  /* MPEG version:
   * 2.5, reserved, II, I */
   { -1,  -1,        -1,  -1},
   { 8,   -1,         8,  32},
   { 16,  -1,        16,  40},
   { 24,  -1,        24,  48},
   { 32,  -1,        32,  56},
   { 40,  -1,        40,  64},
   { 48,  -1,        48,  80},
   { 56,  -1,        56,  96},
   { 64,  -1,        64, 112},
   { 80,  -1,        80, 128},
   { 96,  -1,        96, 160},
   {112,  -1,       112, 192},
   {128,  -1,       128, 224},
   {144,  -1,       144, 256},
   {160,  -1,       160, 320},
   { -1,  -1,        -1,  -1}
};

const int shine_scale_fact_band_index[9][23] =
{
  /* MPEG-I */
    /* Table B.8.b: 44.1 kHz */
  {0,4,8,12,16,20,24,30,36,44,52,62,74,90,110,134,162,196,238,288,342,418,576},
    /* Table B.8.c: 48 kHz */
  {0,4,8,12,16,20,24,30,36,42,50,60,72,88,106,128,156,190,230,276,330,384,576},
    /* Table B.8.a: 32 kHz */
  {0,4,8,12,16,20,24,30,36,44,54,66,82,102,126,156,194,240,296,364,448,550,576},
  /* MPEG-II */
    /* Table B.2.b: 22.05 kHz */
  {0,6,12,18,24,30,36,44,54,66,80,96,116,140,168,200,238,284,336,396,464,522,576},
    /* Table B.2.c: 24 kHz */
  {0,6,12,18,24,30,36,44,54,66,80,96,114,136,162,194,232,278,330,394,464,540,576},
    /* Table B.2.a: 16 kHz */
  {0,6,12,18,24,30,36,44,45,66,80,96,116,140,168,200,238,248,336,396,464,522,576},

  /* MPEG-2.5 */
    /* 11.025 kHz */
  {0,6,12,18,24,30,36,44,54,66,80,96,116,140,168,200,238,284,336,396,464,522,576},
    /* 12 kHz */
  {0,6,12,18,24,30,36,44,54,66,80,96,116,140,168,200,238,284,336,396,464,522,576},
    /* MPEG-2.5 8 kHz */
  {0,12,24,36,48,60,72,88,108,132,160,192,232,280,336,400,476,566,568,570,572,574,576},
};

/* note. 0.035781 is shine_enwindow maximum value */
/* scale and convert to fixed point before storing */
#define SHINE_EW(x)						(long)((double)(x) * 0x7fffffff)
#define SHINE_EW2(a,b)					SHINE_EW(a), SHINE_EW(b)
#define SHINE_EW10(a,b,c,d,e,f,g,h,i,j)	SHINE_EW2(a,b), SHINE_EW2(c,d), SHINE_EW2(e,f), SHINE_EW2(g,h), SHINE_EW2(i,j)

const long shine_enwindow[] = {
SHINE_EW10(   0.000000, -0.000000, -0.000000, -0.000000, -0.000000, -0.000000, -0.000000, -0.000001, -0.000001, -0.000001),
SHINE_EW10(  -0.000001, -0.000001, -0.000001, -0.000002, -0.000002, -0.000002, -0.000002, -0.000003, -0.000003, -0.000003),
SHINE_EW10(  -0.000004, -0.000004, -0.000005, -0.000005, -0.000006, -0.000007, -0.000008, -0.000008, -0.000009, -0.000010),
SHINE_EW10(  -0.000011, -0.000012, -0.000014, -0.000015, -0.000017, -0.000018, -0.000020, -0.000021, -0.000023, -0.000025),
SHINE_EW10(  -0.000028, -0.000030, -0.000032, -0.000035, -0.000038, -0.000041, -0.000043, -0.000046, -0.000050, -0.000053),
SHINE_EW10(  -0.000056, -0.000060, -0.000063, -0.000066, -0.000070, -0.000073, -0.000077, -0.000081, -0.000084, -0.000087),
SHINE_EW10(  -0.000091, -0.000093, -0.000096, -0.000099,  0.000102,  0.000104,  0.000106,  0.000107,  0.000108,  0.000109),
SHINE_EW10(   0.000109,  0.000108,  0.000107,  0.000105,  0.000103,  0.000099,  0.000095,  0.000090,  0.000084,  0.000078),
SHINE_EW10(   0.000070,  0.000061,  0.000051,  0.000040,  0.000027,  0.000014, -0.000001, -0.000017, -0.000034, -0.000053),
SHINE_EW10(  -0.000073, -0.000094, -0.000116, -0.000140, -0.000165, -0.000191, -0.000219, -0.000247, -0.000277, -0.000308),
SHINE_EW10(  -0.000339, -0.000371, -0.000404, -0.000438, -0.000473, -0.000507, -0.000542, -0.000577, -0.000612, -0.000647),
SHINE_EW10(  -0.000681, -0.000714, -0.000747, -0.000779, -0.000810, -0.000839, -0.000866, -0.000892, -0.000915, -0.000936),
SHINE_EW10(  -0.000954, -0.000969, -0.000981, -0.000989, -0.000994, -0.000995, -0.000992, -0.000984,  0.000971,  0.000954),
SHINE_EW10(   0.000931,  0.000903,  0.000869,  0.000829,  0.000784,  0.000732,  0.000674,  0.000610,  0.000539,  0.000463),
SHINE_EW10(   0.000379,  0.000288,  0.000192,  0.000088, -0.000021, -0.000137, -0.000260, -0.000388, -0.000522, -0.000662),
SHINE_EW10(  -0.000807, -0.000957, -0.001111, -0.001270, -0.001432, -0.001598, -0.001767, -0.001937, -0.002110, -0.002283),
SHINE_EW10(  -0.002457, -0.002631, -0.002803, -0.002974, -0.003142, -0.003307, -0.003467, -0.003623, -0.003772, -0.003914),
SHINE_EW10(  -0.004049, -0.004175, -0.004291, -0.004396, -0.004490, -0.004570, -0.004638, -0.004691, -0.004728, -0.004749),
SHINE_EW10(  -0.004752, -0.004737, -0.004703, -0.004649, -0.004574, -0.004477, -0.004358, -0.004215, -0.004049, -0.003859),
SHINE_EW10(  -0.003643, -0.003402,  0.003135,  0.002841,  0.002522,  0.002175,  0.001801,  0.001400,  0.000971,  0.000516),
SHINE_EW10(   0.000033, -0.000476, -0.001012, -0.001574, -0.002162, -0.002774, -0.003411, -0.004072, -0.004756, -0.005462),
SHINE_EW10(  -0.006189, -0.006937, -0.007703, -0.008487, -0.009288, -0.010104, -0.010933, -0.011775, -0.012628, -0.013489),
SHINE_EW10(  -0.014359, -0.015234, -0.016113, -0.016994, -0.017876, -0.018757, -0.019634, -0.020507, -0.021372, -0.022229),
SHINE_EW10(  -0.023074, -0.023907, -0.024725, -0.025527, -0.026311, -0.027074, -0.027815, -0.028533, -0.029225, -0.029890),
SHINE_EW10(  -0.030527, -0.031133, -0.031707, -0.032248, -0.032755, -0.033226, -0.033660, -0.034056, -0.034413, -0.034730),
SHINE_EW10(  -0.035007, -0.035242, -0.035435, -0.035586, -0.035694, -0.035759,  0.035781,  0.035759,  0.035694,  0.035586),
SHINE_EW10(   0.035435,  0.035242,  0.035007,  0.034730,  0.034413,  0.034056,  0.033660,  0.033226,  0.032755,  0.032248),
SHINE_EW10(   0.031707,  0.031133,  0.030527,  0.029890,  0.029225,  0.028533,  0.027815,  0.027074,  0.026311,  0.025527),
SHINE_EW10(   0.024725,  0.023907,  0.023074,  0.022229,  0.021372,  0.020507,  0.019634,  0.018757,  0.017876,  0.016994),
SHINE_EW10(   0.016113,  0.015234,  0.014359,  0.013489,  0.012628,  0.011775,  0.010933,  0.010104,  0.009288,  0.008487),
SHINE_EW10(   0.007703,  0.006937,  0.006189,  0.005462,  0.004756,  0.004072,  0.003411,  0.002774,  0.002162,  0.001574),
SHINE_EW10(   0.001012,  0.000476, -0.000033, -0.000516, -0.000971, -0.001400, -0.001801, -0.002175, -0.002522, -0.002841),
SHINE_EW10(   0.003135,  0.003402,  0.003643,  0.003859,  0.004049,  0.004215,  0.004358,  0.004477,  0.004574,  0.004649),
SHINE_EW10(   0.004703,  0.004737,  0.004752,  0.004749,  0.004728,  0.004691,  0.004638,  0.004570,  0.004490,  0.004396),
SHINE_EW10(   0.004291,  0.004175,  0.004049,  0.003914,  0.003772,  0.003623,  0.003467,  0.003307,  0.003142,  0.002974),
SHINE_EW10(   0.002803,  0.002631,  0.002457,  0.002283,  0.002110,  0.001937,  0.001767,  0.001598,  0.001432,  0.001270),
SHINE_EW10(   0.001111,  0.000957,  0.000807,  0.000662,  0.000522,  0.000388,  0.000260,  0.000137,  0.000021, -0.000088),
SHINE_EW10(  -0.000192, -0.000288, -0.000379, -0.000463, -0.000539, -0.000610, -0.000674, -0.000732, -0.000784, -0.000829),
SHINE_EW10(  -0.000869, -0.000903, -0.000931, -0.000954,  0.000971,  0.000984,  0.000992,  0.000995,  0.000994,  0.000989),
SHINE_EW10(   0.000981,  0.000969,  0.000954,  0.000936,  0.000915,  0.000892,  0.000866,  0.000839,  0.000810,  0.000779),
SHINE_EW10(   0.000747,  0.000714,  0.000681,  0.000647,  0.000612,  0.000577,  0.000542,  0.000507,  0.000473,  0.000438),
SHINE_EW10(   0.000404,  0.000371,  0.000339,  0.000308,  0.000277,  0.000247,  0.000219,  0.000191,  0.000165,  0.000140),
SHINE_EW10(   0.000116,  0.000094,  0.000073,  0.000053,  0.000034,  0.000017,  0.000001, -0.000014, -0.000027, -0.000040),
SHINE_EW10(  -0.000051, -0.000061, -0.000070, -0.000078, -0.000084, -0.000090, -0.000095, -0.000099, -0.000103, -0.000105),
SHINE_EW10(  -0.000107, -0.000108, -0.000109, -0.000109, -0.000108, -0.000107, -0.000106, -0.000104,  0.000102,  0.000099),
SHINE_EW10(   0.000096,  0.000093,  0.000091,  0.000087,  0.000084,  0.000081,  0.000077,  0.000073,  0.000070,  0.000066),
SHINE_EW10(   0.000063,  0.000060,  0.000056,  0.000053,  0.000050,  0.000046,  0.000043,  0.000041,  0.000038,  0.000035),
SHINE_EW10(   0.000032,  0.000030,  0.000028,  0.000025,  0.000023,  0.000021,  0.000020,  0.000018,  0.000017,  0.000015),
SHINE_EW10(   0.000014,  0.000012,  0.000011,  0.000010,  0.000009,  0.000008,  0.000008,  0.000007,  0.000006,  0.000005),
SHINE_EW10(   0.000005,  0.000004,  0.000004,  0.000003,  0.000003,  0.000003,  0.000002,  0.000002,  0.000002,  0.000002),
SHINE_EW10(   0.000001,  0.000001,  0.000001,  0.000001,  0.000001,  0.000001,  0.000000,  0.000000,  0.000000,  0.000000),
SHINE_EW2 (   0.000000,  0.000000)
};
