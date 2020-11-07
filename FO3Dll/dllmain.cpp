#include "ForDebug.h"
#include "framework.h"
#include "D3D.h"
#include "ScriptsDLL.h"
#include "PacketsAnal.h"
#include "AnalyzeNetBuffer.h"
#include "PacketSender.h"
#include "UserData.h"
#include "PathFinder.h"
#include "Initialization.h"
#include "InGameScripts.h"

DWORD CALLBACK MainThread(LPVOID args)
{
	srand(time(NULL));
	int shift = 1, vk_f5 = 0, vk_end = 0;
	bool terminateOneHexThread = false;
	Initializator initializator;
	initializator.SetInitializator();
	initializator.Await();
	PBYTE foclient = GetFOClient();
	PathFinder* pathFinder = new PathFinder();
	D3D::Constructor(FO3_WND, &pathFinder->NeedMove);
	Scripts::EnableOneHex(&shift);
	HANDLE onehexthread = Scripts::EnablePathFinding(pathFinder, &terminateOneHexThread);

	uint nextTime = 0;
	uint* fotime = (uint*)GET_TIME_PTR;
	InGameScripts::SetHooks(&nextTime);

	while (true)
	{
		shift = GetAsyncKeyState(VK_SHIFT) != 0;
		vk_f5 = GetAsyncKeyState(VK_F5) != 0;
		vk_end = GetAsyncKeyState(VK_END) != 0;
		pathFinder->NeedMove = (GetKeyState(VK_CAPITAL) & 1) == 0;

		if (shift && vk_f5) {
			RefreshMe(foclient);
		}		

		if (shift && vk_end)
		{
			Beep(200, 200);
			terminateOneHexThread = true;
			ResumeThread(onehexthread);
			Sleep(200);
			InGameScripts::UnsetHooks();
			Scripts::UnsetHooks();
			D3D::Destructor();
			delete pathFinder;
			break;
		}

		D3D::SetOutputText(
			nextTime > * fotime 
			? xorstr("heal in ") + std::to_string(nextTime - *fotime) 
			: ""
		);

		Sleep(100);
	}
	ExitThread(0);
}

DWORD CALLBACK DebugThread(LPVOID args)
{
	srand(time(NULL));
	Initializator initializator;
	initializator.SetInitializator();
	initializator.Await();
	PBYTE foclient = GetFOClient();
	PathFinder* pathFinder = new PathFinder();
	int shift = 1, vk_f5 = 0;
	bool terminateOneHexThread = false;
	D3D::Constructor(FO3_WND, &pathFinder->NeedMove);
	Scripts::EnableOneHex( &shift);
	HANDLE onehexthread = Scripts::EnablePathFinding(pathFinder, &terminateOneHexThread);
	bool debugMode = false;
	bool showoutput = false;
	bool showinput = false;
	AnalyzeNetBuffer::InitializationOfAnalyzer(&showinput, &showoutput);
	PacketsAnal::SetInlineHook(&showinput, &showoutput);
	AnalyzeNetBuffer::SetOutputDebugMessages(debugMode);
	uint nextTime = 0;
	uint* fotime = (uint*)GET_TIME_PTR;
	InGameScripts::SetHooks(&nextTime);

	while (true)
	{
		shift = GetAsyncKeyState(VK_SHIFT) != 0;
		vk_f5 = GetAsyncKeyState(VK_F5) != 0;
		pathFinder->NeedMove = (GetKeyState(VK_CAPITAL) & 1) != 0;

		if (shift && GetAsyncKeyState(VK_F5) != 0) {
			RefreshMe(foclient);
		}

		/*if (GetAsyncKeyState(VK_NUMPAD7)) {
			JustSender js;
			js.Send();
		}*/

		if (GetAsyncKeyState(VK_NUMPAD4)) {
			showinput = !showinput;
		}

		if (GetAsyncKeyState(VK_NUMPAD5)) {
			showoutput = !showoutput;
		}

		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			system("cls");
		}

		if (GetAsyncKeyState(VK_F1))
		{
			debugMode = !debugMode;
			AnalyzeNetBuffer::SetOutputDebugMessages(debugMode);
		}

		if (GetAsyncKeyState(VK_END))
		{
			Beep(200, 200);
			Scripts::UnsetHooks();
			terminateOneHexThread = true;
			ResumeThread(onehexthread);
			D3D::Destructor();
			delete pathFinder;
			InGameScripts::UnsetHooks();
			AnalyzeNetBuffer::Unset();
			ForDebug::FreeCons();
			PacketsAnal::Unset();
			break;
		}

		std::string str = (shift ? xorstr("HexMachine Enabled ") : xorstr("HexMachine Disabled"));
		str += xorstr(" [Shift]\n");
		str += (showinput ? xorstr(" ShowInput Enabled ") : xorstr(" ShowInput Disabled"));
		str += xorstr(" [Num4]\n");
		str += (showoutput ? xorstr("ShowOutput Enabled ") : xorstr("ShowOutput Disabled"));
		str += xorstr(" [Num5]\n");
		str += (debugMode ? xorstr(" ShowDebug Enabled ") : xorstr(" ShowDebug Disabled"));
		str += xorstr(" [F1]\n");
		str +=
			nextTime > * fotime
			? xorstr("отхил ") + std::to_string(nextTime - *fotime)
			: "";
		D3D::SetOutputText(debugMode ? "" : str);


		Sleep(100);
	}

	ExitThread(0);
}

bool isActivated = false;

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || isActivated)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	isActivated = true;
	if (GetModuleHandleA(FO3_NAME) || GetModuleHandleA(FO2_NAME))
	{
#ifdef RELEASE
		CreateThread(0, 0, MainThread, 0, 0, 0);
#else
		CreateThread(0, 0, DebugThread, 0, 0, 0);
#endif
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
#ifndef RELEASE
		if (GetModuleHandleA(FO3_NAME) || GetModuleHandleA(FO2_NAME))
		{
			ForDebug::CrtConsole();
			Beep(200, 200);
		}
#endif
		break;
    case DLL_PROCESS_DETACH:
#ifndef RELEASE
		ForDebug::FreeCons();
#endif
        break;
    }
    return TRUE;
}