#include "InGameScripts.h"
#include "framework.h"

uint* _nexTime;
void _cdecl Handle34(DWORD* ebp) {
	if ((WORD)ebp[-4] == 245) {
		*_nexTime = (uint)ebp[-5];
		//printf_s("0x%X=[%X] 0x%X=[%X]\n", 4 * 4, (WORD)ebp[-4], 4 * 5, ebp[-5]);
	}
}

HookSetter* _hsHandleMsg34;
void __declspec(naked) hook_HandleMsg34() {
	_asm {
		push ebp
		call Handle34
		add esp, 4
	}
	JMPPURE(_hsHandleMsg34)
}

void InGameScripts::SetHooks(unsigned int* nextTime)
{	
	_nexTime = nextTime;
	_hsHandleMsg34 = CrtHookSetter((PBYTE)GET_ADR(FO_HANDLE_MSG_34), (DWORD)hook_HandleMsg34, 7);
	SetHookSetter(_hsHandleMsg34);
}

void InGameScripts::UnsetHooks()
{
	UnsetHook(_hsHandleMsg34);
}
