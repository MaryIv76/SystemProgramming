#include "pch.h"
#include "dll.h"
#include <stdio.h>

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
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	printf("Hardware information: \n");
	printf("\tOEM ID: %u\n", systemInfo.dwOemId);
	printf("\tNumber of processors: %u\n", systemInfo.dwNumberOfProcessors);
	printf("\tPage size: %u\n", systemInfo.dwPageSize);
	printf("\tProcessor type: %u\n", systemInfo.dwProcessorType);
	printf("\tMinimum application address: %lx\n", systemInfo.lpMinimumApplicationAddress);
	printf("\tMaximum application address: %lx\n", systemInfo.lpMaximumApplicationAddress);
	printf("\tActive processor mask: %u\n", systemInfo.dwActiveProcessorMask);
}