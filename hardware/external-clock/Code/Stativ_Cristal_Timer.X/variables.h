/* 
 * File:   variables.h
 * Author: keysie
 *
 * Created on July 15, 2015, 6:43 PM
 */

#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <stdint.h>

    bool toggle;
    uint8_t preloadOffsetH;
    uint8_t preloadOffsetL;
    int16_t preloadOffset;


#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

