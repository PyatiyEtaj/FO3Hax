#include "Initialization.h"
#include "framework.h"
#include "FO3Funcs.h"

HookSetter* _hsCrittersProcess;
bool _initializationDone = false;
void __fastcall hook_CrittersProcess(int* this_, PASS_ARG) {
	InitFo3Funcs();
	_initializationDone = true;
	typedef void (__fastcall* orig)(int*, PASS_ARG);
	orig ret = orig(_hsCrittersProcess->OriginalOps);
	ret(this_, 0);
}

void Initializator::SetInitializator() noexcept
{
	isInited_ = &_initializationDone;
	_hsCrittersProcess = CrtHookSetter((PBYTE)GET_ADR(FO3_CRITTERS_PROCESS), (DWORD)hook_CrittersProcess, 5);
    SetHookSetter(_hsCrittersProcess);
}

void Initializator::Await() const noexcept
{
	while (!IsInit()) {
		Sleep(100);
	}
	UnsetHook(_hsCrittersProcess);
}
