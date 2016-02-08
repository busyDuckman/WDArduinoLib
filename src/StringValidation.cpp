// 
// 
// 

#include "StringValidation.h"

//what we replace with if all else fails.
#define SAFE_CHAR '_'

//--------------------------------------------------------------------------------------------------------------------------------------
char *mallocSanitizedCopy(const char*string, StringValidationRules validation, int maxLen)
{
	//get the size of the sanitized string
	int bufferLen = (maxLen >= 0) ? min(maxLen, strlen(string)) : strlen(string);
	if(bufferLen == 0)
	{
		return NULL;
	}

	//alloc buffer
	char *buffer = (char *)malloc(bufferLen+1);
	buffer[bufferLen] = 0; //null terminate
	if(sanitizeString(const_cast<char*>(string), buffer, validation, maxLen) == buffer)
	{
		return buffer;
	}
	else
	{
		free(buffer);
		return NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
char *sanitizeString(char* string, char *buffer, StringValidationRules validation, int maxLen)
{
	bool debug = validation == (StringValidationRules::NoWhiteSpace | StringValidationRules::NoTabs);
	//get the size of the sanitized string
	int bufferLen = (maxLen >= 0) ? min(maxLen, strlen(string)) : strlen(string);
	if(bufferLen == 0)
	{
		return NULL;
	}

	//fill out sanitized string
	for(int i=0; i<bufferLen; i++)
	{
		char c = string[i];
		buffer[i] = _details::isOkChar(c, validation) ? c :                                            //no substitute
			((c == '\t' && _details::betterUseASpaceInsteadOfTab(validation)) ? ' ' : SAFE_CHAR);   //substiture safe char for a space or a '_'

		//variable name validation is assuming we are remoting to some terminal session and creating a new variable;
		//many languages dont allow a number to start a variable name
		if((i == 0) && (validation & StringValidationRules::VariableName) && (!isAlpha(c)))
		{
			buffer[i] = SAFE_CHAR;
		}
	}

	buffer[(bufferLen == maxLen) ? (bufferLen-1) : bufferLen] = 0;

	return buffer;
}

//--------------------------------------------------------------------------------------------------------------------------------------
// Sometimes is makes sense to replace a tab with a space, sometimes not.
// This method encapsulates that logic.
bool _details::betterUseASpaceInsteadOfTab(StringValidationRules validation)
{
	return (validation & StringValidationRules::NoTabs) && 
		(!(  (validation & StringValidationRules::NoWhiteSpace) || (validation & StringValidationRules::VariableName) ));
}

//--------------------------------------------------------------------------------------------------------------------------------------
bool _details::isOkChar(char c, StringValidationRules validation)
{
	//get this out the way
	if(validation == StringValidationRules::None)
	{
		return true;
	}

	//stuf that is ok regardless
	if(isAlphaNumeric(c) || (c == '_'))
	{
		return true;
	}

	//if VariableName rules are in effect, we have now failed
	if(validation & StringValidationRules::VariableName)
	{
		return false;
	}

	//now apply any white space tests
	if(isWhitespace(c))
	{
		if(validation & StringValidationRules::NoWhiteSpace)
		{
			return false;
		}
		
		if(c == '\t' && (validation & StringValidationRules::NoTabs))
		{
			return false;
		}

		//done: NB: StringValidationRules::VariableName paths don't get this far.
		return true;
	}

	//if quotes are not allowed by the validation rules, throw error now
	if((validation & StringValidationRules::NoSingleQuotes) && (c == '\''))
	{
		return false;
	}

	if((validation & StringValidationRules::NoDoubleQuotes) && (c == '\"'))
	{
		return false;
	}

	if((validation & StringValidationRules::NoNewLines) && ((c == '\r') || (c == '\n')))
	{
		return false;
	}
	
	// Sort final tests here.
	// ie: FileNameNoExt, FileNameFull, Path, NoNewLines
	switch(c) //I just think elsesitch sounds cool
	{
		case '\'':

		//stuff file names can always have
		case '{': case '}': case '(': case ')': case '[': case ']': 		
		//other misc chars, file/path safe, but not variable safe
		case '!': case '@': case '#': case '$': case '%': case '^': case '&': case '~': case '`': case ',': 
		case '+':  case '-': 
			return true;

	    //file system special chars, definaty not in any filename, but ok for a path.
		case '?': case '*': case '"': case '>': case '<': case '|': case '/%': case ':': case '/': case '\\':
			return !((validation & StringValidationRules::FileNameFull) || (validation & StringValidationRules::FileNameNoExt) );

		//things a FileNameFull, or path can have, but a FileNameNoExt can't
		case '.': return !(validation & StringValidationRules::FileNameNoExt);

		case '\r': case '\n': 
			return ! ((validation & StringValidationRules::FileNameNoExt) || 
				      (validation & StringValidationRules::FileNameFull)  || 
				      (validation & StringValidationRules::Path)          || 
				      (validation & StringValidationRules::NoNewLines)       );
	}

	//anything else is probably a risky prospect on a com port
	return  !(validation & StringValidationRules::ComPortSafe);
}

//--------------------------------------------------------------------------------------------------------------------------------------
char *trimString(char* string, char *buffer, int maxLen)
{
	// I am writing this in a plain form, non-optimal.
	// I expect the compiler to make certian optimisations.
	
	//basic tests
	if((string == NULL) || (buffer == NULL))
	{
		return NULL;
	}
	if(maxLen == 1) //only room to null terminate
	{
		buffer[0] = 0;
		return buffer;
	}
	if(maxLen == 0) 
	{
		return NULL;
	}

	char *src = string;
	char *dest = buffer;

	//advance to first non space character
	while(isWhitespace(*src))
	{
		src++;
	}
	
	//find end of string
	int len = 0;
	while(src[len] != 0)
	{
		len++;
		if((maxLen >= 0) && (len >= (maxLen-1)))
		{
			break;
		}
	}
	
	//was it all spaces, or a 0 maxLen
	if(len == 0)
	{
		buffer[0] = 0;
		return buffer;
	}
	
	//shorten the len, to remove trailing whitespace
	while(isWhitespace(src[len-1]))
	{
		len--;
	}
	
	//len must now be > 0 and <= maxLen
	for(int i=0; i<len; i++)
	{
		buffer[i] = src[i];
	}
	
	//null terminate
	buffer[len] = 0;
}


