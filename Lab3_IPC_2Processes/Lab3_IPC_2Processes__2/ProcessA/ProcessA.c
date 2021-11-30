#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#define MAX_LENGTH 101

DWORD GetProcessID(wchar_t* process)
{
	PROCESSENTRY32 processEntry;
	DWORD processID = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		processEntry.dwSize = sizeof(processEntry);
		Process32First(snapshot, &processEntry);

		do
		{
			if (wcscmp(processEntry.szExeFile, process) == 0)
			{
				processID = processEntry.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &processEntry));

		CloseHandle(snapshot);
	}

	return processID;
}

int Fail()
{
	printf("FAIL!\n");
	system("pause");
	return -1;
}

void main(void)
{
	LPCTSTR pipeName = L"\\\\.\\pipe\\mynamedpipe";
	HANDLE hProcessB;
	LPVOID szAddress;
	DWORD cbWritten, cbRead, dBuf;
	BOOL success;
	DWORD szSize = sizeof(char) * MAX_LENGTH;
	char* str = (char*)malloc(szSize);
	HANDLE hPipe = INVALID_HANDLE_VALUE;

	printf("INPUT STRING: ");
	fgets(str, MAX_LENGTH - 1, stdin);

	printf("Opening ProcessB...");
	hProcessB = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessID(L"ProcessB.exe"));
	if (hProcessB == INVALID_HANDLE_VALUE)
		return Fail();

	printf("\nAllocating memory for ProcessB...");
	szAddress = VirtualAllocEx(hProcessB, 0, szSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	printf("\nWriting your string in ProcessB memory...");
	success = WriteProcessMemory(hProcessB, szAddress, str, szSize, 0);
	if (success == FALSE)
		return Fail();

	printf("\nConnecting to named pipe...");
	while (hPipe == INVALID_HANDLE_VALUE)
	{
		hPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	}

	printf("\nWriting memory address...");
	dBuf = (DWORD)szAddress;
	success = WriteFile(hPipe, &dBuf, sizeof(dBuf), &cbWritten, NULL);
	if (success == FALSE)
		return Fail();

	printf("\nWaiting answer from ProcessB...");
	success = ReadFile(hPipe, &szAddress, sizeof(szAddress), &cbRead, NULL);
	if (success == FALSE)
		return Fail();

	printf("\nReleasing ProcessB memory...");
	success = VirtualFreeEx(hProcessB, szAddress, 0, MEM_RELEASE);
	if (success == FALSE)
		return Fail();

	printf("\nClosing named pipe...");
	success = CloseHandle(hPipe);
	if (success == FALSE)
		return Fail();

	printf("\nClosing ProcessB...");
	success = CloseHandle(hProcessB);
	if (success == FALSE)
		return Fail();

	printf("\nProcessA was finished...\n");
	system("pause");
	return 0;
}