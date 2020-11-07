#include "ScriptsDLL.h"

HookSetter* _hs;
int* _isEnabled1Hex;
_declspec(naked) void hook_CrittersProcess() {
    _asm {
                push eax
                mov eax, [_isEnabled1Hex]
                mov eax, [eax]
                test eax, eax
                pop eax
                jz original_value
                mov esi, 1
                jmp return_to_the_func

        original_value :
                add esi, eax

        return_to_the_func:
                mov eax, _hs
                mov eax, [eax + 10h]
                jmp eax
    }
}

HookSetter* _hsSendAttack;
PathFinder* _pathFinder;
HANDLE _runOneHexThread;
void __fastcall hook_SendAttack(int* this_, PASS_ARG, char a2, int a3, int a4, char a5, char a6, int targetId, int a8, int a9) {
    typedef  void(__fastcall*orig)(int* this_, PASS_ARG, char a2, int a3, int a4, char a5, char a6, int targetId, int a8, int a9);
    orig __exit = orig(_hsSendAttack->OriginalOps);
    __exit(this_, 0, a2, a3, a4, a5, a6, targetId, a8, a9);
    if (_pathFinder && _runOneHexThread && !_pathFinder->IsAttack) {
        _pathFinder->IsAttack = true;
        _pathFinder->Target = GetCritterById(targetId);
        ResumeThread(_runOneHexThread); 
    }
}

// Thread for safe 1 hex
bool* _terminate;
DWORD CALLBACK RunOneHex(LPVOID) {       
    while (true) {
        SuspendThread(_runOneHexThread);

        if (_pathFinder) {
            if (!_pathFinder->IsInited) _pathFinder->Init();
            if (_pathFinder->NeedMove && _pathFinder->IsAttack) {
                Sleep(200); // wait until attack animation will start
                uchar* target = _pathFinder->Target;
                if (target) {
                    ushort hx = (ushort)target[6],
                        hy = (ushort)target[8];
                    if (_pathFinder->FindBestHex(hx, hy)) {
                        _pathFinder->MoveChosenTo(hx, hy);
                    }
                }
            }
            _pathFinder->IsAttack = false;
        }

        if (_terminate && *_terminate) break; // terminate
    }
    ExitThread(0);
}

void Scripts::EnableOneHex(int* shift)
{
    _isEnabled1Hex = shift;
    _hs = CrtHookSetter((PBYTE)GET_ADR(FO3_ONE_HEX), (DWORD)hook_CrittersProcess, 6);
    SetHookSetter(_hs);
    ClearFirstBytesOriginalOps(_hs, 2);
}

HANDLE Scripts::EnablePathFinding(PathFinder* pathFinder, bool* terminate)
{
    _terminate = terminate;
    _pathFinder = pathFinder;
    _hsSendAttack = CrtHookSetter((PBYTE)GET_ADR(FO_SEND_ATTACK), (DWORD)hook_SendAttack, 10);
    SetHookSetter(_hsSendAttack);
    _runOneHexThread = CreateThread(0, 0, RunOneHex, 0, 0, 0);
    return _runOneHexThread;
}

void Scripts::UnsetHooks()
{
    UnsetHook(_hs);
    UnsetHook(_hsSendAttack);
}
