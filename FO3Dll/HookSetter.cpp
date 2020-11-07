#include "HookSetter.h"
#include "utils.h"

const unsigned int BYTES_BACKUP = 16;

HookSetter* CrtHookSetter(PBYTE needToHookF, DWORD newAdrOfF, DWORD countOfSafeByte)
{
	HookSetter* hs = (HookSetter*)malloc(sizeof(HookSetter));
	if (hs)
	{
		hs->OriginalCode = (PBYTE)calloc(countOfSafeByte, sizeof(BYTE));
		hs->MyCode = (PBYTE)calloc(countOfSafeByte, sizeof(BYTE));
		hs->FuncAdr = needToHookF;
		hs->Adr = newAdrOfF;
		hs->CountOfSafeByte = countOfSafeByte;
		hs->IsSetted = false;
	}

	return hs;
}

PBYTE __makeorigcall(DWORD safe, PBYTE adr, BYTE* originalCode)
{
	PBYTE changedCode = (PBYTE)malloc(BYTES_BACKUP);
	if (changedCode)
	{
		ZeroMemory(changedCode, BYTES_BACKUP);
		memcpy_s(changedCode, safe, originalCode, safe);
		changedCode[safe] = 0xE9;
		DWORD addr = (DWORD)(adr + safe) - (DWORD)(changedCode + safe) - 5;
		memcpy_s(changedCode + (safe + 1), 4, (void*)&addr, 4);
		DWORD temp;
		VirtualProtect(changedCode, BYTES_BACKUP, PAGE_EXECUTE_READWRITE, &temp);
	}
	return changedCode;
}

bool __sethook(HookSetter* hs, DWORD hookedFunc, PBYTE func9)
{
	DWORD oldProtectDip = 0;
	if (VirtualProtect((PBYTE)func9, 8, PAGE_EXECUTE_READWRITE, &oldProtectDip))
	{
		memcpy_s(hs->OriginalCode, hs->CountOfSafeByte, func9, hs->CountOfSafeByte);
		hs->MyCode[0] = 0xE9;
		DWORD addr = hookedFunc - (DWORD)func9 - 5;
		memcpy_s(hs->MyCode + 1, 4, (void*)&addr, 4);
		memcpy_s(func9, 5, hs->MyCode, 5);
		return true;
	}

	return false;
}

void SetHookSetter(HookSetter* hs)
{
	if (__sethook(hs, hs->Adr, hs->FuncAdr)) {
		hs->OriginalOps = __makeorigcall(hs->CountOfSafeByte, hs->FuncAdr, hs->OriginalCode);
		hs->IsSetted = true;
	}
}

void UnsetHook(HookSetter* hs)
{
	if (hs)
	{
		if (hs->IsSetted) memcpy_s(hs->FuncAdr, hs->CountOfSafeByte, hs->OriginalCode, hs->CountOfSafeByte);
		free(hs->OriginalCode);
		free(hs->OriginalOps); // ne uveren, hotay nujno
		free(hs->MyCode);
		free(hs);
	}
}

void ClearFirstBytesOriginalOps(HookSetter* hs, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
		hs->OriginalOps[i] = 0x90;
}

int VMTHOOK(int** vptr, int vmtNumber, void* function)
{
	DWORD temp;
	int originalFunction = vptr[0][vmtNumber];
	if (VirtualProtect(*vptr, 4, PAGE_EXECUTE_READWRITE, &temp)) {
		vptr[0][vmtNumber] = (int)function;
	}
	return originalFunction;
}

int VMTHOOK(PDWORD vptr, int vmtNumber, void* function)
{
	DWORD temp;
	int originalFunction = vptr[vmtNumber];
	if (VirtualProtect(vptr, 4, PAGE_EXECUTE_READWRITE, &temp)) {
		vptr[vmtNumber] = (int)function;
	}
	return originalFunction;
}

