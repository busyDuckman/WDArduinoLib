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

#ifndef _WD_BITARITHMATIC_
#define _WD_BITARITHMATIC_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/**
 * Prints a byte as two hex digits.
 * Note: Upper case with a leading zero.
 * @param outStream Stream to output to.
 * @param value Integer to print.
 */
void printHexByte(Print *outStream, const byte value);
/** @copydoc ::printHexByte */
void printHexInt16(Print *outStream, const uint16_t value);
/** @copydoc ::printHexByte */
void printHexInt24(Print *outStream, const uint32_t value);
/** @copydoc ::printHexByte */
void printHexInt32(Print *outStream, const uint32_t value);

/**
 * Prints a byte as two hex digits.
 * Note: Upper case with a leading zero.
 * @param outStream Stream to output to.
 * @param value Integer to print.
 * @param seperatorDist How often a space is inserted (normaly 4 or 8).
 */
template<typename T>
void printAsBinary(Print *outStream, const T value, byte seperatorDist)
{
	if(outStream == NULL)
	{
		return;
	}

	for(int i=((sizeof(T)*8)-1); i>=0; i--)
	{
		outStream->print(((value >> i) & 1) ? '1' : '0');

		if((seperatorDist > 0) && (i != 0) && ((i % seperatorDist) == 0))
		{
			outStream->print(' ');
		}
	}
}

/**
 * Prints a byte as two hex digits.
 * Note: Upper case with a leading zero.
 * @param outStream Stream to output to.
 * @param value Integer to print.
 */
template<typename T>
void printAsBinary(Print *outStream, const T value)
{
	printAsBinary(outStream, value, 0);
}

namespace _details
{
   /**
	* Stores a n bit number at given bit position
	* Only works with unsigned data types, due to c++ providing arathmentic shift, but no logical shift.
	*
	* @param bits        How many bits (n) in our number.
	* @param pos         Bit position (zero based) of the n bits to be stored.
	* @param bitUnion    The larger datatype (type S).  Must support << ~ |= and - and cast from an interger.
	* @param value       The n most significant bits of this are stored.  Must support << and cast to S
	*
	*/
	template<typename S, typename R, const int bits, const int pos>
	void setBits(S &bitUnion, const R value)
	{
		const S bitUnionMask = ~(((((S)1) << bits)-1)<<(pos+1-bits));
		bitUnion &= bitUnionMask;//clear the destination bits
		const R valueMask = (((R)1) << bits)-1;

		bitUnion |= (((S)(value&valueMask))<<(pos+1-bits));
	}
}

/**
 * Stores a n bit number at given bit position
 * Only works with unsigned data types, due to c++ providing arathmentic shift, but no logical shift.
 *
 * @param bits        How many bits (n) in our number.
 * @param pos         Bit position (zero based) of the n bits to be stored.
 * @param bitUnion    The larger datatype (type S).  Must support << ~ |= and - and cast from an interger.
 * @param value       The n most significant bits of this are stored.  Must support << and cast to S
 *
 */
template<const int bits, const int pos>
inline void setBits(uint8_t &bitUnion, uint8_t value)
{
	_details::setBits<uint8_t, uint8_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint16_t &bitUnion, uint8_t value)
{
	_details::setBits<uint16_t, uint8_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint32_t &bitUnion, uint8_t value)
{
	_details::setBits<uint32_t, uint8_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint64_t &bitUnion, uint8_t value)
{
	_details::setBits<uint64_t, uint8_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint16_t &bitUnion, uint16_t value)
{
	_details::setBits<uint16_t, uint16_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint32_t &bitUnion, uint16_t value)
{
	_details::setBits<uint32_t, uint16_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint64_t &bitUnion, uint16_t value)
{
	_details::setBits<uint64_t, uint16_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint32_t &bitUnion, uint32_t value)
{
	_details::setBits<uint32_t, uint32_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint64_t &bitUnion, uint32_t value)
{
	_details::setBits<uint64_t, uint32_t, bits, pos>(bitUnion, value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, uint8_t) */
template<const int bits, const int pos>
inline void setBits(uint64_t &bitUnion, uint64_t value)
{
	_details::setBits<uint64_t, uint64_t, bits, pos>(bitUnion, value);
}

/**
 * Stores a n bit number at given bit position
 * Only works with unsigned data types, due to c++ providing arathmentic shift, but no logical shift.
 *
 * @param bits        How many bits (n) in our number.
 * @param pos         Bit position (zero based) of the n bits to be stored.
 * @param bitUnion    The larger datatype (type S).  Must support << ~ |= and - and cast from an interger.
 * @param value       The n most significant bits of this are stored.  Must support << and cast to S
 *
 * @note: overloaded because ints are common literals
 */
template<const int bits, const int pos>
inline void setBits(uint8_t &bitUnion, const int value)
{
	_details::setBits<uint8_t, uint8_t, bits, pos>(bitUnion, (byte)value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, const int) */
template<const int bits, const int pos>
inline void setBits(uint16_t &bitUnion, const int value)
{
	_details::setBits<uint16_t, uint8_t, bits, pos>(bitUnion, (uint8_t)value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, const int) */
template<const int bits, const int pos>
inline void setBits(uint32_t &bitUnion, const int value)
{
	_details::setBits<uint32_t, uint8_t, bits, pos>(bitUnion, (uint32_t)value);
}

/** @overload ::setBits<const int, const int>(&uint8_t, const int) */
template<const int bits, const int pos>
inline void setBits(uint64_t &bitUnion, const int value)
{
	_details::setBits<uint64_t, uint8_t, bits, pos>(bitUnion, (uint32_t)value);
}


/**
 * Gets a n bit number at given bit position
 *
 * @param bits        How many bits (n) in our number.
 * @param pos         Bit position (zero based) of the n bits to be stored.
 * @param bitUnion    The larger datatype.  
 * @return           The value from the specified bits
 *
 */
template<typename S, typename R, const int bits, const int pos>
inline R getBits(const S bitUnion)
{
	//R is often smaller than S, and some archetectures may favor working in R where possible.
	const R mask = ((((R)1) << bits)-1);
	return ((R)(bitUnion>>(pos+1-bits))) & mask;
}

/**
 * Gets a n bit number at given bit position
 *
 * @param bits        How many bits (n) in our number.
 * @param pos         Bit position (zero based) of the n bits to be stored.
 * @param bitUnion    The larger datatype.  
 * @return           The value from the specified bits
 *
 */
template<const int bits, const int pos>
inline unsigned int getUnsignedInt(const uint8_t bitUnion)
{
	return getBits<uint8_t, unsigned int, bits, pos>(bitUnion);
}

/** @overload ::getUnsignedInt<const int, const int>(const uint8_t)  */
template<const int bits, const int pos>
inline unsigned int getUnsignedInt(const uint16_t bitUnion)
{
	return getBits<uint16_t, unsigned int, bits, pos>(bitUnion);
}

/** @overload ::getUnsignedInt<const int, const int>(const uint8_t)  */
template<const int bits, const int pos>
inline unsigned int getUnsignedInt(const uint32_t bitUnion)
{
	return getBits<uint32_t, unsigned int, bits, pos>(bitUnion);
}

/** @overload ::getUnsignedInt<const int, const int>(const uint8_t)  */
template<const int bits, const int pos>
inline unsigned int getUnsignedInt(const uint64_t bitUnion)
{
	return getBits<uint64_t, unsigned int, bits, pos>(bitUnion);
}

/**
 * Assembles a larger integer from bytes.
 * @param a highest byte
 * @param b next byte
 * @param c next byte
 * @param d lowest byte
 * @return An unsigned integer (32 bit)
 */
inline uint32_t bytesTo32bit(byte a, byte b, byte c, byte d)
{
	//I am relying on the compiler to do certian optimisations
	uint32_t r;
	byte *p = (byte *)&r;
	p[0] = d;	p[1] = c;	p[2] = b;	p[3] = a;
	return r;
}

/**
 * Assembles a larger integer from bytes.
 * @param a highest byte
 * @param b lowest byte
 * @return An unsigned integer (16 bit)
 */
inline uint16_t bytesTo16bit(byte a, byte b)
{
	//I am relying on the compiler to do certian optimisations
	uint16_t r;
	byte *p = (byte *)&r;
	p[0] = b;	p[1] = a;
	return r;
}


#endif //_WD_BITARITHMATIC_