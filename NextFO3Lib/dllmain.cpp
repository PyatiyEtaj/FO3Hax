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
FO3::Functions Fo3Functions;

DWORD CALLBACK MainThread(LPVOID _)
{
	Settings.IsConsoleActivate = false;

	if (Settings.IsConsoleActivate)
	{
		Settings.Console->Create();
	}

	GObjects.Init();

	Resources::Notification();

	Features::Activate();

	while (true)
	{
		/////////////////////////////////////////////////////////////
		Keyboard.Shift = GetKeyState(VK_SHIFT);
		Keyboard.LeftControl = GetKeyState(VK_CONTROL);
		Keyboard.Space = GetKeyState(VK_SPACE);
		Keyboard.Delete = GetKeyState(VK_DELETE);
		Keyboard.MouseL = GetKeyState(VK_LBUTTON);
		Keyboard.MouseR = GetKeyState(VK_RBUTTON);

		Settings.IsOff =
			Keyboard.LeftControl.IsDown() &&
			Keyboard.Delete.IsDown();
		/////////////////////////////////////////////////////////////

		if (Keyboard.LeftControl.IsDown() && Keyboard.MouseL.IsDown() && Keyboard.MouseR.IsUp())
		{
			Fo3Functions.AutoClick(&GObjects);
		}

		if (Keyboard.LeftControl.IsDown() && Keyboard.Space.IsDown())
		{
			Fo3Functions.CallLastAttackAction(&GObjects);
			Sleep(125);
		}

		if (Settings.IsOff)
		{
			Resources::Notification();
			Settings.Console->Free();
			Fo3Functions.RestoreUIDFlag();
			break;
		}
		Sleep(25);
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
		auto mainThread = CreateThread(0, 0, MainThread, 0, 0, 0);
		if (mainThread)
		{
			CloseHandle(mainThread);
		}
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

