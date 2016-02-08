// StringValidation.h

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

#ifndef _STRINGVALIDATION_h
#define _STRINGVALIDATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Sanity.h"

/**
 * Flags for validation rules.
 *   eg: (FileNameFull | NoWhiteSpace | ComPortSafe) creates validation for a filename (without spaces), that is safe for com port style text transmition.
 */
enum StringValidationRules {None = 0, ComPortSafe = 1, VariableName = 2, FileNameNoExt = 4, 
							FileNameFull = 8,  NoTabs = 16, NoWhiteSpace = 32, Path = 64, 
							NoSingleQuotes=128, NoDoubleQuotes=256, NoNewLines = 512,
							
							NoQuotes=NoSingleQuotes|NoDoubleQuotes};


/**
 * Sanatises a string.
 * @param string        Text to validate.
 * @param buffer		Destination buffer.
 * @param validation    Flags indicating type of validation needed.
 * @param maxLen        A maximumlength for the final string, -1 to disable
 * @return buffer on success. NULL on error.
 *
 * @note  The buffer will never hold more characters than the original string.
 * @note string and buffer can be the same, to make the function work in place.
 */
char *sanitizeString(char* string, char *buffer, StringValidationRules validation, int maxLen);

/** @overload sanitizeString(char*, char*, StringValidationRules, int) */
inline char *sanitizeString(char*string, char *buffer, int validation, int maxLen)
{
	return sanitizeString(string, buffer, (StringValidationRules) validation, maxLen);
}

/** @overload sanitizeString(char*, char*, StringValidationRules, int) */
inline char *sanitizeString(char *buffer, StringValidationRules validation, int maxLen)
{
	return sanitizeString(buffer, buffer, validation, maxLen);
}

/** @overload sanitizeString(char*, char*, StringValidationRules, int) */
inline char *sanitizeString(char *buffer, int validation, int maxLen)
{
	return sanitizeString(buffer, buffer, (StringValidationRules) validation, maxLen);
}

/**
* Sanatises a string.
* @param string       Text to validate.
* @param validation   Flags indicating type of validation needed.
* @param maxLen       A maximumlength for the final string, -1 to disable
* @return Pointer to dynamicly allocated memory. NULL on error.
*
* @note  The buffer will never hold more characters than the original string.
*/
char *mallocSanitizedCopy(const char*string, StringValidationRules validation, int maxLen);

/** @overload mallocSanitizedCopy(const char*, StringValidationRules, int) */
inline char *mallocSanitizedCopy(const __FlashStringHelper* text, StringValidationRules validation, int maxLen)
{
	char *buffer = mallocCopyOf(text);
	char *sanitized = mallocSanitizedCopy(buffer, validation, maxLen);
	free(buffer);
	return sanitized;
}

/** @overload mallocSanitizedCopy(const char*, StringValidationRules, int) */
inline char *mallocSanitizedCopy(const char*string, int validation, int maxLen)
{
	return mallocSanitizedCopy(string, (StringValidationRules) validation, maxLen);
}

/** @overload mallocSanitizedCopy(const char*, StringValidationRules, int) */
inline char *mallocSanitizedCopy(const __FlashStringHelper* text, int validation, int maxLen)
{
	return mallocSanitizedCopy(text, (StringValidationRules) validation, maxLen);
}

/**
 * Trims whitespace around a string.
 * @param string        Text to validate.
 * @param buffer		Destination buffer.
 * @param validation    Flags indicating type of validation needed.
 * @param maxLen        A maximumlength for the final string, -1 to disable
 * @return buffer on success. NULL on error.
 *
 * @note string or buffer being NULL causes no action, and NULL to be returned.
 * @note string and buffer can be the same, to make the function work in place.
 */
char *trimString(char* string, char *buffer, int maxLen);

/** @overload trimString(const char*, char *, StringValidationRules, int) */
inline char *trimString(char *buffer, int maxLen)
{
	return trimString(buffer, buffer, maxLen);
}

namespace _details
{
	// Sometimes is makes sense to replace a tab with a space, sometimes not.
	// This method encapsulates that logic.
	bool betterUseASpaceInsteadOfTab(StringValidationRules validation);

	/**
	* Return true if a chracter is ok by the given validation rules.
	*	@param c: the character to validate
	*	@param validation: type of validation to apply.
	*/
	bool isOkChar(char c, StringValidationRules validation);
};


#endif

