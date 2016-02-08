/** 
 *  @file    CapturePrint.h
 *  @author  Warren Creemers (duckman)
 *  @date    22/12/2015
 *  @version 1.0 
 *  
 *  @copyright Copyright 2015 Dr Warren Creemers. All rights reserved.  See Licence.txt for details.
 *
 *  @brief CapturePrint classe.
 */

#ifndef _CAPTUREPRINT_H
#define _CAPTUREPRINT_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/**
 * CapturePrint implements Print re-directing all output to a buffer.
 *  - This class will maintain a buffer as a null terminated char[].
 *  - This buffer will only ever hold one line, it is flushed on NL or CR char (\r\n smarts inbuilt).
 *  - \0 can be printed to this buffer.
 *  - Full buffers recive no new data until a newline is sent.
 *  - It can is call a method every newline (\r\n smarts inbuilt).
 *  - It can call a method is called every time an overflow occurs.
 *    - If it return true the buffer is reset and overflow flag cleared, allowing for more data.
 *    .
 *  .
 * @warning bufferSize must be >= 2
 */
class CapturePrint : public Print
{
 protected:
	 int bufferLen;
	 char *data;
	 int dataPos;
	 bool eol;
	 bool currentLineOverflowed;

	 //void onLineFinished(const CapturePrint& cPrint)
	 void (*onLineFinished)(const CapturePrint *cPrint);

	 //bool onBufferOverflow(const CapturePrint& cPrint)
	 bool (*onBufferOverflow)(const CapturePrint *cPrint);


 public:
   /**
	* CapturePrint constructor.
	*
	* @param buffer where data is stored. 
	*               - This class will maintain the buffer as a null terminated char[].
	*               - This buffer will only ever hold one line, it is flushed on NL or CR char.
	*               - \0 can be printed to this buffer.
	*               - \0 can be printed to this buffer.
	*               - Full buffers recive no new data until a newline is sent.
	* @param bufferSize size of buffer. NB: bufferSize-1 is the max string size, to allow for null termination.
	* @warning bufferSize must be >= 2
	*/
	CapturePrint(char *buffer, int bufferSize) : CapturePrint(buffer, bufferSize, NULL, NULL) { }

   /**
	* CapturePrint constructor.
	*
	* @param buffer where data is stored. 
	*               - This class will maintain the buffer as a null terminated char[].
	*               - This buffer will only ever hold one line, it is flushed on NL or CR char (\r\n smarts inbuilt).
	*               - \0 can be printed to this buffer.
	*               - Full buffers recive no new data until a newline is sent.
	* @param bufferSize size of buffer. NB: bufferSize-1 is the max string size, to allow for null termination.
	* @param onLineFinishedFunc This method is called every newline
	*               - \r\n smarts inbuilt.
	*               - Example function: bool onBufferOverflow(const CapturePrint *cPrint) { }
	* @param onLineFinishedFunc This method is called every time an overflow occurs.
	*               - If it return true the buffer is reset and overflow flag cleared, allowing for more data.
	*               - Example function: void onLineFinished(const CapturePrint *cPrint) { }
	* @warning bufferSize must be >= 2
	*/
	CapturePrint(char *buffer, 
				 int bufferSize, 
				 void (*onLineFinishedFunc)(const CapturePrint *cPrint),
				 bool (*onBufferOverflowFunc)(const CapturePrint *cPrint));

   /**
	* Resets the class.
	* NB: called by constructor, alo suitable for general use.
	*/
	void init();

   /**
	* Write a character to the buffer.
	* @param c character to write.
	*/
	virtual size_t write(uint8_t c);
	
   /**
	* Returns a pointer to the buffer.
	*/
	char * const& getLastLineBuffer() const { return data; }

   /**
	* Returns the size of the buffer (_not_ the length of the current string).
	*/
	char * const& getMaxBufferSize() const { return data; }

   /**
	* Return true if the buffer overflowed (and overflow was not handled my delegate).
	*/
	bool overflowed() const	{ return currentLineOverflowed; }

   /**
	* Return true if we are at the end of the current line (buffer will hold the current line).
	*/
	bool endOfLine() const	{ return eol; }
};


#endif

