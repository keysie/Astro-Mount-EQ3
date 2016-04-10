/**
  MSSP Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    i2c.c

  @Summary
    This is the generated header file for the MSSP driver using 
    MPLAB® Code Configurator

  @Description
    This header file provides APIs for driver for MSSP.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC18F13K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB 	          :  MPLAB X v2.35 or v3.00
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

#include "mcc.h"
#include "../variables.h"

#define I2C_SLAVE_ADDRESS 8
#define I2C_SLAVE_MASK    127

// custom commands
#define OFFSET_HIGH     1
#define OFFSET_LOW      2
#define UPDATE_OFFSET   3


typedef enum {
    SLAVE_NORMAL_DATA,
    SLAVE_DATA_ADDRESS,
} SLAVE_WRITE_DATA_TYPE;

/**
 Section: Global Variables
 */

volatile uint8_t I2C_slaveWriteData = 0x55;

/**
 Section: Local Functions
 */
void I2C_StatusCallback(I2C_SLAVE_DRIVER_STATUS i2c_bus_state);

/**
  Prototype:        void I2C_Initialize(void)
  Input:            none
  Output:           none
  Description:      I2C_Initialize is an
                    initialization routine that takes inputs from the GUI.
  Comment:          
  Usage:            I2C_Initialize();

 */
void I2C_Initialize(void) {
    // initialize the hardware
    // BF RCinprocess_TXcomplete; UA dontupdate; SMP Standard Speed; P stopbit_notdetected; S startbit_notdetected; R_nW write_noTX; CKE Idle to Active; D_nA lastbyte_address; 
    SSPSTAT = 0x80;
    // SSPEN enabled; WCOL no_collision; SSPOV no_overflow; CKP Idle:Low, Active:High; SSPM 7 Bit; 
    SSPCON1 = 0x26;
    // ACKSTAT received; RCEN disabled; RSEN disabled; ACKEN disabled; SEN enabled; ACKDT acknowledge; GCEN disabled; PEN disabled; 
    SSPCON2 = 0x01;
    // MSK 127; 
    SSPMSK = (I2C_SLAVE_MASK << 1); // adjust UI mask for R/nW bit            
    // SSPADD 8; 
    SSPADD = (I2C_SLAVE_ADDRESS << 1); // adjust UI address for R/nW bit

    // clear the slave interrupt flag
    PIR1bits.SSPIF = 0;
    // enable the master interrupt
    PIE1bits.SSPIE = 1;

}

void I2C_ISR(void) {
    uint8_t i2c_data = 0x55;


    // NOTE: The slave driver will always acknowledge
    //       any address match.

    PIR1bits.SSPIF = 0; // clear the slave interrupt flag
    i2c_data = SSPBUF; // read SSPBUF to clear BF
    if (1 == SSPSTATbits.R_nW) {
        if ((1 == SSPSTATbits.D_nA) && (1 == SSPCON2bits.ACKSTAT)) {
            // callback routine can perform any post-read processing
            I2C_StatusCallback(I2C_SLAVE_READ_COMPLETED);
        } else {
            // callback routine should write data into SSPBUF
            I2C_StatusCallback(I2C_SLAVE_READ_REQUEST);
        }
    } else if (0 == SSPSTATbits.D_nA) {
        // this is an I2C address

        // callback routine should prepare to receive data from the master
        I2C_StatusCallback(I2C_SLAVE_WRITE_REQUEST);
    } else {
        I2C_slaveWriteData = i2c_data;

        // callback routine should process I2C_slaveWriteData from the master
        I2C_StatusCallback(I2C_SLAVE_WRITE_COMPLETED);
    }

    SSPCON1bits.CKP = 1; // release SCL

} // end I2C_ISR()

/**

    Example implementation of the callback

    This slave driver emulates an EEPROM Device.
    Sequential reads from the EEPROM will return data at the next
    EEPROM address.

    Random access reads can be performed by writing a single byte
    EEPROM address, followed by 1 or more reads.

    Random access writes can be performed by writing a single byte
    EEPROM address, followed by 1 or more writes.

    Every read or write will increment the internal EEPROM address.

    When the end of the EEPROM is reached, the EEPROM address will
    continue from the start of the EEPROM.
 */

void I2C_StatusCallback(I2C_SLAVE_DRIVER_STATUS i2c_bus_state) {

    static uint8_t EEPROM_Buffer[] ={
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
    };

    static uint8_t command = 0;
    static uint8_t slaveWriteType = SLAVE_NORMAL_DATA;
    
    /* PROTOCOL: PIC acts like an EEPROM: First byte received is interpreted
       as an address, or in this case a command. The second byte is data to 
       be written to the address. 
     * 
     * Currently, there are 3 commands:
     * RECEIVE_OFFSET_HIGH: send offset MSBs after this
     * RECEIVE_OFFSET_LOW:  send offset LSBs after this
     * UPDATE_OFFSET:       concatonate MSBs&LSBs and use this as temporary 
     *                      offset. (temporary = not saved after power off)
     * TODO:
     * 
     * 
     * Because the code is still sketchy, the command can be used as real address
     * between 0x00 and 0x3F to address a ram variable in the PIC. 
     */


    switch (i2c_bus_state) {
        case I2C_SLAVE_WRITE_REQUEST:
            // the master will be sending the eeprom address next
            slaveWriteType = SLAVE_DATA_ADDRESS;
            break;


        case I2C_SLAVE_WRITE_COMPLETED:

            switch (slaveWriteType) {
                case SLAVE_DATA_ADDRESS:
                    command = I2C_slaveWriteData;
                    slaveWriteType = SLAVE_NORMAL_DATA;
                    break;


                case SLAVE_NORMAL_DATA:
                    switch(command) {
                        case OFFSET_HIGH:
                            preloadOffsetH = I2C_slaveWriteData;
                            break;
                        case OFFSET_LOW:
                            preloadOffsetL = I2C_slaveWriteData;
                            break;
                        case UPDATE_OFFSET:
                            preloadOffset = (preloadOffsetH << 8) 
                                    + preloadOffsetL;
                            DATAEE_WriteByte(0, preloadOffsetH);
                            DATAEE_WriteByte(1, preloadOffsetL);
                            break;
                    }
                    
                default:
                    // the master has written data to store in the eeprom
                    EEPROM_Buffer[command++] = I2C_slaveWriteData;
                    if (sizeof (EEPROM_Buffer) <= command) {
                        command = 0; // wrap to start of eeprom page
                    }
                    slaveWriteType = SLAVE_DATA_ADDRESS;
                    break;

            } // end switch(slaveWriteType)

            //slaveWriteType = SLAVE_NORMAL_DATA;
            break;

        case I2C_SLAVE_READ_REQUEST:
            switch(command) {
                        case OFFSET_HIGH:
                            SSPBUF = preloadOffsetH;
                            break;
                        case OFFSET_LOW:
                            SSPBUF = preloadOffsetL;
                            break;
                        default:
                            SSPBUF = EEPROM_Buffer[command++];
                            if (sizeof (EEPROM_Buffer) <= command) {
                                command = 0; // wrap to start of eeprom page
                            }
                            break;
                    }
            
            break;

        case I2C_SLAVE_READ_COMPLETED:
        default:;

    } // end switch(i2c_bus_state)

}

