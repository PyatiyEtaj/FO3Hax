#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <Psapi.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "HookSetter.h"
#include "XorStr.h"

// it's have to compile in release mode because by default msvc inline pretty much every function
// and optimize them, so all hooks (main functions) work perfectly, in debug mode work nothing 
// this macros need to turn on/off debug functions (net analizator, console and so on)
#define RELEASE // if need debug just comment this
//#define CONSOLE

#define PASS_ARG void*
using uint = unsigned int;
using uchar = unsigned char;
using ushort = unsigned short;

#define USHORT_MAX 65535

#define FO3_NAME xorstr("FOnline 3.exe")
#define FO2_NAME xorstr("FOnline 2.exe")
#define FO3_WND xorstr("FOnline 3")
#define CACHE_SCRIPTS xorstr("scripts.client_parameters.dll")

#define GET_CHOSEN(foclient) ((uchar**)((PDWORD)foclient + 12540))
#define GET_ADR(adr) ((DWORD)GetModuleHandleA(FO3_NAME) + adr - 0x400000)
#define GET_ADR_CACHE(adr) ((DWORD)GetModuleHandleA(CACHE_SCRIPTS) + adr - 0x10000000)
#define GET_ADR_PURE(adr) ((DWORD)GetModuleHandleA(FO3_NAME) + adr)

#define NETBUFFER_PUSH_UINT 0x460D60
#define NETBUFFER_PUSH_UCHAR 0x461160
#define NETBUFFER_PUSH_USHORT 0x460F60

#define NETBUFFER_WRITE_UINT 0x460DD0
#define NETBUFFER_WRITE_UINT_DUP 0x460ED0
#define NETBUFFER_WRITE_USHORT 0x460FD0
#define NETBUFFER_WRITE_USHORT_DUP 0x4610D0
#define NETBUFFER_WRITE_BOOL 0x4613D0
#define NETBUFFER_WRITE_UCHAR 0x4611D0

/*#define NETBUFFER_WRITE_UINT 0x460E48
#define NETBUFFER_WRITE_UINT_DUP 0x460F48
#define NETBUFFER_WRITE_USHORT 0x46104B
#define NETBUFFER_WRITE_USHORT_DUP 0x46114B
#define NETBUFFER_WRITE_BOOL 0x46144D
#define NETBUFFER_WRITE_UCHAR 0x461246*/

#define FO3_LOGIN 0x47430C

#define FO3_NETPROCESS 0x4A44E0
#define FO3_WSARECVEND 0x4649D4
#define FO3_WSASEND 0x4645E9

#define FO3_ONE_HEX 0x48D0DE

#define FO3_CRITTERS_PROCESS 0x48BEA0

#define WRITE_LOG 0x50D760
#define FO_CLIENT_ADR 0x2F9BBC8
#define PARSE_SOCKET 0x4A4C20
#define NET_OUTPUT 0x4644C0
#define GET_CRITTERS_DISTATION 0x4817A0

#define FO_CRITTER_MANAGER 0x85E518
#define FO_SEND_ATTACK 0x464B80
#define FO_GET_CRITTER_BY_ID 0x481870
#define FO_IS_CRITTER_BUSY 0x4809C0
#define FO_SEND_MOVE 0x4796C0
#define FO_TRANSIT_CRITTER 0x4FDBD0
#define FO_FIND_PATH 0x4FFFA0
#define FO_REFRESH_ME 0x46FE50
#define FO_GET_CHOSEN 0x466FF0
#define FO_MOVE_HEX_BY_DIR_UNSAFE 0x4D5C60 

#define FO_HANDLE_MSG_34 0x4902C8

#define GET_TIME_PTR (((DWORD*)*(DWORD*)GET_ADR_CACHE(0x1001829C))+7)

#define GLOBAL_MOUSE_Y 0x86F478
#define GLOBAL_MOUSE_X 0x86F47C

// debug info
#define SHOW_DEBUG_NET_MESSAGES 0x874269
#define SHOW_HELP_INFO 0x874267
#define SHOW_DEBUG_INFO 0x874268
#define SHOW_DEBUG_SPRITES 0x87426A
#define SHOW_HIGHLIGHT 0x874358
//---------------------------------------

struct HaxSettings {
	float Colors[3] = { 1.0f, 1.0f, 0.0f };
	int   ThreadLatency = 100;
	int   CrossHairMul = 1;
	bool  UseSafe1Hex = true;
	bool  ShowHealRateCD = true;
	bool  DisableWndProc = true;
	bool  IsMenuOpen = false;
	bool  IsImGuiInit = false;
	bool  Uninject = false;
	bool  NeedRefresh = false;
	bool  StartInPackets = false;
	bool  StartOutPackets = false;
	bool  OpenPackets = false;
	bool  OpenLog = false;
	bool  OpenSendPackets = false;
	int   TextPosX = 50;
	int   TextPosY = 300;
	int   FontSize = 18;
};