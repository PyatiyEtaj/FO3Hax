#include "Initialization.h"
#include "framework.h"
#include "FO3Funcs.h"

HookSetter* hsCrittersProcess;
bool initializationDone = false;
void __fastcall hook_CrittersProcess(int* this_, PASS_ARG) {
	InitFo3Funcs();
	initializationDone = true;
	typedef void (__fastcall* orig)(int*, PASS_ARG);
	orig ret = orig(hsCrittersProcess->OriginalOps);
	ret(this_, 0);
}

void Initializator::SetInitializator() noexcept
{
	isInited_ = &initializationDone;
	hsCrittersProcess = CrtHookSetter((PBYTE)GET_ADR(FO3_CRITTERS_PROCESS), (DWORD)hook_CrittersProcess, 5);
    SetHookSetter(hsCrittersProcess);
}

void Initializator::Await() const noexcept
{
	while (!IsInit()) {
		Sleep(100);
	}
	UnsetHook(hsCrittersProcess);
}
