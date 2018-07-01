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
#define NTCMAPSHIFT (1) // Only every other value to conserve space
// Table with Beta 2735K, generated using the following code:
// #define beta (2735.0)
// for (uint16_t i = 139; i < 980; i += 2) {
//     float f = round((1 / ((log(((10000.0 * i) / (1023.0 - i))/10000.0)/beta) + (1 / (273.15 + 25.000)))) - 273.15);
//     printf("%d, ", (int)f);
// }

#define NTCMAPOFFSET (139)
static const int8_t ntcmap[] = { 100, 99, 99, 98, 97, 96, 95, 95, 94, 93, 92, 92, 91, 90, 90, 89,
                                88, 88, 87, 86, 86, 85, 84, 84, 83, 83, 82, 81, 81, 80, 80, 79,
                                79, 78, 77, 77, 76, 76, 75, 75, 74, 74, 73, 73, 72, 72, 71, 71,
                                70, 70, 69, 69, 68, 68, 68, 67, 67, 66, 66, 65, 65, 64, 64, 64,
                                63, 63, 62, 62, 62, 61, 61, 60, 60, 60, 59, 59, 58, 58, 58, 57,
                                57, 56, 56, 56, 55, 55, 55, 54, 54, 54, 53, 53, 52, 52, 52, 51,
                                51, 51, 50, 50, 50, 49, 49, 49, 48, 48, 48, 47, 47, 47, 46, 46,
                                46, 45, 45, 45, 45, 44, 44, 44, 43, 43, 43, 42, 42, 42, 41, 41,
                                41, 41, 40, 40, 40, 39, 39, 39, 39, 38, 38, 38, 37, 37, 37, 37,
                                36, 36, 36, 35, 35, 35, 35, 34, 34, 34, 33, 33, 33, 33, 32, 32,
                                32, 32, 31, 31, 31, 31, 30, 30, 30, 29, 29, 29, 29, 28, 28, 28,
                                28, 27, 27, 27, 27, 26, 26, 26, 26, 25, 25, 25, 25, 24, 24, 24,
                                24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 21, 20, 20, 20,
                                20, 19, 19, 19, 19, 18, 18, 18, 18, 17, 17, 17, 17, 16, 16, 16,
                                16, 15, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12,
                                12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8,
                                8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4,
                                4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0,
                                0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -4, -4,
                                -4, -4, -5, -5, -5, -6, -6, -6, -6, -7, -7, -7, -7, -8, -8, -8,
                                -8, -9, -9, -9, -10, -10, -10, -10, -11, -11, -11, -12, -12, -12, -12, -13,
                                -13, -13, -14, -14, -14, -15, -15, -15, -15, -16, -16, -16, -17, -17, -17, -18,
                                -18, -18, -19, -19, -19, -20, -20, -20, -21, -21, -21, -22, -22, -22, -23, -23,
                                -23, -24, -24, -24, -25, -25, -26, -26, -26, -27, -27, -28, -28, -28, -29, -29,
                                -30, -30, -30, -31, -31, -32, -32, -33, -33, -34, -34, -35, -35, -36, -36, -37,
                                -37, -38, -38, -39, -39, -40, -40, -41, -42, -42, -43, -44, -44, -45, -46, -46,
                                -47, -48, -49, -50, -50 };
#define NTCMAPSIZE (sizeof(ntcmap) / sizeof(ntcmap[0]))

// Input Voltage Monitor ADC value 185 ~10V so ADC value * 54 results in mV
// Compressor current monitor ADC value 80 ~ 1.6A so ADC value * 20 results in mA
// (but we're really only really interested in the power which should be below 45W)
// so we calculate that instead. Using the raw ADC values for input voltage and compressor
// current (185 for 10V and 80 for 1.6A) we can approximate the expected 16W from
// 185 shifted down 2 bits to fit in 8 bits, multiplied by 80 then using the upper 8 bits
// of result.

static int8_t s_temp = 0; // Cooler compartment temperature in degrees Celsius
static uint16_t s_voltage = 0; // Input voltage in mV
static uint16_t s_fancurrent = 0; // Fan current in mA
static uint8_t s_comppower = 0; // Compressor power in Watts

void AnalogUpdate(void) {
    uint16_t tmp = ADC_GetConversion( AN5_NTC );
    int16_t offset = (tmp - NTCMAPOFFSET) >> NTCMAPSHIFT;
    if (offset < 0) {
        s_temp = -127;
    } else if( offset > NTCMAPSIZE ) {
        s_temp = 127;
    } else {
        s_temp = ntcmap[offset];
    }

    uint16_t adcval_v = ADC_GetConversion( AN2_VoltMon );
    s_voltage = adcval_v * 54; // Close enough of an approximation for input voltage in mV

    s_fancurrent = ADC_GetConversion( AN7_FanCur ) * 22; // Initial approximation of fan current reading in mA

    uint16_t compressor = ADC_GetConversion( AN8_CompCur ); // Compressor current in 20mA steps
    s_comppower = 99; // Outside limits
    if (compressor < 0x100) { // Avoid overflow
        uint8_t voltage = (adcval_v + 2) >> 2; // Fit in byte + round
        uint16_t tmp = compressor * voltage;
        s_comppower = (tmp + 128) >> 8; // Divide with rounding
    }
}

int8_t AnalogGetTemperature(void) {
    return s_temp;
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
