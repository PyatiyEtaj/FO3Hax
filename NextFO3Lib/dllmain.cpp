#include <Windows.h>

#include "framework.hpp"
#include "Util.hpp"
#include "GlobalState.hpp"
#include "Fo3Functions.hpp"
#include "Features.hpp"

GlobalState::Settings Settings;
GlobalState::KeyBoard Keyboard;
GlobalState::GlobalObjects GObjects;

FO3::Fo3Functions Fo3Functions;

DWORD CALLBACK MainThread(LPVOID _)
{
	Settings.IsConsoleActivate = false;
	if (Settings.IsConsoleActivate)
	{
		Settings.Console->Create();
		std::cout << "--[ HELLO ]--" << std::endl;
	}

	Features::Activate();

	while (true)
	{
		Keyboard.Shift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
		Keyboard.LeftControl = GetAsyncKeyState(VK_CONTROL) & 0x8000;
		Keyboard.Space = GetAsyncKeyState(VK_SPACE) & 0x8000;

		if (Keyboard.LeftControl && Keyboard.Space)
		{
			Fo3Functions.CallLastAttackAction(GObjects);
		}

		if (GetAsyncKeyState(VK_F4) != 0)
		{
			Beep(200, 200);
			Settings.Console->Free();
			Fo3Functions.RestoreUIDFlag();
			break;
		}
		Sleep(200);
	}
	ExitThread(0);
}

// INJECTION
LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || Settings.IsInjected)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	Settings.IsInjected = true;
	if (GetModuleHandleA(FO3_NAME))
	{
		CreateThread(0, 0, MainThread, 0, 0, 0);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	DisableThreadLibraryCalls(hModule);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

