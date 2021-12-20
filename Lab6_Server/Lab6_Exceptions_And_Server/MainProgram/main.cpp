#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#include <stdio.h>
#include <iostream> 
#include "..\Functionality\pluginsManager.h"
#include <windows.h>
#include <string>
#pragma comment (lib, "..\\Debug\\Functionality.lib")
#pragma comment(lib, "WS2_32.lib")
using namespace std;

DWORD WINAPI clientSend(LPVOID lpParam, char* message) 
{ 
	char buffer[1024] = { 0 };
	strcpy_s(buffer, 1024, message);
	SOCKET server = *(SOCKET*)lpParam;
	if (send(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
	{
		cout << "send failed with error: " << WSAGetLastError() << endl;
		return -1;
	}
	return 1;
}

int main()
{
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
		return -1;
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //коннект к серверу
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555); //порт
	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "Server connection failed with error: " << WSAGetLastError() << endl;
		return -1;
	}

	cout << "Connected to server!" << endl;

	try
	{
		setlocale(LC_ALL, "Russian");
		printf("Choose the language\n");
		printf("%s\n", "\"1\", русский");
		printf("%s\n", "\"2\" english");

		bool language = true;
		int tmpLanguage = -1;
		char tmpLanguageChar[200];
		scanf_s("%s", tmpLanguageChar, 200);
		try
		{
			tmpLanguage = stoi(tmpLanguageChar);
		}
		catch (...)
		{
			char message[200] = "Error! Incorrect choice of language. User entered ";
			strcat_s(message, 200, tmpLanguageChar);
			strcat_s(message, 200, "\n");
			clientSend(&server, message);
		}

		if (tmpLanguage != 1 && tmpLanguage != 2)
		{
			printf("%s\n", "Incorrect choice.");
			printf("%s\n\n", "English language is set default.");
			tmpLanguage = 2;
		}

		if (tmpLanguage == 1)
		{
			language = false;
		}
		else
		{
			language = true;
		}


		bool menu = true;
		int choice;
		while (menu)
		{
			if (!language)
			{
				printf("%s\n", "Меню:");
				printf("%s\n", "1 - Загрузить плагины");
				printf("%s\n", "2 - Выгрузить плагины");
				printf("%s\n", "3 - Показать на экране список плагинов");
				printf("%s\n", "4 - Показать информацию о загруженных плагинах");
				printf("%s\n", "5 - Выполнить плагины");
				printf("%s\n", "6 - Очистить консоль");
				printf("%s\n", "7 - Выход");
			}
			else
			{
				printf("%s\n", "Menu:");
				printf("%s\n", "1 - Load plugins");
				printf("%s\n", "2 - Unload plugins");
				printf("%s\n", "3 - Show list of plugins");
				printf("%s\n", "4 - Show info about loaded plugins");
				printf("%s\n", "5 - Execute plugins");
				printf("%s\n", "6 - Clean console");
				printf("%s\n", "7 - Exit");
			}

			if (!language)
			{
				printf("%s", "Ваш выбор: ");
			}
			else
			{
				printf("%s", "Your choice: ");
			}

			char choiceStr[200];
			cin >> choiceStr;

			try
			{
				choice = stoi(choiceStr);
			}
			catch (...)
			{
				cout << "Incorrect choice\n\n";
				char message[200] = "Error! Incorrect choice of menu. User entered ";
				strcat_s(message, 200, choiceStr);
				strcat_s(message, 200, "\n");
				clientSend(&server, message);
				continue;
			}

			char exitChar[100] = "exit\n";

			switch (choice)
			{
			case 1:
				LoadPlugins(language);
				break;
			case 2:
				UnloadPlugins(language);
				break;
			case 3:
				ShowListOfPlugins(language);
				break;
			case 4:
				ShowInfo(language);
				break;
			case 5:
				RunPlugins(language);
				break;
			case 6:
				system("cls");
				break;
			case 7:
				Exit(language);
				clientSend(&server, exitChar);
				menu = false;
				break;
			default:
				if (!language)
				{
					printf("%s\n\n", "Некорректный выбор.");
				}
				else
				{
					printf("%s\n\n", "Incorrect choice.");
				}
				break;
			}
		}
	}
	catch (...)
	{
		char message[200] = "Unknown error!";
		strcat_s(message, 200, "\n");
		clientSend(&server, message);
	}

	closesocket(server);
	WSACleanup();

	system("pause>>NUL");
	return 0;
}