/*
 * analog.c - PIC ADC interface for Mobicool FR34/FR40 compressor cooler
 *
 * Copyright (C) 2018 Werner Johansson, wj@unifiedengineering.se
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mcc_generated_files/mcc.h"
#include "analog.h"

// ADC value to temperature mapping
#define NTCMAPSHIFT (0) // All values
// Table with Beta 2735K, generated using the following code:
// #define beta (2735.0)
// for (uint16_t i = 140; i < 910; i++) {
//     float f = round(10 * ((1 / ((log(((10000.0 * i) / (1023.0 - i))/10000.0)/beta) + (1 / (273.15 + 25.000)))) - 273.15));
//     printf("%d, ", (int)f);
// }

#define NTCMAPOFFSET (140)
static const int16_t ntcmap[] = { 999, 995, 991, 986, 982, 978, 974, 970, 966, 962, 958, 955, 951, 947, 943, 939,
                                936, 932, 928, 925, 921, 917, 914, 910, 907, 903, 900, 896, 893, 889, 886, 883,
                                879, 876, 873, 869, 866, 863, 860, 857, 853, 850, 847, 844, 841, 838, 835, 832,
                                829, 826, 823, 820, 817, 814, 811, 808, 805, 802, 799, 796, 794, 791, 788, 785,
                                782, 780, 777, 774, 772, 769, 766, 763, 761, 758, 756, 753, 750, 748, 745, 743,
                                740, 737, 735, 732, 730, 727, 725, 722, 720, 717, 715, 713, 710, 708, 705, 703,
                                701, 698, 696, 694, 691, 689, 687, 684, 682, 680, 677, 675, 673, 671, 668, 666,
                                664, 662, 659, 657, 655, 653, 651, 649, 646, 644, 642, 640, 638, 636, 634, 632,
                                629, 627, 625, 623, 621, 619, 617, 615, 613, 611, 609, 607, 605, 603, 601, 599,
                                597, 595, 593, 591, 589, 587, 585, 583, 581, 579, 578, 576, 574, 572, 570, 568,
                                566, 564, 562, 561, 559, 557, 555, 553, 551, 550, 548, 546, 544, 542, 541, 539,
                                537, 535, 533, 532, 530, 528, 526, 525, 523, 521, 519, 518, 516, 514, 512, 511,
                                509, 507, 506, 504, 502, 500, 499, 497, 495, 494, 492, 490, 489, 487, 485, 484,
                                482, 480, 479, 477, 476, 474, 472, 471, 469, 467, 466, 464, 463, 461, 459, 458,
                                456, 455, 453, 452, 450, 448, 447, 445, 444, 442, 441, 439, 438, 436, 434, 433,
                                431, 430, 428, 427, 425, 424, 422, 421, 419, 418, 416, 415, 413, 412, 410, 409,
                                407, 406, 404, 403, 401, 400, 398, 397, 395, 394, 393, 391, 390, 388, 387, 385,
                                384, 382, 381, 380, 378, 377, 375, 374, 372, 371, 370, 368, 367, 365, 364, 362,
                                361, 360, 358, 357, 355, 354, 353, 351, 350, 349, 347, 346, 344, 343, 342, 340,
                                339, 337, 336, 335, 333, 332, 331, 329, 328, 327, 325, 324, 323, 321, 320, 318,
                                317, 316, 314, 313, 312, 310, 309, 308, 306, 305, 304, 302, 301, 300, 299, 297,
                                296, 295, 293, 292, 291, 289, 288, 287, 285, 284, 283, 281, 280, 279, 278, 276,
                                275, 274, 272, 271, 270, 269, 267, 266, 265, 263, 262, 261, 260, 258, 257, 256,
                                254, 253, 252, 251, 249, 248, 247, 246, 244, 243, 242, 240, 239, 238, 237, 235,
                                234, 233, 232, 230, 229, 228, 227, 225, 224, 223, 222, 220, 219, 218, 217, 215,
                                214, 213, 212, 210, 209, 208, 207, 205, 204, 203, 202, 201, 199, 198, 197, 196,
                                194, 193, 192, 191, 189, 188, 187, 186, 185, 183, 182, 181, 180, 178, 177, 176,
                                175, 173, 172, 171, 170, 169, 167, 166, 165, 164, 162, 161, 160, 159, 158, 156,
                                155, 154, 153, 152, 150, 149, 148, 147, 145, 144, 143, 142, 141, 139, 138, 137,
                                136, 134, 133, 132, 131, 130, 128, 127, 126, 125, 124, 122, 121, 120, 119, 117,
                                116, 115, 114, 113, 111, 110, 109, 108, 107, 105, 104, 103, 102, 100, 99, 98,
                                97, 96, 94, 93, 92, 91, 90, 88, 87, 86, 85, 83, 82, 81, 80, 79,
                                77, 76, 75, 74, 72, 71, 70, 69, 68, 66, 65, 64, 63, 61, 60, 59,
                                58, 57, 55, 54, 53, 52, 50, 49, 48, 47, 45, 44, 43, 42, 41, 39,
                                38, 37, 36, 34, 33, 32, 31, 29, 28, 27, 26, 24, 23, 22, 21, 19,
                                18, 17, 16, 14, 13, 12, 11, 9, 8, 7, 6, 4, 3, 2, 1, -1,
                                -2, -3, -5, -6, -7, -8, -10, -11, -12, -13, -15, -16, -17, -19, -20, -21,
                                -22, -24, -25, -26, -28, -29, -30, -31, -33, -34, -35, -37, -38, -39, -41, -42,
                                -43, -45, -46, -47, -49, -50, -51, -52, -54, -55, -56, -58, -59, -60, -62, -63,
                                -65, -66, -67, -69, -70, -71, -73, -74, -75, -77, -78, -79, -81, -82, -84, -85,
                                -86, -88, -89, -90, -92, -93, -95, -96, -97, -99, -100, -102, -103, -104, -106, -107,
                                -109, -110, -112, -113, -114, -116, -117, -119, -120, -122, -123, -125, -126, -127, -129, -130,
                                -132, -133, -135, -136, -138, -139, -141, -142, -144, -145, -147, -148, -150, -151, -153, -154,
                                -156, -158, -159, -161, -162, -164, -165, -167, -168, -170, -172, -173, -175, -176, -178, -180,
                                -181, -183, -184, -186, -188, -189, -191, -193, -194, -196, -198, -199, -201, -203, -204, -206,
                                -208, -209, -211, -213, -214, -216, -218, -220, -221, -223, -225, -227, -228, -230, -232, -234,
                                -236, -237, -239, -241, -243, -245, -247, -249, -250, -252, -254, -256, -258, -260, -262, -264,
                                -266, -268, -270, -272, -274, -276, -278, -280, -282, -284, -286, -288, -290, -292, -294, -296,
                                -298, -300 };
#define NTCMAPSIZE (sizeof(ntcmap) / sizeof(ntcmap[0]))

// Input Voltage Monitor ADC value 185 ~10V so ADC value * 54 results in mV
// Compressor current monitor ADC value 80 ~ 1.6A so ADC value * 20 results in mA
// (but we're really only really interested in the power which should be below 45W)
// so we calculate that instead. Using the raw ADC values for input voltage and compressor
// current (185 for 10V and 80 for 1.6A) we can approximate the expected 16W from
// 185 shifted down 2 bits to fit in 8 bits, multiplied by 80 then using the upper 8 bits
// of result.

static int16_t s_temp10 = 0; // Cooler compartment temperature in tenths of degrees Celsius
static uint16_t s_voltage = 0; // Input voltage in mV
static uint16_t s_fancurrent = 0; // Fan current in mA
static uint8_t s_comppower = 0; // Compressor power in Watts

static adc_result_t GetConversion(adc_channel_t channel) {
    adc_result_t res = 0;
    for (uint8_t i = 0; i < 16; i++) {
        res += ADC_GetConversion( channel );
    }
    return res >> 4;
}

void AnalogUpdate(void) {
    uint16_t tmp = GetConversion( AN5_NTC );
    int16_t offset = (tmp - NTCMAPOFFSET) >> NTCMAPSHIFT;
    if (offset < 0) {
        s_temp10 = -32767;
    } else if( offset > NTCMAPSIZE ) {
        s_temp10 = 32767;
    } else {
        s_temp10 = ntcmap[offset];
    }

    uint16_t adcval_v = GetConversion( AN2_VoltMon );
    s_voltage = adcval_v * 54; // Close enough of an approximation for input voltage in mV

    s_fancurrent = GetConversion( AN7_FanCur ) * 22; // Initial approximation of fan current reading in mA

    uint16_t compressor = GetConversion( AN8_CompCur ); // Compressor current in 20mA steps
    s_comppower = 99; // Outside limits
    if (compressor < 0x100) { // Avoid overflow
        uint8_t voltage = (adcval_v + 2) >> 2; // Fit in byte + round
        uint16_t tmp = compressor * voltage;
        s_comppower = (tmp + 128) >> 8; // Divide with rounding
    }
}

int16_t AnalogGetTemperature10(void) {
    return s_temp10;
}

uint16_t AnalogGetVoltage(void) {
    return s_voltage;
}

uint16_t AnalogGetFanCurrent(void) {
    return s_fancurrent;
}

uint8_t AnalogGetCompPower(void) {
    return s_comppower;
}
