#include <Windows.h>

#include "framework.hpp"
#include "Util.hpp"
#include "GlobalState.hpp"
#include "Fo3Functions.hpp"
#include "Features.hpp"
#include "Resources.hpp"

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
	}

	Resources::Notification();

	Features::Activate();

	while (true)
	{
		Keyboard.Shift = GetKeyState(VK_SHIFT);
		Keyboard.LeftControl = GetKeyState(VK_CONTROL);
		Keyboard.Space = GetKeyState(VK_SPACE);

		if (Keyboard.LeftControl.IsDown() && Keyboard.Space.IsDown())
		{
			Fo3Functions.CallLastAttackAction(&GObjects);
		}

		if (GetAsyncKeyState(VK_F4) != 0)
		{
			Resources::Notification();
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
		GObjects.Dll = GetModuleHandleA("NextFO3Lib.dll");
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

