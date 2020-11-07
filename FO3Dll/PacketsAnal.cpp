#include "PacketsAnal.h"
#include <ws2def.h>
#include <WinSock2.h>

LPWSABUF Buffer = nullptr;
LPWSABUF BufferIn = nullptr;
#define BUFFER_X4_SIZE 4
WSABUF BufferX4 [BUFFER_X4_SIZE];
PDWORD Stack = nullptr;
HookSetter* hs_send = nullptr;
HookSetter* hs_netprocess = nullptr;
HookSetter* hs_recv = nullptr;
DWORD count = 1;
int NeedDouble = 0;
DWORD AdrStartSendBytes = 0;
DWORD CountOfDoublePackets = 5;
PDWORD FOClient = nullptr;
bool* OutputP = nullptr;
bool* InputP = nullptr;
#define STACKPOS__COUNT_OF_BUFS 2
#define STACKPOS__BUFFER 1


void hkWSASend() {
	_asm
	{
		mov dword ptr ds : [Buffer] , eax
		mov dword ptr ds : [Stack], esp
	}

	if (Buffer != nullptr && *OutputP)
	{
		printf_s(("[SEND] count=%d adr=%p | adr_buf=%p | len=%d\n"),
			Stack[STACKPOS__COUNT_OF_BUFS], &Buffer->buf, Buffer->buf, Buffer[0].len
		);
		__printer((PBYTE)Buffer[0].buf, Buffer[0].len);
		printf_s(("===========================[WSASendEnd]======================\n\n"));
	}


	typedef  void(*fTmp)();
	fTmp __exit = fTmp(hs_send->OriginalOps);
	__exit();
}

void hkNetProcess() {
	if (*InputP)
		printf_s(("*************************************************************\n"));
	typedef  void(*fTmp)();
	fTmp __exit = fTmp(hs_netprocess->OriginalOps);
	__exit();
}

void hkWSARecvEnd() {
	if (*InputP)
		printf_s(("\n===========================[WSARecvEnd]======================\n"));
	typedef  void(*fTmp)();
	fTmp __exit = fTmp(hs_recv->OriginalOps);
	__exit();
}

void PacketsAnal::SetInlineHook(bool *input, bool *output)
{
	OutputP = output;
	InputP = input;
	//if (input) {
		hs_netprocess = CrtHookSetter((PBYTE)GET_ADR(FO3_NETPROCESS), (DWORD)hkNetProcess, 6);
		SetHookSetter(hs_netprocess);
		hs_recv = CrtHookSetter((PBYTE)GET_ADR(FO3_WSARECVEND), (DWORD)hkWSARecvEnd, 5);
		SetHookSetter(hs_recv);
	//}
	
	//if (output) {
		hs_send = CrtHookSetter((PBYTE)GET_ADR(FO3_WSASEND), (DWORD)hkWSASend, 6);
		SetHookSetter(hs_send);
	//}
}

void PacketsAnal::Unset()
{
	UnsetHook(hs_send);
	UnsetHook(hs_recv);
	UnsetHook(hs_netprocess);
}
