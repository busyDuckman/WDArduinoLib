/** 
 *  @file    PrePrint.h
 *  @author  Warren Creemers (duckman)
 *  @date    22/12/2015
 *  @version 1.0 
 *  
 *  @copyright Copyright 2015 Dr Warren Creemers. All rights reserved.  See Licence.txt for details.
 *
 *  @brief PrePrint class.
 */

#ifndef _PREPRINT_h
#define _PREPRINT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum PrePrintState {GotCR, GotNL, Normal, FirstChar};

/**
 * PrePrint implements Print re-directing all output to another print device, but
 * with something printed before every line.
 *  - Indent a printing routine.
 *  - prepend a comment character.
 */
class PrePrint : public Print
{
 protected:
	 PrePrintState state;
	 Print *innerPrinter;

	 const char *preToken;
	 const char *postToken;

	 void printPreToken();
	 void printPostToken();
	 void printChar(char c);

 public:

   /**
	* CapturePrint constructor.
	*
	* @param innerPrinter device to print to. NULL to disable.
	* @param preToken Token to print before any line. NULL to disable.
	* @param aPostToken Token to print at the end of any line. NULL to disable.
	*/
	PrePrint(Print *innerPrinter, const char *aPreToken, const char *aPostToken);

   /**
	* Reconfigure the output device.
	* @param innerPrinter device to print to. NULL to disable.
	*/
	void setInnerPrinter(Print *innerPrinter);

   /**
	* Write a character to the buffer.
	* @param c character to write.
	*/
	virtual size_t write(uint8_t c);

};


#endif  //_PREPRINT_h

