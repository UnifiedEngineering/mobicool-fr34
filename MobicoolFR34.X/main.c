/*
 * main.c - Mobicool FR34/FR40 compressor cooler alternate PIC16F1829 firmware
 *          (because I wanted to lower the minimum setpoint from -10C to -18C)
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

// Todo:
// Implement SET (maybe F/C switch, battery monitor level selection)
// Implement fan over-current error handling
// Implement compressor stall error reporting
// Implement display timeout to return to idle after a few seconds
// Average numbers on display (also getting rid of the 10ms delay in the loop)
// Brightness control of display, including dimming after a while in idle
// On/Off
// Implement battery monitor, original manual describes Lo/MEd/Hi as follows:
//  Lo, MEd, Hi, (and diS maybe)
//  12V off 10.1V, 11.4V, 11.8V
//  12V on  11.1V, 12.2V, 12.6V
//  24V off 21.5V, 24.1V, 24.6V
//  24V on  23.0V, 25.3V, 26.2V

// This was all that was stored in the EEPROM in original firmware:
// af 2c 00
// 02 04
// This indicates set temperature (+5C I believe), on/off state (likely On) and battery monitor selection (Lo) somehow

#include "mcc_generated_files/mcc.h"
#include "tm1620b.h"
#include "analog.h"
#include "irmcf183.h"

#define MAX_TEMP (10)
#define MIN_TEMP (-18)
#define DEFAULT_TEMP MAX_TEMP

#define MAGIC ('W')

typedef enum {
    IDLE = 0,

    SET_BEGIN,
    SET_TEMP,
    SET_UNIT,
    SET_BATTMON,
    SET_END,

    DISP_BEGIN,
    DISP_VOLT,
    DISP_COMPPOWER,
    DISP_COMPTIMER,
    DISP_COMPSPEED,
    DISP_FANCURRENT,
    DISP_END,
} displaystate_t;

typedef enum {
    EE_MAGIC = 0,
    EE_ONOFF,
    EE_TEMP,
    EE_UNIT,
    EE_BATTMON,
} eedata_t;

typedef enum {
    COMP_OFF = 0,
    COMP_STARTING,
    COMP_RUN,
} comp_state_t;

void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    IO_LightEna_SetHigh();
    TM1620B_Init();
    TM1620B_Update( (uint8_t[]){0, c_U, c_E, c_o, c_S} );

    __delay_ms(200);
    Compressor_Init();
    __delay_ms(1800);

    displaystate_t cur_state = IDLE;
    uint8_t lastkeys = 0;
    uint8_t flashtimer = 0;
    uint16_t seconds = 0;
    uint8_t comp_timer = 0;
    uint8_t comp_speed = 0;
    comp_state_t compstate = COMP_OFF;

    bool eeinvalid = false;
    if (DATAEE_ReadByte(EE_MAGIC) != MAGIC) {
        eeinvalid = true;
    }
    int8_t temp_setpoint = DATAEE_ReadByte(EE_TEMP);
    if (temp_setpoint < MIN_TEMP || temp_setpoint > MAX_TEMP) {
        eeinvalid = true;
    }
    if (eeinvalid) {
        temp_setpoint = DEFAULT_TEMP;
        DATAEE_WriteByte(EE_TEMP, temp_setpoint);
        DATAEE_WriteByte(EE_MAGIC, MAGIC); // Always write magic at the end
    }
    
    int8_t newtemp = temp_setpoint;
    int16_t temp_setpoint10 = temp_setpoint * 10;
    
    while (1) {
        bool compressor_check = false;
        if (TMR1_HasOverflowOccured()) {
            TMR1_Reload();
            PIR1bits.TMR1IF = 0;
            seconds++;
            compressor_check = true;
        }

        AnalogUpdate();
        int16_t temperature10 = AnalogGetTemperature10();
        uint16_t voltage = AnalogGetVoltage();
        uint16_t fancurrent = AnalogGetFanCurrent();
        uint8_t comppower = AnalogGetCompPower();

        uint8_t keys = TM1620B_GetKeys();
        uint8_t pressed_keys = keys & ~lastkeys;

        bool comp_on = Compressor_IsOn();
        uint8_t leds = /*orange*/!comp_on << 7 | /*err*/0b0 << 6 | /*green*/comp_on << 4;

        if (pressed_keys & KEY_ONOFF) {
            // Implement actual power off/on here some day, on long-press
            // Short presses toggles between different status displays
            if (cur_state < DISP_BEGIN || cur_state > DISP_END) cur_state = DISP_BEGIN;
            cur_state++;
            if (cur_state == DISP_END) cur_state = IDLE;
        }
        
        if (pressed_keys & KEY_SET) {
            if (cur_state < SET_BEGIN || cur_state > SET_END) {
                cur_state = SET_BEGIN;
                newtemp = temp_setpoint;
            }
            cur_state++;
            if (cur_state == SET_END) cur_state = IDLE;
        }
        
        if (cur_state == IDLE) { // Perform housekeeping if we need to update settings
            if (newtemp != temp_setpoint) {
                temp_setpoint = newtemp;
                temp_setpoint10 = newtemp * 10;
                DATAEE_WriteByte(EE_TEMP, temp_setpoint);
            }
        }
        
        switch (cur_state) {
            case DISP_VOLT: {
                uint8_t buf[5];
                uint16_t dispvolt = ((voltage + 64) >> 7) + ((voltage + 256) >> 9) + ((voltage + 2048) >> 12); // decivolt
                uint8_t num = FormatDigits(NULL, dispvolt, 2);
                buf[0] = leds;
                buf[1] = 0;
                FormatDigits(&buf[4 - num], dispvolt, 2);
                buf[3] |= ADD_DOT;
                buf[4] = c_V;
                TM1620B_Update( buf );
                break;
            }
            case DISP_COMPPOWER: {
                uint8_t buf[3];
                buf[1] = 0;
                FormatDigits(buf, comppower, 0);
                TM1620B_Update( (uint8_t[]){leds, c_C, 0, buf[0], buf[1]} );
                break;
            }
            case DISP_COMPTIMER: {
                uint8_t buf[3];
                buf[1] = 0;
                FormatDigits(buf, comp_timer, 0);
                TM1620B_Update( (uint8_t[]){leds, c_t, 0, buf[0], buf[1]} );
                break;
            }
            case DISP_COMPSPEED: {
                uint8_t buf[3];
                buf[1] = 0;
                FormatDigits(buf, comp_speed, 0);
                TM1620B_Update( (uint8_t[]){leds, c_r, 0, buf[0], buf[1]} );
                break;
            }
            case DISP_FANCURRENT: {
                uint8_t buf[5];
                uint16_t dispamp = ((fancurrent + 64) >> 7) + ((fancurrent + 256) >> 9) + ((fancurrent + 2048) >> 12); // deciamp
                uint8_t num = FormatDigits(NULL, dispamp, 2);
                buf[0] = leds;
                buf[1] = c_F;
                FormatDigits(&buf[4 - num], dispamp, 2);
                buf[3] |= ADD_DOT;
                buf[4] = c_A;
                TM1620B_Update( buf );
                break;
            }
            case SET_TEMP: {
                uint8_t buf[5] = {leds, 0, 0, 0, c_C | ADD_DOT};
                if (pressed_keys) flashtimer = 0; // restart flash timer on every keypress
                if (pressed_keys & KEY_MINUS && newtemp > MIN_TEMP) newtemp--;
                if (pressed_keys & KEY_PLUS && newtemp < MAX_TEMP) newtemp++;
                if (!(flashtimer & 0x08)) {
                    uint8_t num = FormatDigits(NULL, newtemp, 0);
                    FormatDigits(&buf[4 - num], newtemp, 0); // Right justified
                }
                TM1620B_Update( buf );
                flashtimer++;
                break;
            }
            case SET_UNIT:
                TM1620B_Update( (uint8_t[]){leds, 0, 0, 0, c_C | ADD_DOT} ); // Only Celsius at the moment
                break;
            case SET_BATTMON:
                TM1620B_Update( (uint8_t[]){leds, 0, c_d, c_i, c_S} ); // No battery monitor at the moment
                break;
            case IDLE: {
                uint8_t buf[5] = {leds, 0, 0, 0, c_C | ADD_DOT};
                uint8_t num = FormatDigits(NULL, temperature, 0);
                FormatDigits(&buf[4 - num], temperature, 0); // Right justified
                TM1620B_Update( buf );
                break;
            }
            case SET_BEGIN:
            case SET_END:
            case DISP_BEGIN:
            case DISP_END:
                break;
        }
        __delay_ms(10);
        
        if (compressor_check) {
            uint8_t speed = 0;
            static uint8_t fanspin = 0;
            int16_t tempdiff = (temperature10 - temp_setpoint10);
            if (comp_timer > 0) {
                comp_timer--;
                if (comp_timer == 0) compstate++;
            }
            if (fanspin > 0) fanspin--;
            switch (compstate) {
                case COMP_OFF:
                    if (tempdiff > 0 && comp_timer == 0) {
                        comp_timer = 20;
                    }
                    if (comp_timer == 5) fanspin = 5;
                    Compressor_OnOff(false, fanspin > 0, speed); // Stopped
                    break;
                case COMP_STARTING:
                    speed = 41; // Start at idle speed (revolutions per second)
                    Compressor_OnOff(true, true, speed);
                    if (comp_timer == 0) {
                        comp_timer = 30;
                    }
                    break;
                case COMP_RUN:
                    speed = comp_speed;
                    if (comppower <= 45 && speed < 75 && tempdiff > 2) {
                        speed++;
                    } else if ((comppower > 45 || tempdiff <= 1) && speed > 41) {
                        speed--;
                    }
                    if (tempdiff < 0) {
                        compstate = COMP_OFF;
                        fanspin = 120;
                    } else {
                        Compressor_OnOff(true, true, speed);
                    }
                    break;
            }
            comp_speed = speed;
        }
        lastkeys = keys;
    }
}
