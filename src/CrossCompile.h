// CrossCompile.h

/**
 * It's generaly easier to develop/debug code in the develoment PC.
 * For this reson Arduino / Desktop cross compiles can be usefull.
 *
 * This header provides for getting basic code to cross compile.
 *
 * @copyright Copyright 2015 Dr Warren Creemers. All rights reserved.  See Licence.txt for details.
 *
 * @note
 *   - Defines the macros __INLINE__  and __NO_INLINE__ which work across platgorms.
 *   - Use of byte is ok
 *   - Use NULL not null
 *   - Use int32_t, uint16_t... etc, not int.
 */

#ifndef _CROSSCOMPILE_h
#define _CROSSCOMPILE_h

//-------------------------------------------------------------------------------------------------
// Establish platform
//-------------------------------------------------------------------------------------------------
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "arduino.h"
#define __GCC_TYPE_COMPILER__

#elif defined(_MSC_VER)
#define __INLINE__ __forceinline
#define __NO_INLINE__ __declspec(noinline)
#define byte uint8_t
#define NULL (0)
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <stdint.h>
#include <iomanip>
#include <string>
#include <sstream> 

#define __NOT_AN_ARDUINO_BUILD__

#elif defined(__linux__)
#define byte uint8_t
#define __GCC_TYPE_COMPILER__
#define __NOT_AN_ARDUINO_BUILD__

#else
//Old arduino... or something else
//#include "WProgram.h"  //Uncomment this if using a pre 1.0 arduino IDE [This may work, seriously though, it's time to upgrade].
#define __GCC_TYPE_COMPILER__

#endif

//-------------------------------------------------------------------------------------------------
// Stuff for when the platform is based on GCC
//-------------------------------------------------------------------------------------------------
#ifdef __GCC_TYPE_COMPILER__
#define __INLINE__ __attribute__((always_inline))
#define __NO_INLINE__ __attribute__((noinline))

#endif

//-------------------------------------------------------------------------------------------------
// Stuff for when the platform is not an arduino
//-------------------------------------------------------------------------------------------------
#ifdef __NOT_AN_ARDUINO_BUILD__

#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

//-------------------------------------------------------------------------------------------------
// Arduino Helpers
//-------------------------------------------------------------------------------------------------
class __FlashStringHelper
{
public:
	const char* string;
	__FlashStringHelper(const char* text) : string(text) {}
};
#define F(string_literal) __FlashStringHelper(string_literal)
//(reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

//-------------------------------------------------------------------------------------------------
// Mocking print
//-------------------------------------------------------------------------------------------------
class Print
{
protected:
	std::ostream *sout;
	std::stringstream buffer;
	virtual size_t countingFlush() 
	{
		int s = buffer.str().length();
		if (sout != NULL)
		{
			*sout << buffer.str().c_str();
			sout->flush();
		}
		buffer.str("");
		buffer.clear();
		return s;
	}

public:
	Print(std::ostream *out) : sout(out) {  }
	void setSimulatedOutput(std::ostream *out) { sout = out; }
	int getWriteError() { return false; }
	void clearWriteError() {  }
	virtual size_t write(uint8_t c) { *sout << (char) c; return 1; };

	virtual size_t write(const uint8_t *buffer, size_t size)
	{
		int i=0; 
		for (i = 0; i < size; i++)
		{
			write(buffer[i]);
		}
		return i;
	}

	size_t write(const char *str) 
	{
		if (str == NULL)
		{
			return 0;
		}
		return write((const uint8_t *)str, strlen(str));
	}
	size_t write(const char *buffer, size_t size) { return write((const uint8_t *)buffer, size); }

	size_t print(const __FlashStringHelper &f) { return print(f.string); }
	
	//not porting String ATM
	//size_t print(const String &s) {sout << s};

	void alterBase(int fmt) 
	{
		if(sout != NULL) 
		{
			switch (fmt)
			{
			case DEC:
				*sout << std::dec;
				break;
			case HEX:
				*sout << std::hex;
				break;
			case OCT:
				*sout << std::oct;
				break;
			case BIN:
				*sout << "[NOTE: BIN NOT SUPPORTED]";
				break;
			default:
				*sout << "[NOTE: UNKNOWN PRINT FORMAT]";
				break;
			}
		}
	}

	size_t print(const char s[])					{ if(sout != NULL) { *sout << s; } return strlen(s)+1; }
	size_t print(char c)							{ if(sout != NULL) { *sout << c; } return 1; }
	size_t print(unsigned char i, int fmt = DEC)	{ return print((unsigned long)i, fmt); }
	size_t print(int i, int fmt = DEC)				{ return print((long) i, fmt); }
	size_t print(unsigned int i, int fmt = DEC)		{ return print((unsigned long) i, fmt); }
	
	//using a buffer to get length of transmition
	size_t print(long i, int fmt = DEC) 
	{
		if (sout != NULL) 
		{ 
			alterBase(fmt); 
			buffer << i; 
			alterBase(DEC); 
			return countingFlush(); 
		} 
		return 0;
	}

	size_t print(unsigned long i, int fmt = DEC)
	{
		if (sout != NULL)
		{
			alterBase(fmt);
			buffer << i; 
			alterBase(DEC);
			return countingFlush();
		}
		return 0;
	}

	size_t print(double d, int places = 2)
	{
		if(sout != NULL) 
		{ 
			*sout << std::setprecision(places) << d;  
		}
	}
	//size_t print(float f, int places = 2) { print((double)f, places); } 
	//size_t print(const Printable&);
	const size_t newCharSize = 1; 

	size_t println(const __FlashStringHelper &s)	{ size_t c = print(s); println(); return c+newCharSize;}
	//size_t println(const String &s);
	size_t println(const char s[])					{ size_t c = print(s); println(); return c+newCharSize;}
	size_t println(char c)							{ size_t s = print(c); println(); return s+newCharSize;}
	size_t println(unsigned char i, int fmt = DEC)  { size_t s = print(i, fmt); println(); return s+newCharSize;}
	size_t println(int i , int fmt = DEC)			{ size_t s = print(i, fmt); println(); return s+newCharSize;}
	size_t println(unsigned int i, int fmt=DEC)		{ size_t s = print(i, fmt); println(); return s+newCharSize;}
	size_t println(long i, int fmt = DEC)			{ size_t s = print(i, fmt); println(); return s+newCharSize;}
	size_t println(unsigned long i, int fmt=DEC)	{ size_t s = print(i, fmt); println(); return s+newCharSize;}
	size_t println(double d, int places = 2)		{ size_t s = print(d, places); println(); }
	//size_t println(const Printable&);
	size_t println()								{ if(sout != NULL) { *sout << std::endl;} return newCharSize; }
};

//-------------------------------------------------------------------------------------------------
// Mocking stream
//-------------------------------------------------------------------------------------------------
class Stream : public Print
{
public:
	virtual int available() { return 1;	}
	virtual int read() { return 0; };
	virtual int peek() { return -1; };
	virtual void flush() { }
	Stream(std::ostream *out) : Print(out) { }
};

//-------------------------------------------------------------------------------------------------
// Mocking Serial ports
//-------------------------------------------------------------------------------------------------
class HardwareSerial : public Stream
{
public:
	inline HardwareSerial() : Stream(&std::cout) {}
	void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
	void begin(unsigned long, uint8_t) {  }
	void end() {}
	int availableForWrite(void) {return 1;}
	/*
	virtual size_t write(uint8_t n)
	inline size_t write(unsigned long n)
	inline size_t write(long n) 
	inline size_t write(unsigned int n) 
	inline size_t write(int n) */
	using Print::write; // pull in write(str) and write(buf, size) from Print
	operator bool() { return true; }

	// Interrupt handlers - Not intended to be called externally
	inline void _rx_complete_irq(void);
	void _tx_udr_empty_irq(void);
};

extern HardwareSerial Serial;

#endif //__NOT_AN_ARDUINO_BUILD__


#endif //_CROSSCOMPILE_h

