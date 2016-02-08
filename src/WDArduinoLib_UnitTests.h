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

#ifndef _WDLIBB_ARDUINO_UNIT_TESTS_h
#define _WDLIBB_ARDUINO_UNIT_TESTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//----------------------------------------------------------------------------------------------------------
// Testing framework data
//----------------------------------------------------------------------------------------------------------
#define TEST_NAME_BUFFER_LEN (64)
extern long passCount;
extern long failCount;
extern long countAtLastNameChange;
extern char currentTestName[TEST_NAME_BUFFER_LEN];

//----------------------------------------------------------------------------------------------------------
// Testing framework
//----------------------------------------------------------------------------------------------------------
void setTestName(const __FlashStringHelper* name);
void setTestName(const char *name);
bool assert(bool isTrue, Print *outStream=NULL);
void pass();
void fail(Print *outStream=NULL);
void printTestSummary(Print *outStream);

template<typename T>
__attribute__ ((noinline)) bool assert_EQ(T a, T b, Print *outStream=NULL)
{
	bool ok = assert(a == b, outStream);
	if((!ok) && (outStream != NULL))
	{
		outStream->print(F("  error:  0x"));
		outStream->print(a, HEX);
		outStream->print(F(" != 0x"));
		outStream->println(b, HEX);
	}
}

bool assert_isString(const char *string, const __FlashStringHelper* expectedString, Print *outStream=NULL);
template<typename T>
__attribute__ ((noinline)) bool assert_NEQ(T a, T b, Print *outStream=NULL)
{
	bool ok = assert(a != b, outStream);
	if((!ok) && (outStream != NULL))
	{
		outStream->print(F("  error:  0x"));
		outStream->print(a, HEX);
		outStream->print(F(" != 0x"));
		outStream->println(b, HEX);
	}
}

template<typename T>
__attribute__ ((noinline)) bool assert_GT(T a, T b, Print *outStream=NULL)
{
	bool ok = assert(a > b, outStream);
	if((!ok) && (outStream != NULL))
	{
		outStream->print(F("  error:  0x"));
		outStream->print(a, HEX);
		outStream->print(F(" <= 0x"));
		outStream->println(b, HEX);
	}
}
template<typename T>
__attribute__ ((noinline)) bool assert_LT(T a, T b, Print *outStream=NULL)
{
	bool ok = assert(a < b, outStream);
	if((!ok) && (outStream != NULL))
	{
		outStream->print(F("  error:  0x"));
		outStream->print(a, HEX);
		outStream->print(F(" >= 0x"));
		outStream->println(b, HEX);
	}
}

//should cause something to stick around.
template<typename T>
void justifyExistanceOf(T something)
{
	for(int i=0; i < 100000; i++)
	{
		if(something ^ micros() != (9 + i % 31))
		{
			return;
		}
	}
	Serial.print(F("wtf"));
}

#endif //_WDLIBB_ARDUINO_UNIT_TESTS_h