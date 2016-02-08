// 
// 
// 

#include "PrePrint.h"

//----------------------------------------------------------------------------------------------------------
PrePrint::PrePrint(Print *innerPrinter,
				   const char *preToken,
				   const char *postToken)
{
	state = PrePrintState::FirstChar;
	this->innerPrinter = innerPrinter;
	this->preToken = preToken;
	this->postToken = postToken;
}

//----------------------------------------------------------------------------------------------------------
void PrePrint::setInnerPrinter(Print *innerPrinter)
{
	this->innerPrinter = innerPrinter;
}

//----------------------------------------------------------------------------------------------------------
void PrePrint::printPreToken()
{
	if((innerPrinter != NULL) && (preToken != NULL))
	{
		innerPrinter->print(preToken);
	}
}

//----------------------------------------------------------------------------------------------------------
void PrePrint::printPostToken()
{
	if((innerPrinter != NULL) && (postToken != NULL))
	{
		innerPrinter->print(postToken);
	}
}

//----------------------------------------------------------------------------------------------------------
void PrePrint::printChar(char c)
{
	if(innerPrinter != NULL)
	{
		innerPrinter->print(c);
	}
}

//----------------------------------------------------------------------------------------------------------
size_t PrePrint::write(uint8_t c)
{
	//Mantra:
	//  -Always do post line stuff on firt CR or NL, new text may not follow
	//  -Only do Pre stuff when the next line arrives.
	//  -Flow through case statements, because it suits the logic.

	if(c == '\r')
	{
		switch(state)
		{
			case PrePrintState::FirstChar:
				//  => [pre][post]/r
			case PrePrintState::GotNL:
			    //  ...[post]/n  => ...[post]/n[pre][post]/r
		    case PrePrintState::GotCR:
				//  ...[post]/r  =>  ...[post]/r[pre][post]/r
				printPreToken();
			case PrePrintState::Normal:
				//  [pre]... => [pre]...[post]/r
				printPostToken();
				printChar(c);
				break;
		}
		state = PrePrintState::GotCR;
	}
	else if(c == '\n')
	{
		switch(state)
		{
			case PrePrintState::FirstChar:
				//  => [pre][post]/n
			case PrePrintState::GotNL:
			    //  ...[post]/n  => ...[post]/n[pre][post]/n
				printPreToken();
			case PrePrintState::Normal:
				//  [pre]... => [pre]...[post]/n
				printPostToken();
			case PrePrintState::GotCR: //NOTE: Different logic to before.
				//  ...[post]/r  =>  ...[post]/r/n 
				printChar(c);
				break;
		}
		state = PrePrintState::GotNL;
	}
	else
	{
		switch(state)
		{
			case PrePrintState::FirstChar:
				//  => [pre]...
			case PrePrintState::GotNL:
			    //  ...[post]/n  => ...[post]/n[pre]...
			case PrePrintState::GotCR:
				//  ...[post]/r  => ...[post]/r[pre]...
				printPreToken();
			case PrePrintState::Normal:
				//  [pre]... => [pre]....
				printChar(c);
		}
		state = PrePrintState::Normal;
	}

	return 1;
}

