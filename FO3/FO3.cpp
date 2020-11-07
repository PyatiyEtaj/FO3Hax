#include <iostream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <random>
#include "MyXorStr.h"
#include <sstream>

#define WND_NAME xorstr("FOnline")
HHOOK hhk = nullptr;

typedef HHOOK (__stdcall *ptrSetWindowsHookExA)  (int idHook,HOOKPROC lpfn,HINSTANCE hmod, DWORD dwThreadId);
typedef BOOL  (__stdcall *ptrUnhookWindowsHookEx)(HHOOK hhk);

ptrSetWindowsHookExA fSetWindowsHookExA = (ptrSetWindowsHookExA)GetProcAddress(GetModuleHandleA(xorstr("User32.dll")), xorstr("SetWindowsHookExA"));
ptrUnhookWindowsHookEx fUnhookWindowsHookEx = (ptrUnhookWindowsHookEx)GetProcAddress(GetModuleHandleA(xorstr("User32.dll")), xorstr("UnhookWindowsHookEx"));


inline HHOOK SetWndHook(int hookType, HMODULE h, LPCSTR funcName, DWORD thread)
{
	return fSetWindowsHookExA(hookType, (HOOKPROC)GetProcAddress(h, funcName), h, thread);
}


HHOOK InstallHook(HMODULE pDll)
{
	HHOOK hhook = SetWndHook(5, pDll, xorstr("HookProc"), 0);
	if (!hhook || GetLastError() != 0)
	{
		std::string s = xorstr("something wrong --> ") + GetLastError();
		std::cout << s << std::endl;
		return NULL;
	}
	std::cout << xorstr("HOOK ") << hhook << xorstr(" have setted") << std::endl;

	return hhook;
}

void Injection()
{
	HMODULE pDll = LoadLibraryA(xorstr("FO3Dll.dll"));
	hhk = InstallHook(pDll);
	if (hhk)
	{
		system(xorstr("pause"));
		std::cout << xorstr("HOOK ") << hhk << xorstr(" released") << std::endl;
		fUnhookWindowsHookEx(hhk);
		hhk = nullptr;
	}
	else
		std::cout << xorstr("HOOK hasn't been setted - ") << GetLastError() << std::endl;
}

int main()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			if (!hhk) {
				Injection();
				break;
			}
		}
		Sleep(100);
	}
}