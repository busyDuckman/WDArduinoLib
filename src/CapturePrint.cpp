// 
// 
// 

#include "CapturePrint.h"

//----------------------------------------------------------------------------------------------------------
CapturePrint::CapturePrint(	char *buffer, 
   							 int bufferSize, 
							 void (*onLineFinishedFunc)(const CapturePrint *cPrint),
							 bool (*onBufferOverflowFunc)(const CapturePrint *cPrint))
{
	onLineFinished = onLineFinishedFunc;
	onBufferOverflow = onBufferOverflowFunc;
	data = buffer;
	bufferLen = bufferSize;
	init();
}

//----------------------------------------------------------------------------------------------------------
void CapturePrint::init()
{
	dataPos = 0;
	eol = false;
	data[0] = 0;
	currentLineOverflowed = false;
}

//----------------------------------------------------------------------------------------------------------
size_t CapturePrint::write(uint8_t c)
{
	if((c == '\r') || (c == '\n'))
	{
		//new line
		if(eol) //because /r/n needs to be abosrbed.
		{
			if(c == '\r')
			{
				//we have a blank line
				init();
				eol = true;
				if(onLineFinished != NULL)
				{
					onLineFinished(this);
				}
			}
		}
		else
		{
			eol = true;
			if(onLineFinished != NULL)
			{
				onLineFinished(this);
			}
		}
	}
	else
	{
		if(eol)
		{
			init();
			data[0] = c;
			data[1] = 0;
			dataPos = 1;
		}
		else if(dataPos < (bufferLen-1))
		{
			data[dataPos] = c;
			dataPos++;
			data[dataPos] = 0;
		}
		else
		{
			if(onBufferOverflow != NULL)
			{
				if(onBufferOverflow(this))
				{
					init();
					data[0] = c;
					data[1] = 0;
					dataPos = 1;
				}
				else
				{
					currentLineOverflowed = true;
				}
			}
			else
			{
				currentLineOverflowed = true;
			}
		}
	}

	return 1;
}

