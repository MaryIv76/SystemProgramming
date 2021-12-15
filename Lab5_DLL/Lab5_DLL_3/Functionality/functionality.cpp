#include "pch.h"
#include ".\pluginsManager.h"

void LoadPlugins(bool lang)
{
	pluginsCount = 0;
	WIN32_FIND_DATA file;
	HANDLE hFile = FindFirstFile(L"Plugins\\*.dll", &file);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (pluginsCount == MAXPLUGINSALLOWED)
		{
			break;
		}
		wchar_t buf[256] = L"Plugins\\";
		wcscat_s(buf, 256, file.cFileName);
		Plugins[pluginsCount] = LoadLibrary((LPCWSTR)buf);
		wcscpy_s(pluginsNames[pluginsCount], file.cFileName);
		++pluginsCount;
	} while (FindNextFile(hFile, &file));
	FindClose(hFile);

	if (!lang)
	{
		printf("Загружено плагинов: %d \n\n", pluginsCount);
	}
	else
	{
		printf("Plugins loaded: %d \n\n", pluginsCount);
	}
}

bool WriteIfPluginsNotLoaded(bool lang)
{
	if (pluginsCount == 0)
	{
		if (!lang)
		{
			printf("%s\n\n", "Нет загруженных плагинов");
		}
		else
		{
			printf("%s\n\n", "There're no loaded plugins");
		}
		return true;
	}

	return false;
}

void WritePluginsAreUnloaded(bool lang)
{
	if (!lang)
	{
		printf("%s\n\n", "Все плагины выгружены");
	}
	else
	{
		printf("%s\n\n", "All plugins are unloaded.");
	}
}

void UnloadPlugins(bool lang)
{
	if (WriteIfPluginsNotLoaded(lang))
	{
		return;
	}

	do
	{
		FreeLibrary(Plugins[pluginsCount - 1]);
		Plugins[pluginsCount - 1] = NULL;
		--pluginsCount;
	} while (pluginsCount != 0);

	WritePluginsAreUnloaded(lang);
}

void ShowListOfPlugins(bool lang)
{
	if (WriteIfPluginsNotLoaded(lang))
	{
		return;
	}

	printf("\n");
	for (int i = 0; i < pluginsCount; ++i)
	{
		wchar_t buff[100];
		printf("%d. ", i + 1);
		swprintf(buff, 100, L"%s\n", pluginsNames[i]);
		fputws(buff, stdout);
	}
	printf("\n");
}

void ShowInfo(bool lang)
{
	if (WriteIfPluginsNotLoaded(lang))
	{
		return;
	}

	DWORD bufdword = 0;
	char buffer[512];
	PluginInfo plugInfo;
	for (int i = 0; i < pluginsCount; ++i)
	{
		plugInfo = (PluginInfo)GetProcAddress(Plugins[i], "GetDescription");
		plugInfo(buffer, 512, &bufdword);

		wchar_t buff[100];

		if (!lang)
		{
			swprintf(buff, 100, L"Описание плагина %s: ", pluginsNames[i]);
			fputws(buff, stdout);
			printf("%s\n", buffer);
		}
		else
		{
			swprintf(buff, 100, L"Plugin %s description: ", pluginsNames[i]);
			fputws(buff, stdout);
			printf("%s\n", buffer);
		}

		plugInfo = (PluginInfo)GetProcAddress(Plugins[i], "GetAuthor");
		plugInfo(buffer, 512, &bufdword);
		if (!lang)
		{
			printf("Автор: %s\n\n", buffer);
		}
		else
		{
			printf("Author: %s\n\n", buffer);
		}
	}
}

void RunPlugins(bool lang)
{
	if (WriteIfPluginsNotLoaded(lang))
	{
		return;
	}

	PluginExecute plugExecute;
	for (int i = 0; i < pluginsCount; ++i)
	{
		plugExecute = (PluginExecute)GetProcAddress(Plugins[i], "Execute");
		plugExecute();
	}
}


void Exit(bool lang)
{
	if (pluginsCount != 0)
	{
		do
		{
			FreeLibrary(Plugins[pluginsCount - 1]);
			Plugins[pluginsCount - 1] = NULL;
			--pluginsCount;
		} while (pluginsCount != 0);
		
		WritePluginsAreUnloaded(lang);
	}

	if (!lang)
	{
		printf("%s", "Программа завершена...");
	}
	else
	{
		printf("%s", "Program is finished...");
	}
}