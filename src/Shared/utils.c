#include "utils.h"

void strReplaceChar(char* string, char toFind, char toReplace, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (string[i] == toFind) string[i] = toReplace;
	}
}