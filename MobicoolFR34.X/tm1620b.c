/*
 * tm1620b.c - TM1620B display/keypad driver as used in Mobicool
 *             FR34/FR40 compressor cooler
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
#include "tm1620b.h"

/*
 * 5 grids, 8 segments
 * GRID 4 3 2 1
 *
 * DOT=SEG2   SEG3 
 *          S      S
 *          E      E
 *          G      G
 *          1      4
 *            SEG13
 *          S      S
 *          E      E
 *          G      G
 *          1      6
 *          2      
 *            SEG5
 * 
 * GRID5
 * seg 13 = bicolor1
 * seg 12 = error LED
 * seg 5 = bicolor2
 * 
 * Display buffer is reversed so first byte contains LEDs, then the four digits in correct order:
 * byte 0: bit 7 = bicolor orange, bit 6 = error LED, bit 4 = bicolor green
 * byte 1-4, digit 1 to 4 as follows:
 *  
 * DOT=bit1   bit2 
 *          b      b
 *          i      i
 *          t      t
 *          0      3
 *            bit7
 *          b      b
 *          i      i
 *          t      t
 *          6      5
 *            bit4
 * 
 * Key mapping:
 * bit function (bit set when key is pressed)
 *  0  Down
 *  1  Up
 *  2  Set
 *  3  On/Off
 * 
 */

const uint8_t hexdigits[] = { 0b01111101, 0b00101000, 0b11011100, 0b10111100, // 0-3
                                     0b10101001, 0b10110101, 0b11110101, 0b00101100, // 4-7
                                     0b11111101, 0b10111101, 0b11101101, 0b11110001, // 8-b
                                     0b01010101, 0b11111000, 0b11010101, 0b11000101}; // c-f

// Instruction command sets
#define TM_DISPMODE (0b00 << 6)
#define TM_DATA (0b01 << 6)
#define TM_DISPCTRL (0b10 << 6)
#define TM_ADDR (0b11 << 6)

// Number of grids x segments
#define TM_DM_4X9 (0b00 << 0)
#define TM_DM_5X8 (0b01 << 0)
#define TM_DM_6X7 (0b10 << 0)
#define TM_DM_7X6 (0b11 << 0)

#define TM_D_WRITE (0b00 << 0) // Display write
#define TM_D_READ (0b10 << 0) // Key read
#define TM_D_ADDR_FIXED (0b1 << 2)
#define TM_D_ADDR_INCR (0b0 << 2)

#define TM_DC_BRIGHTNESS(x) (x & 0x7)
#define TM_DC_ENABLE (0b1 << 3)

#define TM_A_DISPADDR(x) (x & 0xf)

// No additional delays are needed here because of the slow CPU speed

static uint8_t TM1620B_Recv(bool stbhigh) {
    uint8_t data = 0;
    for (uint8_t b = 0; b < 8; b++) {
        IO_CLK_SetLow();
        data >>= 1;
        data |= (IO_DIO_PORT << 7);
        IO_CLK_SetHigh();
    }

    if (stbhigh) {
        IO_STB_SetHigh();
    }
    return data;
}

static void TM1620B_Send(uint8_t data, bool stbhigh) {
    IO_STB_SetLow();
    
    for (uint8_t b = 0; b < 8; b++) {
        IO_DIO_LAT = data & 1;
        data >>= 1;
        IO_CLK_SetLow();
        IO_CLK_SetHigh();
    }

    if (stbhigh) {
        IO_STB_SetHigh();
    }
}

void TM1620B_Update(uint8_t* buf) {
    TM1620B_Send(TM_ADDR | TM_A_DISPADDR(0), true);
    TM1620B_Send(TM_DATA | TM_D_WRITE | TM_D_ADDR_INCR, false);
    for (int8_t i = 4; i >= 0; i--) {
        uint8_t tmp = buf[i];
        TM1620B_Send(tmp & 0x3f, false); // Seg 1-6 (bit 0-5)
        TM1620B_Send((tmp & 0xc0) >> 3, i == 0); // Seg 12-13 (bit 6-7)
    }
}

uint8_t TM1620B_GetKeys(void) {
    TM1620B_Send(TM_DATA | TM_D_READ | TM_D_ADDR_INCR, false);
    IO_DIO_SetDigitalInput();
    uint8_t keys = 0;
    for (int8_t i = 0; i < 3; i++) {
        uint8_t tmp = TM1620B_Recv(i == 2);
        keys >>= 2; // Two keys per byte
        if (tmp & 1<<1) keys |= 1<<4;
        if (tmp & 1<<4) keys |= 1<<5;
    }
    IO_DIO_SetDigitalOutput();
    return keys;
}

void TM1620B_SetBrightness(bool on, uint8_t brightness) {
    TM1620B_Send(TM_DISPCTRL | (on ? TM_DC_ENABLE : 0) | TM_DC_BRIGHTNESS((brightness < 8) ? brightness : 7), true); // Blank display  
}

void TM1620B_Init(void) {
    IO_STB_SetHigh();
    IO_CLK_SetHigh();
    __delay_us(10);
    TM1620B_Send(TM_DISPMODE | TM_DM_5X8, true);
    TM1620B_Send(TM_DISPCTRL | TM_DC_ENABLE | TM_DC_BRIGHTNESS(4), true);
}

uint8_t FormatDigits(uint8_t* outbuf, int16_t inum, uint8_t mindigits) {
    // Max range -199 to 999, but max number of characters output is 3
    // This means that numbers between -100 and -199 will have the minus
    // and first digit combined in one character
    // Returns number of characters which will be used even if outbuf is NULL
    // mindigits sets the minimum number of digits output (useful for fixed-
    // point operation)
    if (inum < -199 || inum > 999) return 0;
    bool isnegative = inum < 0;
    uint16_t num = isnegative ? -inum : inum;
    uint8_t digits = 1;
    if (num > 9) digits++;
    if (num > 99) digits++;
    if (digits < mindigits) digits = mindigits;
    uint8_t numchars = (isnegative && digits < 3) ? (digits + 1) : digits;
    uint8_t offset = numchars - digits;
    if (outbuf) {
        outbuf[0] = 0;
        while (digits--) {
            uint8_t quotient = 0;
            while (num >= 10) {
                quotient++;
                num -= 10;
            }
            // Remainder remains in num
            outbuf[digits + offset] = hexdigits[num];
            num = quotient;
        }        
        if (isnegative) outbuf[0] |= MINUS;
    }
    return numchars;
}
