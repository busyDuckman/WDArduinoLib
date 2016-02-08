#include "WDArduinoLib_UnitTests.h"
#include "BitArithmatic.h"
#include "StringValidation.h"
#include "Sanity.h"

//----------------------------------------------------------------------------------------------------------
// Testing framework
//----------------------------------------------------------------------------------------------------------
long passCount = 0;
long failCount = 0;
long countAtLastNameChange = 0;
char currentTestName[TEST_NAME_BUFFER_LEN];

//----------------------------------------------------------------------------------------------------------
void setTestName(const __FlashStringHelper* name) 
{
	currentTestName[0]= 0;
	if(name != NULL)
	{
		strlcpy_P(currentTestName, (const char PROGMEM *)name, TEST_NAME_BUFFER_LEN);
	}
	
	countAtLastNameChange = passCount+failCount;
}

//----------------------------------------------------------------------------------------------------------
void setTestName(const char *name)
{
	currentTestName[0]= 0;
	if(name != NULL)
	{
		strlcpy(currentTestName, name, TEST_NAME_BUFFER_LEN);
	}
	
	countAtLastNameChange = passCount+failCount;
}

//----------------------------------------------------------------------------------------------------------
bool assert(bool isTrue, Print *outStream)
{
	if(isTrue) 
	{
		pass();
		return true;
	}
	else 
	{
		fail(outStream);
		return false;
	}
}

//----------------------------------------------------------------------------------------------------------
void pass()
{
	passCount++;
}

//----------------------------------------------------------------------------------------------------------
void fail(Print *outStream)
{
	if(outStream != NULL)
	{
		outStream->print(F("TEST FAILED: "));
		if(currentTestName != NULL)
		{
			int localTestNum = (passCount + failCount) - countAtLastNameChange + 1;
			outStream->print(F("(")); 
			outStream->print(currentTestName); 
			outStream->print(F(" [test ")); 
			outStream->print(localTestNum);
			outStream->print(F("])"));
		}
		outStream->println();
	}
	failCount++;
}

//----------------------------------------------------------------------------------------------------------
void printTestSummary(Print *outStream)
{
	if(outStream != NULL)
	{
		if(failCount > 0)
		{
			outStream->print(passCount); Serial.println(F(" test passed"));
			outStream->print(failCount); Serial.println(F(" test FAILED"));
		}
		else
		{
			outStream->print(F("All tests passed (")); Serial.print(passCount); Serial.println(F(" tests run)"));
		}
	}
}

//----------------------------------------------------------------------------------------------------------
bool assert_isString(const char *string, const __FlashStringHelper* expectedString, Print *outStream)
{
	bool ok = assert(strcmp(string, expectedString) == 0, outStream);
	if((!ok) && (outStream != NULL))
	{
		outStream->print(F("  error: \""));
		outStream->print(string);
		outStream->print(F("\" != \""));
		outStream->print(expectedString);
		outStream->println(F("\""));
	}
}

