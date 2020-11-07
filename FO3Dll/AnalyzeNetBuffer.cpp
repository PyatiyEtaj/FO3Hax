#include <iostream>
#include "AnalyzeNetBuffer.h"
#include "framework.h"
#include "utils.h"
#include "HookSetter.h"
#include "D3D.h"
#include "FO3Funcs.h"

const char* OutPushUintMsg   = "[out]   uint = [%d] [0x%X]\n";
const char* OutPushUcharMsg  = "[out]  uchar = [%d] [0x%X]\n";
const char* OutPushUshortMsg = "[out] ushort = [%d] [0x%X]\n";
const char* InWriteUintMsg   = "[in]    uint = [%d] [0x%X]\n";
const char* InWriteUcharMsg  = "[in]   uchar = [%d] [0x%X]\n";
const char* InWriteUshortMsg = "[in]  ushort = [%d] [0x%X]\n";

HookSetter* hs_out_uint;
HookSetter* hs_out_uchar;
HookSetter* hs_out_ushort;

HookSetter* hs_in_uint;
HookSetter* hs_in_uint_dup;
HookSetter* hs_in_bool;
HookSetter* hs_in_uchar;
HookSetter* hs_in_ushort;
HookSetter* hs_in_ushort_dup;

DWORD* _foTime = nullptr;

#define SHOW_CHAR 0
#define SHOW_INT 1
#define SHOW_SHORT 2

#define SHOW_IN_CHAR 3
#define SHOW_IN_INT 4
#define SHOW_IN_BOOL 5
#define SHOW_IN_SHORT 6

bool* Output = nullptr;
bool* Input = nullptr;

const char* MessageType(uchar msg) {
    switch (msg)
    {
    case 0x2E:
        return "Net_OnCritterXY";
    case 0x5A:
        return "Net_OnSomeItem";
    case 0x7E:
        return "Net_OnRunClientScript";
    case 0x5B:
        return "Net_???[0x5B]";
    case 0x65:
        return "Net_???[0x65]";
    case 0x34:
        return "Net_???[0x34]";
    case 0x41:
        return "Net_OnChosenAddItem";
    case 0x22:
        return "Net_OnTextMsg";
    case 0x42:
        return "Net_OnChosenEraseItem";
    case 0x53:
        return "Net_OnContainerInfo";
    case 0x4E:
        return "Net_OnCheckUID2";
    case 0x1E:
        return "Net_OnCheckUID1";
    case 0x4B:
        return "Net_???[0x4B]";
    case 0x47:
        return "Net_OnAddItemOnMap";
    case 0x48:
        return "Net_OnChangeItemOnMap";
    case 0x4A:
        return "Net_OnEraseItemFromMap";
    case 0x3F:
        return "Net_???[0x3F]";
    case 0x40:
        return "Net_???[0x40]";
    case 0x3D:
        return "Net_OnCraftAsk";
    case 0x33:
        return "Net_chosen...";
    case 0x24:
        return "Net_OnMapTextMsg";
    case 0x25:
        return "Net_OnMapTextMsgLex";
    case 0x2A:
        return "Net_OnCritterDir";
    case 0x23:
        return "Net_OnMapText";
    case 0x20:
        return "Net_OnText";
    case 0x17:
        return "Net_OnItemLexems";
    case 0x0C:
        return "Net_OnAddCritter";
    case 0x0D:
        return "Net_OnRemoveCritter";
    case 0x16:
        return "Net_OnCritterLexems";
    case 0x06:
        return "Net_OnCheckUID0";
    case 0x05:
        return "Net_Ping";
    case 0x02:
        return "Net_AuthSuccess";
    case 0x75:
        return "Net_OnGameInfo";
    case 0x7A:
        return "Net_OnMap";
    case 0x63:
        return "Net_OnFlyEffect";
    case 0x5D:
        return "Net_OnCritterMoveItem";
    case 0x8E:
        return "Net_OnProtoItemData";
    case 0x8D:
        return "Net_OnMsgData";
    }
    return "unknown";
}

inline void _cdecl IsMessage(DWORD value, const char* type) {
    uchar msg = (value & 0x0000FF00) >> 8;
    (value & 0xFFFF00FF) == 0xBD5A00AA
        ? printf_s("[%s]    uint = [%11u] [0x%8X] [%s]\n", type, msg, msg, MessageType(msg))
        : printf_s("[%s]    uint = [%11u] [0x%8X] [%d]\n", type, uint(value), uint(value), value - *_foTime);
}

void _cdecl Show(DWORD ret, DWORD value, int type) {
    switch (type)
    {
    case SHOW_CHAR:
        if (*Output)
            printf_s("[out]  uchar = [%11u] [0x%8X] {call=0x%6X}\n", uchar(value), uchar(value), ret);
            //printf_s("\tchar = 0x%X;\n", uchar(value));
        break;
    case SHOW_INT:
        if (*Output)
            printf_s("[out]   uint = [%11u] [0x%8X] {call=0x%6X}\n", uint(value), uint(value), ret);
            //printf_s("\tint = 0x%X;\n", uint(value));
        break;
    case SHOW_SHORT:
        if (*Output)
            printf_s("[out] ushort = [%11u] [0x%8X] {call=0x%6X}\n", ushort(value), ushort(value), ret);
            //printf_s("\tshort = 0x%X;\n", ushort(value));
        break;

    case SHOW_IN_CHAR:
        if (*Input)
            printf_s("[in]   uchar = [%11u] [0x%8X]\n", uchar(value), uchar(value));
        break;
    case SHOW_IN_BOOL:
        if (*Input)
            printf_s("[in]    bool = [%11u] [0x%8X]\n", bool(value), bool(value));
        break;
    case SHOW_IN_INT:
        //printf_s("[in]    uint = [%11u] [0x%8X] {return=0x%6X}\n", uint(value), uint(value), ret);
        if (*Input)
            IsMessage(value, "in");
        break;
    case SHOW_IN_SHORT:
        if (*Input)
            printf_s("[in]  ushort = [%11u] [0x%8X]\n", ushort(value), ushort(value));
        break;
    }
}

void __stdcall hkPushUint() {
    _asm {
        mov eax, [ebp + 8]
        push SHOW_INT
        push eax
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_out_uint
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_out_uint)
}

void __stdcall hkPushChar() {
    _asm {
        mov eax, [ebp + 8]
        push SHOW_CHAR
        push eax
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_out_uchar
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_out_uchar)
}

void __stdcall hkPushUshort() {
    _asm {
        mov eax, [ebp + 8]
        push SHOW_SHORT
        push eax
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_out_ushort
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_out_ushort)
}

void __stdcall hkWriteUint() {
    _asm {
        push SHOW_IN_INT
        push[eax]
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_in_uint
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_in_uint)
}

void __stdcall hkWriteBool() {
    _asm {
        push SHOW_IN_BOOL
        push[edx]
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_in_bool
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_in_bool)
}

void __stdcall hkWriteUint_dup() {
    _asm {
        push SHOW_IN_INT
        push[eax]
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_in_uint_dup
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_in_uint_dup)
}

void __stdcall hkWriteChar() {
    _asm {
        push SHOW_IN_CHAR
        push[eax]
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_in_uchar
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_in_uchar)
}

void __stdcall hkWriteUshort() {
    _asm {
        push SHOW_IN_SHORT
        push[eax]
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_in_ushort
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_in_ushort)
}

void __stdcall hkWriteUshortDup() {
    _asm {
        push SHOW_IN_SHORT
        push[eax]
        mov eax, [ebp + 4]
        push eax
        call Show
        add esp, 4 * 3
        ; mov eax, hs_in_ushort
        ; mov eax, [eax + 10h]
        ; pop ebp
        ; jmp eax
    }
    JMP(hs_in_ushort)
}


void AnalyzeNetBuffer::InitializationOfAnalyzer(bool* input, bool* output) {
    Output = output;
    Input= input;
    _foTime = GET_TIME_PTR;
    hs_in_uint = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_UINT), (DWORD)hkWriteUint, 5);
    SetHookSetter(hs_in_uint);
    hs_in_uint_dup = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_UINT_DUP), (DWORD)hkWriteUint_dup, 5);
    SetHookSetter(hs_in_uint_dup);
    hs_in_uchar = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_UCHAR), (DWORD)hkWriteChar, 6);
    SetHookSetter(hs_in_uchar);
    hs_in_ushort = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_USHORT), (DWORD)hkWriteUshort, 5);
    SetHookSetter(hs_in_ushort);
    hs_in_ushort_dup = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_USHORT_DUP), (DWORD)hkWriteUshortDup, 5);
    SetHookSetter(hs_in_ushort_dup);
    hs_in_bool = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_BOOL), (DWORD)hkWriteBool, 6);
    SetHookSetter(hs_in_bool);

    hs_out_uint = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_PUSH_UINT), (DWORD)hkPushUint, 6);
    SetHookSetter(hs_out_uint);
    hs_out_uchar = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_PUSH_UCHAR), (DWORD)hkPushChar, 6);
    SetHookSetter(hs_out_uchar);
    hs_out_ushort = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_PUSH_USHORT), (DWORD)hkPushUshort, 6);
    SetHookSetter(hs_out_ushort);
}

void AnalyzeNetBuffer::SetOutputDebugMessages(bool flag)
{
    PBYTE showDbgNetMsg = (PBYTE)GET_ADR(SHOW_DEBUG_NET_MESSAGES);
    *showDbgNetMsg = flag;
    showDbgNetMsg = (PBYTE)GET_ADR(SHOW_HELP_INFO);
    *showDbgNetMsg = flag;
    showDbgNetMsg = (PBYTE)GET_ADR(SHOW_DEBUG_INFO);
    *showDbgNetMsg = flag;
    //showDbgNetMsg = (PBYTE)GET_ADR(SHOW_DEBUG_SPRITES);
    //*showDbgNetMsg = flag;
    showDbgNetMsg = (PBYTE)GET_ADR(SHOW_HIGHLIGHT);
    *showDbgNetMsg = flag;
}


void AnalyzeNetBuffer::Unset() {
    UnsetHook(hs_out_uint);
    UnsetHook(hs_out_uchar);
    UnsetHook(hs_out_ushort);

    UnsetHook(hs_in_uint);
    UnsetHook(hs_in_uint_dup);
    UnsetHook(hs_in_uchar);
    UnsetHook(hs_in_ushort);
    UnsetHook(hs_in_bool);
}


void Send_vic_1() {
    PBYTE foclient = (PBYTE)(*(PDWORD)GET_ADR(FO_CLIENT_ADR));
    Sleep(1000);
}

void AnalyzeNetBuffer::SendMsg()
{
    Send_vic_1();
}

#undef SHOW_CHAR
#undef SHOW_INT
#undef SHOW_SHORT
#undef SHOW_IN_CHAR
#undef SHOW_IN_INT
#undef SHOW_IN_SHORT