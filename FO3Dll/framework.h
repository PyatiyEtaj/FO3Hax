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

#define PASS_ARG void*
#define uint unsigned int
#define uchar unsigned char
#define ushort unsigned short

#define USHORT_MAX 65535

#define FO3_NAME xorstr("FOnline 3.exe")
#define FO2_NAME xorstr("FOnline 2.exe")
#define FO3_WND xorstr("FOnline 3")
#define CACHE_SCRIPTS xorstr("scripts.client_parameters.dll")

#define GET_ADR(adr) ((DWORD)GetModuleHandleA(FO3_NAME) + adr - 0x400000)
#define GET_ADR_CACHE(adr) ((DWORD)GetModuleHandleA(CACHE_SCRIPTS) + adr - 0x10000000)
#define GET_ADR_PURE(adr) ((DWORD)GetModuleHandleA(FO3_NAME) + adr)

#define NETBUFFER_PUSH_UINT 0x460D60
#define NETBUFFER_PUSH_UCHAR 0x461160
#define NETBUFFER_PUSH_USHORT 0x460F60

#define NETBUFFER_WRITE_UINT 0x460E48
#define NETBUFFER_WRITE_UINT_DUP 0x460F48
#define NETBUFFER_WRITE_BOOL 0x46144D
#define NETBUFFER_WRITE_UCHAR 0x461246
#define NETBUFFER_WRITE_USHORT 0x46104B
#define NETBUFFER_WRITE_USHORT_DUP 0x46114B

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


#define JMP(hs)\
		_asm { mov eax, hs }\
		_asm { mov eax, [eax + 10h]}\
		_asm { pop ebp }\
		_asm { jmp eax }

#define JMPPURE(hs) \
		_asm { mov eax, hs}\
		_asm { mov eax, [eax + 10h]}\
		_asm { jmp eax}


/*
[out]   uint = [ 3176807594] [0xBD5A3CAA] {call=0x464FA0} // msg
[out]   uint = [         12] [0x       C] {call=0x464FB2} // length of packets
[out] ushort = [          0] [0x       0] {call=0x464FBA} // count of up skills
* -- skill number
* -- count of sp
[out] ushort = [        457] [0x     1C9] {call=0x465045} // choosen perk
[SEND] count=1 adr=00199538 | adr_buf=04FDE680 | len=12
51 1 5A BD FB CF 0 0 A7 FF 36 73
===========================[WSASendEnd]======================
*/