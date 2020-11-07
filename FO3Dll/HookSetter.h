#pragma once
#include <windows.h>

typedef struct 
{
	PBYTE OriginalCode;
	PBYTE MyCode;
	PBYTE FuncAdr;
	DWORD Adr;
	PBYTE OriginalOps;
	DWORD CountOfSafeByte; // value ~ [5, 11]
	bool IsSetted;
} HookSetter;

HookSetter* CrtHookSetter(PBYTE needToHookF, DWORD newAdrOfF, DWORD countOfSafeByte);
void SetHookSetter(HookSetter* hs);
void UnsetHook(HookSetter* hs);
void ClearFirstBytesOriginalOps(HookSetter* hs, unsigned int count);
int VMTHOOK(int** vptr, int vmtNumber, void* function);
int VMTHOOK(PDWORD vptr, int vmtNumber, void* function);
