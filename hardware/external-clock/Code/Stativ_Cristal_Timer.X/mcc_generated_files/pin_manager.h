/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB® Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC18F13K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
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

// get/set IO_RA4 aliases
#define IO_RA4_TRIS               TRISA4
#define IO_RA4_LAT                LATA4
#define IO_RA4_PORT               PORTAbits.RA4
#define IO_RA4_WPU                WPUA4
#define IO_RA4_ANS                ANS3
#define IO_RA4_SetHigh()    do { LATA4 = 1; } while(0)
#define IO_RA4_SetLow()   do { LATA4 = 0; } while(0)
#define IO_RA4_Toggle()   do { LATA4 = ~LATA4; } while(0)
#define IO_RA4_GetValue()         PORTAbits.RA4
#define IO_RA4_SetDigitalInput()    do { TRISA4 = 1; } while(0)
#define IO_RA4_SetDigitalOutput()   do { TRISA4 = 0; } while(0)

#define IO_RA4_SetPullup()    do { WPUA4 = 1; } while(0)
#define IO_RA4_ResetPullup()   do { WPUA4 = 0; } while(0)
#define IO_RA4_SetAnalogMode()   do { ANS3 = 1; } while(0)
#define IO_RA4_SetDigitalMode()   do { ANS3 = 0; } while(0)
// get/set SDA aliases
#define SDA_TRIS               TRISB4
#define SDA_LAT                LATB4
#define SDA_PORT               PORTBbits.RB4
#define SDA_WPU                WPUB4
#define SDA_ANS                ANS10
#define SDA_SetHigh()    do { LATB4 = 1; } while(0)
#define SDA_SetLow()   do { LATB4 = 0; } while(0)
#define SDA_Toggle()   do { LATB4 = ~LATB4; } while(0)
#define SDA_GetValue()         PORTBbits.RB4
#define SDA_SetDigitalInput()    do { TRISB4 = 1; } while(0)
#define SDA_SetDigitalOutput()   do { TRISB4 = 0; } while(0)

#define SDA_SetPullup()    do { WPUB4 = 1; } while(0)
#define SDA_ResetPullup()   do { WPUB4 = 0; } while(0)
#define SDA_SetAnalogMode()   do { ANS10 = 1; } while(0)
#define SDA_SetDigitalMode()   do { ANS10 = 0; } while(0)
// get/set SCL aliases
#define SCL_TRIS               TRISB6
#define SCL_LAT                LATB6
#define SCL_PORT               PORTBbits.RB6
#define SCL_WPU                WPUB6
#define SCL_SetHigh()    do { LATB6 = 1; } while(0)
#define SCL_SetLow()   do { LATB6 = 0; } while(0)
#define SCL_Toggle()   do { LATB6 = ~LATB6; } while(0)
#define SCL_GetValue()         PORTBbits.RB6
#define SCL_SetDigitalInput()    do { TRISB6 = 1; } while(0)
#define SCL_SetDigitalOutput()   do { TRISB6 = 0; } while(0)

#define SCL_SetPullup()    do { WPUB6 = 1; } while(0)
#define SCL_ResetPullup()   do { WPUB6 = 0; } while(0)

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    GPIO and peripheral I/O initialization
 * @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize(void);

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