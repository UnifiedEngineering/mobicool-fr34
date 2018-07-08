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
// Table with Beta 2670K, generated using the following code:
// #define beta (2670.0)
// for (uint16_t i = 144; i < 905; i++) {
//     float f = round(10 * ((1 / ((log(((10000.0 * i) / (1023.0 - i))/10000.0)/beta) + (1 / (273.15 + 25.000)))) - 273.15));
//     printf("%d, ", (int)f);
// }

#define NTCMAPOFFSET (140)
static const int16_t ntcmap[] = { 1005, 1001, 996, 992, 988, 984, 980, 976, 972, 968, 964, 960, 956, 952, 949, 945, 
                                941, 937, 934, 930, 926, 923, 919, 916, 912, 908, 905, 901, 898, 895, 891, 888, 
                                884, 881, 878, 874, 871, 868, 865, 861, 858, 855, 852, 849, 846, 842, 839, 836, 
                                833, 830, 827, 824, 821, 818, 815, 812, 809, 806, 804, 801, 798, 795, 792, 789, 
                                786, 784, 781, 778, 775, 773, 770, 767, 765, 762, 759, 757, 754, 751, 749, 746, 
                                743, 741, 738, 736, 733, 731, 728, 726, 723, 721, 718, 716, 713, 711, 708, 706, 
                                704, 701, 699, 696, 694, 692, 689, 687, 685, 682, 680, 678, 675, 673, 671, 669, 
                                666, 664, 662, 660, 657, 655, 653, 651, 649, 646, 644, 642, 640, 638, 636, 634, 
                                631, 629, 627, 625, 623, 621, 619, 617, 615, 613, 611, 608, 606, 604, 602, 600, 
                                598, 596, 594, 592, 590, 588, 586, 584, 583, 581, 579, 577, 575, 573, 571, 569, 
                                567, 565, 563, 561, 559, 558, 556, 554, 552, 550, 548, 546, 545, 543, 541, 539, 
                                537, 535, 534, 532, 530, 528, 526, 525, 523, 521, 519, 518, 516, 514, 512, 511, 
                                509, 507, 505, 504, 502, 500, 498, 497, 495, 493, 492, 490, 488, 487, 485, 483, 
                                481, 480, 478, 476, 475, 473, 471, 470, 468, 467, 465, 463, 462, 460, 458, 457, 
                                455, 454, 452, 450, 449, 447, 446, 444, 442, 441, 439, 438, 436, 434, 433, 431, 
                                430, 428, 427, 425, 424, 422, 420, 419, 417, 416, 414, 413, 411, 410, 408, 407, 
                                405, 404, 402, 401, 399, 398, 396, 395, 393, 392, 390, 389, 387, 386, 384, 383, 
                                381, 380, 378, 377, 375, 374, 373, 371, 370, 368, 367, 365, 364, 362, 361, 360, 
                                358, 357, 355, 354, 352, 351, 350, 348, 347, 345, 344, 342, 341, 340, 338, 337, 
                                335, 334, 333, 331, 330, 329, 327, 326, 324, 323, 322, 320, 319, 317, 316, 315, 
                                313, 312, 311, 309, 308, 307, 305, 304, 302, 301, 300, 298, 297, 296, 294, 293, 
                                292, 290, 289, 288, 286, 285, 284, 282, 281, 280, 278, 277, 276, 274, 273, 272, 
                                270, 269, 268, 266, 265, 264, 262, 261, 260, 258, 257, 256, 255, 253, 252, 251, 
                                249, 248, 247, 245, 244, 243, 242, 240, 239, 238, 236, 235, 234, 233, 231, 230, 
                                229, 227, 226, 225, 224, 222, 221, 220, 218, 217, 216, 215, 213, 212, 211, 209, 
                                208, 207, 206, 204, 203, 202, 201, 199, 198, 197, 196, 194, 193, 192, 191, 189, 
                                188, 187, 185, 184, 183, 182, 180, 179, 178, 177, 175, 174, 173, 172, 170, 169, 
                                168, 167, 165, 164, 163, 162, 160, 159, 158, 157, 155, 154, 153, 152, 150, 149, 
                                148, 147, 145, 144, 143, 142, 140, 139, 138, 137, 136, 134, 133, 132, 131, 129, 
                                128, 127, 126, 124, 123, 122, 121, 119, 118, 117, 116, 114, 113, 112, 111, 109, 
                                108, 107, 106, 104, 103, 102, 101, 100, 98, 97, 96, 95, 93, 92, 91, 90, 
                                88, 87, 86, 85, 83, 82, 81, 80, 78, 77, 76, 75, 73, 72, 71, 70, 
                                68, 67, 66, 65, 63, 62, 61, 60, 58, 57, 56, 55, 53, 52, 51, 50, 
                                48, 47, 46, 45, 43, 42, 41, 40, 38, 37, 36, 35, 33, 32, 31, 29, 
                                28, 27, 26, 24, 23, 22, 21, 19, 18, 17, 16, 14, 13, 12, 10, 9, 
                                8, 7, 5, 4, 3, 1, 0, -1, -2, -4, -5, -6, -8, -9, -10, -11, 
                                -13, -14, -15, -17, -18, -19, -21, -22, -23, -25, -26, -27, -28, -30, -31, -32, 
                                -34, -35, -36, -38, -39, -40, -42, -43, -44, -46, -47, -48, -50, -51, -52, -54, 
                                -55, -56, -58, -59, -60, -62, -63, -65, -66, -67, -69, -70, -71, -73, -74, -75, 
                                -77, -78, -80, -81, -82, -84, -85, -87, -88, -89, -91, -92, -94, -95, -96, -98, 
                                -99, -101, -102, -103, -105, -106, -108, -109, -111, -112, -113, -115, -116, -118, -119, -121, 
                                -122, -124, -125, -127, -128, -130, -131, -133, -134, -135, -137, -138, -140, -141, -143, -145, 
                                -146, -148, -149, -151, -152, -154, -155, -157, -158, -160, -161, -163, -164, -166, -168, -169, 
                                -171, -172, -174, -176, -177, -179, -180, -182, -184, -185, -187, -188, -190, -192, -193, -195, 
                                -197, -198, -200, -202, -203, -205, -207, -208, -210, -212, -214, -215, -217, -219, -220, -222, 
                                -224, -226, -227, -229, -231, -233, -235, -236, -238, -240, -242, -244, -246, -247, -249, -251, 
                                -253, -255, -257, -259, -260, -262, -264, -266, -268, -270, -272, -274, -276, -278, -280, -282, 
                                -284, -286, -288, -290, -292, -294, -296, -298, -300 };
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
