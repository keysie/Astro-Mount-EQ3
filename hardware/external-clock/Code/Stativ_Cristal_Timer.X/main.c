/* 
 * File:   main.c
 * Author: keysie
 *
 * Created on July 15, 2015, 2:30 PM
 */

//#include <pic18f13k22.h>
#include "variables.h"
#include "mcc_generated_files/mcc.h"


void setup()
{
    // load last used offset from eeprom
    preloadOffsetH = DATAEE_ReadByte(0);
    preloadOffsetL = DATAEE_ReadByte(1);
    preloadOffset = (preloadOffsetH << 8) 
                                    + preloadOffsetL;
    
    // call system setup
    SYSTEM_Initialize();
    
    preloadOffset = 0;
            
    // Enable interrupts!!
    INTCONbits.PEIE = 1;
    INTCONbits.GIE  = 1;
    
    toggle = true;
}

/*
 * 
 */
int main() 
{
    
    // setup the whole thing
    setup();
    
    while (1==1)
    {
        //asm("sleep");
        // no sleep for now.
    }
}

