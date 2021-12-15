#include <stdio.h>
#include <iostream> 
#include <windows.h>
#include "..\Functionality\pluginsManager.h"

#pragma comment (lib, "..\\Debug\\Functionality.lib")

int main()
{
	setlocale(LC_ALL, "Russian");
	printf("Choose the language\n");
	printf("%s\n", "\"0\", �������");
	printf("%s\n", "\"1\" english");

	bool language = true;
	int tmpLanguage = -1;
	scanf_s("%i", &tmpLanguage);
	if (tmpLanguage != 1 && tmpLanguage != 0)
	{
		printf("%s\n\n", "Incorrect choice.");
		return 0;
	}
	language = tmpLanguage;

	bool menu = true;
	int choice;
	while (menu)
	{
		if (!language)
		{
			printf("%s\n", "����:");
			printf("%s\n", "1 - ��������� �������");
			printf("%s\n", "2 - ��������� �������");
			printf("%s\n", "3 - �������� �� ������ ������ ��������");
			printf("%s\n", "4 - �������� ���������� � ����������� ��������");
			printf("%s\n", "5 - ��������� �������");
			printf("%s\n", "6 - �������� �������");
			printf("%s\n", "7 - �����");
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
			printf("%s", "��� �����: ");
		}
		else
		{
			printf("%s", "Your choice: ");
		}
		scanf_s("%d", &choice);

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
			menu = false;
			break;
		default:
			if (!language)
			{
				printf("%s\n\n", "������������ �����.");
			}
			else
			{
				printf("%s\n\n", "Incorrect choice.");
			}
			break;
		}
	}

	system("pause>>NUL");
	return 0;
}