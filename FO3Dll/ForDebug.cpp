#include "ForDebug.h"

bool ConsoleIsAllocated = false;

HMODULE ForDebug::GetModulesNames(std::string moduleName)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if (EnumProcessModules(handle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			CHAR szModName[MAX_PATH];
			if (GetModuleFileNameExA(handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				printf_s("%s | %s\n", szModName, moduleName.c_str());
				if (_stricmp(szModName, moduleName.c_str()) == 0)
				{
					CloseHandle(handle);
					return hMods[i];
				}
			}
		}
	}
	return nullptr;
}
bool ForDebug::CrtConsole()
{
	bool check = AllocConsole();
	if (check) // новая консоль
	{
		char wnd_title[256];
		HWND hwnd = GetForegroundWindow(); // get handle of currently active window
		GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));

		FILE* stream;
		freopen_s(&stream, "CONOUT$", "w", stdout);
		//freopen("CONOUT$", "w", stdout); // перенаправление вывода
		system("chcp 1251");
		printf_s("Console of %s has opened\n=======================================================\n", wnd_title);
	}
	ConsoleIsAllocated = check;
	return check;
}
void ForDebug::FreeCons()
{
	//TerminateProcess(GetConsoleWindow(),0);
	if (ConsoleIsAllocated) FreeConsole();
	ConsoleIsAllocated = FALSE;
}

void ForDebug::PrinterBYTE(PBYTE b, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf_s("%X ", b[i]);
	}
	printf_s("\n");
}

