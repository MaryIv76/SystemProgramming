#include <windows.h>
#include <stdio.h>

int main()
{
	LPCTSTR pipename = L"\\\\.\\pipe\\mynamedpipe";
	HANDLE hNamedPipe;
	DWORD szAddress;
	DWORD cbRead, cbWritten;
	char* message;
	BOOL success;


	printf("Creating named pipe   ");
	hNamedPipe = CreateNamedPipe(
		pipename,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE |
		PIPE_READMODE_MESSAGE |
		PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		sizeof(DWORD),
		sizeof(DWORD),
		100,
		NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		printf("Fail!\n");
		system("pause");
		return -1;
	}

	printf("Success!\nConnecting to pipe   ");
	ConnectNamedPipe(hNamedPipe, NULL);

	printf("Success!\nReading from pipe   ");
	success = ReadFile(hNamedPipe, &szAddress, sizeof(szAddress), &cbRead, NULL);
	if (success == FALSE)
	{
		printf("Fail!\n");
		system("pause");
		return -1;
	}

	printf("Success!\n\nYour string: ");
	message = (char*)szAddress;
	printf_s("%s\n", message);

	printf("Sending answer to ProccessA   ");
	success = WriteFile(hNamedPipe, &szAddress, sizeof(szAddress), &cbWritten, NULL);
	if (success == FALSE)
	{
		printf("Fail!\n");
		system("pause");
		return -1;
	}

	printf("Success!\nDisconnecting from named pipe   ");
	success = DisconnectNamedPipe(hNamedPipe);
	if (success == FALSE)
	{
		printf("Fail!\n");
		system("pause");
		return -1;
	}

	printf("Success!\nClosing named pipe   ");
	success = CloseHandle(hNamedPipe);
	if (success == FALSE)
	{
		printf("Fail!\n");
		system("pause");
		return -1;
	}

	printf("Success!\nProcessB was finished   \n");
	system("pause");
	return 0;
}