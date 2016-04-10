/**
  TMR0 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.h

  @Summary
    This is the generated header file for the TMR0 driver using MPLAB® Code Configurator

  @Description
    This header file provides APIs for TMR0.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC18F13K22
        Driver Version    :  2.00
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

#ifndef _TMR0_H
#define _TMR0_H

/**
  Section: Included Files
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif



    /**
      Section: Macro Declarations
     */

#define TMR0_INTERRUPT_TICKER_FACTOR    1

    /**
      Section: TMR0 APIs
     */

    /**
      @Summary
        Initializes the TMR0.

      @Description
        This function initializes the TMR0 Registers.
        This function must be called before any other TMR0 function is called.

      @Preconditions
        None

      @Param
        None

      @Returns
        None

      @Comment
    

      @Example
        <code>
        main()
        {
            // Initialize TMR0 module
            TMR0_Initialize();

            // Do something else...
        }
        </code>
     */
    void TMR0_Initialize(void);

    /**
      @Summary
        This function starts the TMR0.

      @Description
        This function starts the TMR0 operation.
        This function must be called after the initialization of TMR0.

      @Preconditions
        Initialize  the TMR0 before calling this function.

      @Param
        None

      @Returns
        None

      @Example
        <code>
        // Initialize TMR0 module

        // Start TMR0
        TMR0_StartTimer();

        // Do something else...
        </code>
     */
    void TMR0_StartTimer(void);

    /**
      @Summary
        This function stops the TMR0.

      @Description
        This function stops the TMR0 operation.
        This function must be called after the start of TMR0.

      @Preconditions
        Initialize  the TMR0 before calling this function.

      @Param
        None

      @Returns
        None

      @Example
        <code>
        // Initialize TMR0 module

        // Start TMR0
        TMR0_StartTimer();

        // Do something else...

        // Stop TMR0;
        TMR0_StopTimer();
        </code>
     */
    void TMR0_StopTimer(void);


    /**
      @Summary
        Reads the 16 bits TMR0 register value.

      @Description
        This function reads the 16 bits TMR0 register value and return it.

      @Preconditions
        Initialize  the TMR0 before calling this function.

      @Param
        None

      @Returns
        This function returns the 16 bits value of TMR0 register.

      @Example
        <code>
        // Initialize TMR0 module

        // Start TMR0
        TMR0_StartTimer();

        // Read the current value of TMR0
        if(0 == TMR0_Read16bitTimer())
        {
            // Do something else...

            // Reload the TMR value
            TMR0_Reload();
        }
        </code>
     */
    uint16_t TMR0_Read16bitTimer(void);

    /**
      @Summary
        Writes the 16 bits value to TMR0 register.

      @Description
        This function writes the 16 bits value to TMR0 register.
        This function must be called after the initialization of TMR0.

      @Preconditions
        Initialize  the TMR0 before calling this function.

      @Param
        timerVal - Value to write into TMR0 register.

      @Returns
        None

      @Example
        <code>
        #define PERIOD 0x8000
        #define ZERO   0x0000

        while(1)
        {
            //Read the TMR0 register
            if(ZERO == TMR0_Read16bitTimer())
            {
                // Do something else...

                // Write the TMR0 register
                TMR0_Write16bitTimer(PERIOD);
            }

            // Do something else...
        }
        </code>
     */
    void TMR0_Write16bitTimer(uint16_t timerVal);

    /**
      @Summary
        Reload the 16 bits value to TMR0 register.

      @Description
        This function reloads the 16 bit value to TMR0 register.
        This function must be called to write initial value into TMR0 register.

      @Preconditions
        Initialize  the TMR0 before calling this function.

      @Param
        None

      @Returns
        None

      @Example
        <code>
        while(1)
        {
            if(TMR0IF)
            {
                // Do something else...

                // clear the TMR0 interrupt flag
                TMR0IF = 0;

                // Reload the initial value of TMR0
                TMR0_Reload16bit();
            }
        }
        </code>
     */
    void TMR0_Reload16bit(void);

    /**
      @Summary
        Timer Interrupt Service Routine

      @Description
        Timer Interrupt Service Routine is called by the Interrupt Manager.

      @Preconditions
        Initialize  the TMR0 module with interrupt before calling this isr.

      @Param
        None

      @Returns
        None
     */
    void TMR0_ISR(void);

    /**
      @Summary
        CallBack function

      @Description
        This function is called from the timer ISR. User can write your code in this function.

      @Preconditions
        Initialize  the TMR0 module with interrupt before calling this function.

      @Param
        None

      @Returns
        None
     */
    void TMR0_CallBack(void);

#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif

#endif // _TMR0_H
/**
 End of File
 */