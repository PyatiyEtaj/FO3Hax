#include <iostream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <random>
#include <sstream>

#define WND_NAME "FOnline"
HHOOK hhk = nullptr;

inline HHOOK SetWndHook(int hookType, HMODULE h, LPCSTR funcName, DWORD thread)
{
	return SetWindowsHookExA(hookType, (HOOKPROC)GetProcAddress(h, funcName), h, thread);
}

HHOOK InstallHook(HMODULE pDll)
{
	HHOOK hhook = SetWndHook(5, pDll, "HookProc", 0);
	if (!hhook || GetLastError() != 0)
	{
		std::string s = "something wrong --> " + GetLastError();
		std::cout << s << std::endl;
		return NULL;
	}
	std::cout << "HOOK " << hhook << " have setted" << std::endl;

	return hhook;
}

void Injection()
{
	HMODULE pDll = LoadLibraryA("NextFO3Lib.dll");
	hhk = InstallHook(pDll);
	if (hhk)
	{
		system(("pause"));
		std::cout << ("HOOK ") << hhk << (" released") << std::endl;
		UnhookWindowsHookEx(hhk);
		hhk = nullptr;
	}
	else
		std::cout << ("HOOK hasn't been setted - ") << GetLastError() << std::endl;
}

int main()
{
	int i = 0;;
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			if (!hhk) {
				Injection();
				break;
			}
		}
		Sleep(200);
	}
}