#include "pch.h"
#include "dll.h"
#include <stdio.h>
#include <stdlib.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 100, "Written by Mariya Ivanova");
	*pdwBytesWritten = result;
	return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 100, "Plagin shows system info");
	*pdwBytesWritten = result;
	return result;
}

VOID Execute()
{
	system("msinfo32");
	printf("\n");
}