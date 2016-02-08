#include "BitArithmatic.h"

//--------------------------------------------------------------------------------------------------------------------------------------
inline void printLower4BitsHex(Print *outStream, const byte _value)
{
	if(outStream == NULL)
	{
		return;
	}

	byte value = _value & 0b1111;
	if(value < 10)
	{
		outStream->print((char)('0' + value));
	}
	else
	{
		outStream->print((char)('A' + value - 10));
	}
}

void printHexByte(Print *outStream, const byte value)
{
	if(outStream == NULL)
	{
		return;
	}
	printLower4BitsHex(outStream, value >> 4);
	printLower4BitsHex(outStream, value);
}

//--------------------------------------------------------------------------------------------------------------------------------------
void printHexInt16(Print *outStream, const uint16_t value)
{
	if(outStream == NULL)
	{
		return;
	}

	byte *p = (byte *)&value;
	printHexByte(outStream, p[1]);
	printHexByte(outStream, p[0]);
}

//--------------------------------------------------------------------------------------------------------------------------------------
void printHexInt24(Print *outStream, const uint32_t value)
{
	if(outStream == NULL)
	{
		return;
	}

	byte *p = (byte *)&value;
	printHexByte(outStream, p[2]);
	printHexByte(outStream, p[1]);
	printHexByte(outStream, p[0]);
	
}

//--------------------------------------------------------------------------------------------------------------------------------------
void printHexInt32(Print *outStream, const uint32_t value)
{
	if(outStream == NULL)
	{
		return;
	}

	byte *p = (byte *)&value;
	printHexByte(outStream, p[3]);
	printHexByte(outStream, p[2]);
	printHexByte(outStream, p[1]);
	printHexByte(outStream, p[0]);
}

