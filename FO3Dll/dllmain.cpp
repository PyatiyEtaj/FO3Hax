#include "ForDebug.h"
#include "framework.h"
#include "D3D.h"
#include "ScriptsDLL.h"
#include "PacketsAnal.h"
#include "AnalyzeNetBuffer.h"
#include "PacketSender.h"
#include "PathFinder.h"
#include "Initialization.h"
#include "InGameScripts.h"
#include "Packets.h"
#include "Logger.h"
#include <FO3/FO3Dll/Stats.h>
/*#include <CommCtrl.h>
#pragma comment(lib, "Comctl32")*/

HaxSettings g_HaxSettings;
WNDPROC oWndProc;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (g_HaxSettings.IsImGuiInit && (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) || g_HaxSettings.DisableWndProc))
		return true;
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

DWORD CALLBACK MainThread(LPVOID args)
{
	srand(time(NULL));
	int shift = 1, vk_f5 = 0, vk_end = 0, vk_f1;
	bool terminateOneHexThread = false;
	Initializator initializator;
	initializator.SetInitializator();
	initializator.Await();

	PBYTE foclient = GetFOClient();
	Scripts::EnableOneHex(&shift);
	HANDLE onehexthread = Scripts::EnablePathFinding(&terminateOneHexThread);
	uint nextTime = 0;
	uint* fotime = (uint*)GET_TIME_PTR;
	InGameScripts::SetHooks(&nextTime);

	oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, FO3_WND), GWL_WNDPROC, (LONG_PTR)&WndProc);
	D3D::Constructor(FO3_WND);
#ifndef RELEASE
	AnalyzeNetBuffer::InitializationOfAnalyzer();
	PacketsAnal::SetInlineHook();
#endif
	Stats stats;
	stats.Init(foclient);
	uchar** chosen = GET_CHOSEN(foclient);
	Logger::Add("chosen = %p\n", chosen);
	Logger::Add("chosen = %p\n", *chosen);


	while (true)
	{
		shift = GetAsyncKeyState(VK_SHIFT) != 0;
		vk_f5 = GetAsyncKeyState(VK_F5) != 0;
		vk_f1 = GetAsyncKeyState(VK_F1) != 0;
		vk_end = GetAsyncKeyState(VK_END) != 0;

		if ((GetKeyState(VK_INSERT) & 0x1) == g_HaxSettings.IsMenuOpen) {
			g_HaxSettings.IsMenuOpen = !g_HaxSettings.IsMenuOpen;
			g_HaxSettings.DisableWndProc = g_HaxSettings.IsMenuOpen;
		}

		if (g_HaxSettings.NeedRefresh) {
			g_HaxSettings.NeedRefresh = false;
			RefreshMe(foclient);
		}		

		if (g_HaxSettings.Uninject)
		{
			Beep(200, 200);
			terminateOneHexThread = true;
			ResumeThread(onehexthread);
			Sleep(200);
#ifndef RELEASE
			PacketsAnal::Unset();
			AnalyzeNetBuffer::Unset();
#endif
			InGameScripts::UnsetHooks();
			Scripts::UnsetHooks();
			oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, FO3_WND), GWL_WNDPROC, (LONG_PTR)oWndProc);
			D3D::Destructor();
#ifdef CONSOLE
			ForDebug::FreeCons();
#endif
			break;
		}
		std::string healin = g_HaxSettings.ShowHealRateCD && nextTime > *fotime
			? xorstr("heal in ") + std::to_string(nextTime - *fotime)
			: "";
		std::string statistics = "";
		if (stats.IsInit) {
			statistics += "\nAP = " + std::to_string(stats.GetParamAp()) + "/" + std::to_string(stats.GetParamMaxAp());
			statistics += "\nHp = " + std::to_string(stats.GetParamHp()) + "/" + std::to_string(stats.GetParamMaxHp());
			statistics += "\nMaxCrit = " + std::to_string(stats.GetParamMaxCritical());
		}
		healin += statistics;
		D3D::SetOutputText(healin);

		Sleep(g_HaxSettings.ThreadLatency);
	}
	ExitThread(0);
}

/*DWORD CALLBACK DebugThread(LPVOID args)
{
	srand(time(NULL));
	Initializator initializator;
	initializator.SetInitializator();
	initializator.Await();
	PBYTE foclient = GetFOClient();
	PathFinder* pathFinder = new PathFinder();
	int shift = 1, vk_f5 = 0;
	bool terminateOneHexThread = false;
	//D3D::Constructor(FO3_WND, &pathFinder->NeedMove, NULL);

	D3D::Constructor(FO3_WND, &haxSettings);

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

		//if (GetAsyncKeyState(VK_NUMPAD7)) {
		//	JustSender js;
		//	js.Send();
		//}

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
}*/


bool isActivated = false;

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || isActivated)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	isActivated = true;
	if (GetModuleHandleA(FO3_NAME))
	{
		CreateThread(0, 0, MainThread, 0, 0, 0);
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
		if (GetModuleHandleA(FO3_NAME))
		{
#ifdef CONSOLE
			ForDebug::CrtConsole();
			Beep(200, 200);
#endif
		}
		break;
    case DLL_PROCESS_DETACH:
#ifdef CONSOLE
		ForDebug::FreeCons();
#endif
        break;
    }
    return TRUE;
}