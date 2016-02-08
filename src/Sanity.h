// Sanity.h
// Methods that keep me sane.
/** 
 *  @file    .h
 *  @author  Warren Creemers (duckman)
 *  @date    22/12/2015
 *  @version 1.0 
 *  
 *  @copyright Copyright 2015 Dr Warren Creemers. All rights reserved.  See Licence.txt for details.
 *
 *  @brief 
 */

#ifndef _SANITY_h
#define _SANITY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/** @public
 * Baudrates supported by the arduino platfrom.
 * @ see 
 */
enum CommonBaudRates {BaudRate300=300, BaudRate600=600, BaudRate1200=1200, BaudRate2400=2400, BaudRate4800=4800, BaudRate9600=9600, BaudRate14400=14400, 
						BaudRate19200=19200, BaudRate28800=28800, BaudRate31250=31250, BaudRate38400=38400, BaudRate57600=57600, BaudRate115200=115200};

/**
 * Clamps a value between an inclusive range.
 * ie: min <= value <= max
 * @param value value to clamp
 * @param min Inclusive min. Must be <= max.
 * @param max Inclusive max. Must be >= min.
 * @return min <= value <= max
 */
template<typename T>
inline T clamp(T value, T min, T max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

/**
 * dynamically alocate a copy of a __FlashStringHelper.
 * @param text A __FlashStringHelper storing text in progmem.
 */
char *mallocCopyOf(const __FlashStringHelper* text);

/**
 * Copy a __FlashStringHelper to local memory.
 * @param Destination buffer.
 * @param src A __FlashStringHelper storing text in progmem.
 */
char *strcpy(char *dest, const __FlashStringHelper* src);

/**
 * Copy a __FlashStringHelper to local memory. Maximum size is specified.
 * String will be null terminated.
 * @param Destination buffer.
 * @param src A __FlashStringHelper storing text in progmem.
 * @param len Maximun length of string
 * @return Number of characters copied.
 */
size_t strlcpy(char *dest, const __FlashStringHelper* src, size_t len);


/**
 * compare string to a __FlashStringHelper.
 * @param a A string to compare.
 * @param b A __FlashStringHelper other text.
 */
int strcmp(const char *a, const __FlashStringHelper* b);

#endif //_SANITY_h