/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F1829
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_LightEna aliases
#define IO_LightEna_TRIS                 TRISAbits.TRISA1
#define IO_LightEna_LAT                  LATAbits.LATA1
#define IO_LightEna_PORT                 PORTAbits.RA1
#define IO_LightEna_WPU                  WPUAbits.WPUA1
#define IO_LightEna_ANS                  ANSELAbits.ANSA1
#define IO_LightEna_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define IO_LightEna_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define IO_LightEna_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define IO_LightEna_GetValue()           PORTAbits.RA1
#define IO_LightEna_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define IO_LightEna_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define IO_LightEna_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define IO_LightEna_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define IO_LightEna_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define IO_LightEna_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set AN2_VoltMon aliases
#define AN2_VoltMon_TRIS                 TRISAbits.TRISA2
#define AN2_VoltMon_LAT                  LATAbits.LATA2
#define AN2_VoltMon_PORT                 PORTAbits.RA2
#define AN2_VoltMon_WPU                  WPUAbits.WPUA2
#define AN2_VoltMon_ANS                  ANSELAbits.ANSA2
#define AN2_VoltMon_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define AN2_VoltMon_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define AN2_VoltMon_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define AN2_VoltMon_GetValue()           PORTAbits.RA2
#define AN2_VoltMon_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define AN2_VoltMon_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define AN2_VoltMon_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define AN2_VoltMon_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define AN2_VoltMon_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define AN2_VoltMon_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set IO_DIO aliases
#define IO_DIO_TRIS                 TRISAbits.TRISA4
#define IO_DIO_LAT                  LATAbits.LATA4
#define IO_DIO_PORT                 PORTAbits.RA4
#define IO_DIO_WPU                  WPUAbits.WPUA4
#define IO_DIO_ANS                  ANSELAbits.ANSA4
#define IO_DIO_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define IO_DIO_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define IO_DIO_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define IO_DIO_GetValue()           PORTAbits.RA4
#define IO_DIO_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define IO_DIO_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define IO_DIO_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define IO_DIO_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define IO_DIO_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define IO_DIO_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set AN10_1V8Mon aliases
#define AN10_1V8Mon_TRIS                 TRISBbits.TRISB4
#define AN10_1V8Mon_LAT                  LATBbits.LATB4
#define AN10_1V8Mon_PORT                 PORTBbits.RB4
#define AN10_1V8Mon_WPU                  WPUBbits.WPUB4
#define AN10_1V8Mon_ANS                  ANSELBbits.ANSB4
#define AN10_1V8Mon_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define AN10_1V8Mon_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define AN10_1V8Mon_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define AN10_1V8Mon_GetValue()           PORTBbits.RB4
#define AN10_1V8Mon_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define AN10_1V8Mon_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define AN10_1V8Mon_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define AN10_1V8Mon_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define AN10_1V8Mon_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define AN10_1V8Mon_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()               do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()                do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()                do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()       do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()      do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set IO_FanEna aliases
#define IO_FanEna_TRIS                 TRISBbits.TRISB6
#define IO_FanEna_LAT                  LATBbits.LATB6
#define IO_FanEna_PORT                 PORTBbits.RB6
#define IO_FanEna_WPU                  WPUBbits.WPUB6
#define IO_FanEna_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define IO_FanEna_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define IO_FanEna_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define IO_FanEna_GetValue()           PORTBbits.RB6
#define IO_FanEna_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define IO_FanEna_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define IO_FanEna_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define IO_FanEna_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()               do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()                do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()                do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()       do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()      do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)

// get/set IO_Comp12VCtrl aliases
#define IO_Comp12VCtrl_TRIS                 TRISCbits.TRISC0
#define IO_Comp12VCtrl_LAT                  LATCbits.LATC0
#define IO_Comp12VCtrl_PORT                 PORTCbits.RC0
#define IO_Comp12VCtrl_WPU                  WPUCbits.WPUC0
#define IO_Comp12VCtrl_ANS                  ANSELCbits.ANSC0
#define IO_Comp12VCtrl_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define IO_Comp12VCtrl_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define IO_Comp12VCtrl_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define IO_Comp12VCtrl_GetValue()           PORTCbits.RC0
#define IO_Comp12VCtrl_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define IO_Comp12VCtrl_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define IO_Comp12VCtrl_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define IO_Comp12VCtrl_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define IO_Comp12VCtrl_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define IO_Comp12VCtrl_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set AN5_NTC aliases
#define AN5_NTC_TRIS                 TRISCbits.TRISC1
#define AN5_NTC_LAT                  LATCbits.LATC1
#define AN5_NTC_PORT                 PORTCbits.RC1
#define AN5_NTC_WPU                  WPUCbits.WPUC1
#define AN5_NTC_ANS                  ANSELCbits.ANSC1
#define AN5_NTC_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define AN5_NTC_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define AN5_NTC_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define AN5_NTC_GetValue()           PORTCbits.RC1
#define AN5_NTC_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define AN5_NTC_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define AN5_NTC_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define AN5_NTC_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define AN5_NTC_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define AN5_NTC_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set IO_DCDCEna aliases
#define IO_DCDCEna_TRIS                 TRISCbits.TRISC2
#define IO_DCDCEna_LAT                  LATCbits.LATC2
#define IO_DCDCEna_PORT                 PORTCbits.RC2
#define IO_DCDCEna_WPU                  WPUCbits.WPUC2
#define IO_DCDCEna_ANS                  ANSELCbits.ANSC2
#define IO_DCDCEna_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define IO_DCDCEna_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define IO_DCDCEna_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define IO_DCDCEna_GetValue()           PORTCbits.RC2
#define IO_DCDCEna_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define IO_DCDCEna_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define IO_DCDCEna_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define IO_DCDCEna_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define IO_DCDCEna_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define IO_DCDCEna_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set AN7_FanCur aliases
#define AN7_FanCur_TRIS                 TRISCbits.TRISC3
#define AN7_FanCur_LAT                  LATCbits.LATC3
#define AN7_FanCur_PORT                 PORTCbits.RC3
#define AN7_FanCur_WPU                  WPUCbits.WPUC3
#define AN7_FanCur_ANS                  ANSELCbits.ANSC3
#define AN7_FanCur_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define AN7_FanCur_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define AN7_FanCur_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define AN7_FanCur_GetValue()           PORTCbits.RC3
#define AN7_FanCur_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define AN7_FanCur_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define AN7_FanCur_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define AN7_FanCur_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define AN7_FanCur_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define AN7_FanCur_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set IO_STB aliases
#define IO_STB_TRIS                 TRISCbits.TRISC4
#define IO_STB_LAT                  LATCbits.LATC4
#define IO_STB_PORT                 PORTCbits.RC4
#define IO_STB_WPU                  WPUCbits.WPUC4
#define IO_STB_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_STB_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_STB_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_STB_GetValue()           PORTCbits.RC4
#define IO_STB_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_STB_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_STB_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_STB_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)

// get/set IO_CLK aliases
#define IO_CLK_TRIS                 TRISCbits.TRISC5
#define IO_CLK_LAT                  LATCbits.LATC5
#define IO_CLK_PORT                 PORTCbits.RC5
#define IO_CLK_WPU                  WPUCbits.WPUC5
#define IO_CLK_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define IO_CLK_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define IO_CLK_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define IO_CLK_GetValue()           PORTCbits.RC5
#define IO_CLK_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define IO_CLK_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define IO_CLK_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define IO_CLK_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)

// get/set AN8_CompCur aliases
#define AN8_CompCur_TRIS                 TRISCbits.TRISC6
#define AN8_CompCur_LAT                  LATCbits.LATC6
#define AN8_CompCur_PORT                 PORTCbits.RC6
#define AN8_CompCur_WPU                  WPUCbits.WPUC6
#define AN8_CompCur_ANS                  ANSELCbits.ANSC6
#define AN8_CompCur_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define AN8_CompCur_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define AN8_CompCur_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define AN8_CompCur_GetValue()           PORTCbits.RC6
#define AN8_CompCur_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define AN8_CompCur_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define AN8_CompCur_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define AN8_CompCur_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define AN8_CompCur_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define AN8_CompCur_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/