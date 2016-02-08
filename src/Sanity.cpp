
#include "Sanity.h"

char *mallocCopyOf(const __FlashStringHelper* text)
{
	if(text == NULL)
	{
		return NULL;
	}

	const char PROGMEM *p = (const char PROGMEM *)text;
	char *buffer = (char*)malloc(strlen_P(p)+1);
	buffer[0] = 0;
	strcpy_P(buffer, p); //if any issues, check return value == buffer
	return buffer;
}

char *strcpy(char *dest, const __FlashStringHelper* src)
{
	if(src == NULL)
	{
		return NULL;
	}

	const char PROGMEM *p = (const char PROGMEM *)src;
	return strcpy_P(dest, p);
}

size_t strlcpy(char *dest, const __FlashStringHelper* src, size_t len)
{
	if(src == NULL)
	{
		return NULL;
	}

	const char PROGMEM *p = (const char PROGMEM *)src;
	return strlcpy_P(dest, p, len);
}

int strcmp(const char *a, const __FlashStringHelper* b)
{
	const char PROGMEM *p = (const char PROGMEM *)b;
	return strcmp_P(a, p);
}