/*
 * irmcf183.c - IRMCF183 UART interface for Mobicool FR34/FR40 compressor cooler
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
#include "irmcf183.h"

// Only the following speeds seems to be recognized, all others are silently set to ~65
static const uint8_t s_supported_speeds[] = { 3, 6, 9, 17, 19, 22, 25, 33, 35, 38, 41,
                                              49, 51, 54, 57, 65, 67, 70, 73, 81, 83 };
#define NUM_SPEEDS (sizeof(s_supported_speeds) / sizeof(s_supported_speeds[0]))

static bool s_compressor_on = false;

static void UART_Xmit(uint8_t* buf, uint8_t len) {
    uint8_t chksum = 0;
    for (uint8_t i = 0; i < len; i++) {
        chksum += buf[i];
        while (!EUSART_is_tx_ready());
        EUSART_Write(buf[i]);
    }
    while (!EUSART_is_tx_ready());
    EUSART_Write(chksum);
}

void Compressor_Init(void) {
    UART_Xmit( (uint8_t[]){0xe1, 0xeb, 0x90, 0x00, 0x00, 0x00, 0x00}, 7);
}

void Compressor_OnOff(bool on, bool fanon, uint8_t speedidx) {
    IO_DCDCEna_LAT = fanon;
    IO_FanEna_LAT = fanon;
    IO_Comp12VCtrl_LAT = fanon;
    uint8_t newspeed = 0;
    if (speedidx >= NUM_SPEEDS) speedidx = NUM_SPEEDS - 1;
    newspeed = s_supported_speeds[speedidx];
    UART_Xmit( (uint8_t[]){0xe1, 0xeb, 0x90, on ? 1 : 0, newspeed, 0x00, 0x00}, 7);
    s_compressor_on = on;
}

bool Compressor_IsOn(void) {
    return s_compressor_on;
}

uint8_t Compressor_GetMinSpeedIdx(void) {
    return 7;
}

uint8_t Compressor_GetMaxSpeedIdx(void) {
    return NUM_SPEEDS - 1;
}

uint8_t Compressor_GetDefaultSpeedIdx(void) {
    return 10;
}
