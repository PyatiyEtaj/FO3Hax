#include <iostream>
#include "AnalyzeNetBuffer.h"
#include "utils.h"
#include "HookSetter.h"
#include "D3D.h"
#include "FO3Funcs.h"
#include "Packets.h"

extern HaxSettings g_HaxSettings;

HookSetter* hs_out_uint;
HookSetter* hs_out_uchar;
HookSetter* hs_out_ushort;

HookSetter* hs_in_uint;
HookSetter* hs_in_uint_dup;
HookSetter* hs_in_bool;
HookSetter* hs_in_uchar;
HookSetter* hs_in_ushort;
HookSetter* hs_in_ushort_dup;

enum class ShowType {
    OutChar,
    OutInt,
    OutShort,
    InChar,
    InInt,
    InBool,
    InShort
};

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

inline void _cdecl IsMessage(DWORD ret, DWORD value, const char* type) {
    uchar msg = (value & 0x0000FF00) >> 8;
    (value & 0xFFFF00FF) == 0xBD5A00AA
        ? Packets::AddIn("[%s]    uint = [%11u] [0x%8X] [%s] {call=0x%6X}\n", type, msg, msg, MessageType(msg), ret)
        : Packets::AddIn("[%s]    uint = [%11u] [0x%8X] {call=0x%6X}\n", type, uint(value), uint(value), ret);
}

void _cdecl Show(DWORD ret, DWORD value, ShowType type) {
    switch (type)
    {
    case ShowType::OutChar:
        if (g_HaxSettings.StartOutPackets)
            Packets::AddOut("[out]  uchar = [%11u] [0x%8X] {call=0x%6X}\n", uchar(value), uchar(value), ret);
        break;
    case ShowType::OutInt:
        if (g_HaxSettings.StartOutPackets)
            Packets::AddOut("[out]   uint = [%11u] [0x%8X] {call=0x%6X}\n", uint(value), uint(value), ret);
        break;
    case ShowType::OutShort:
        if (g_HaxSettings.StartOutPackets)
            Packets::AddOut("[out] ushort = [%11u] [0x%8X] {call=0x%6X}\n", ushort(value), ushort(value), ret);
        break;

    case ShowType::InChar:
        if (g_HaxSettings.StartInPackets)
            Packets::AddIn("[in]   uchar = [%11u] [0x%8X] {call=0x%6X}\n", uchar(value), uchar(value), ret);
        break;
    case ShowType::InBool:
        if (g_HaxSettings.StartInPackets)
            Packets::AddIn("[in]    bool = [%11u] [0x%8X] {call=0x%6X}\n", bool(value), bool(value), ret);
        break;
    case ShowType::InInt:
        if (g_HaxSettings.StartInPackets)
            IsMessage(ret, value, "in");
        break;
    case ShowType::InShort:
        if (g_HaxSettings.StartInPackets)
            Packets::AddIn("[in]  ushort = [%11u] [0x%8X] {call=0x%6X}\n", ushort(value), ushort(value), ret);
        break;
    }
}


#define TEMPLATE_PUSH(name, valuetype, showtype, hs)  \
DWORD* __fastcall name(DWORD* this_, PASS_ARG, valuetype val) {  \
        typedef DWORD* (__fastcall* orig)(DWORD*, PASS_ARG, valuetype);\
        DWORD retAdr;\
        _asm { mov eax, [ebp + 4]} \
        _asm {mov[retAdr], eax} \
        Show(retAdr, val, showtype);\
        orig ret = orig(hs->OriginalOps);\
        return ret(this_, 0, val);\
}

TEMPLATE_PUSH(hkPushUint, uint, ShowType::OutInt, hs_out_uint)
TEMPLATE_PUSH(hkPushChar, uchar, ShowType::OutChar, hs_out_uchar)
TEMPLATE_PUSH(hkPushUshort, ushort, ShowType::OutShort, hs_out_ushort)
#undef TEMPLATE_PUSH

#define TEMPLATE_POP(name, valuetype, showtype, hs)  \
DWORD* __fastcall name(DWORD* this_, PASS_ARG, valuetype* val) {  \
        typedef DWORD* (__fastcall* orig)(DWORD*, PASS_ARG, valuetype*);\
        DWORD retAdr;\
        _asm { mov eax, [ebp + 4]} \
        _asm {mov[retAdr], eax} \
        orig ret = orig(hs->OriginalOps);\
        auto res = ret(this_, 0, val);\
        Show(retAdr, *val, showtype); \
        return res;\
}

TEMPLATE_POP(hkWriteUint, uint, ShowType::InInt, hs_in_uint)
TEMPLATE_POP(hkWriteUintDup, uint, ShowType::InInt, hs_in_uint_dup)
TEMPLATE_POP(hkWriteBool, bool, ShowType::InBool, hs_in_bool)
TEMPLATE_POP(hkWriteChar, uchar, ShowType::InChar, hs_in_uchar)
TEMPLATE_POP(hkWriteUshort, ushort, ShowType::InShort, hs_in_ushort)
TEMPLATE_POP(hkWriteUshortDup, ushort, ShowType::InShort, hs_in_ushort_dup)
#undef TEMPLATE_POP

/////////////////////////////////////////////////////////////////////////

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

void AnalyzeNetBuffer::InitializationOfAnalyzer()
{
    hs_in_uint = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_UINT), (DWORD)hkWriteUint, 6);
    SetHookSetter(hs_in_uint);
    hs_in_uint_dup = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_UINT_DUP), (DWORD)hkWriteUintDup, 6);
    SetHookSetter(hs_in_uint_dup);
    hs_in_uchar = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_UCHAR), (DWORD)hkWriteChar, 6);
    SetHookSetter(hs_in_uchar);
    hs_in_ushort = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_USHORT), (DWORD)hkWriteUshort, 6);
    SetHookSetter(hs_in_ushort);
    hs_in_ushort_dup = CrtHookSetter((PBYTE)GET_ADR(NETBUFFER_WRITE_USHORT_DUP), (DWORD)hkWriteUshortDup, 6);
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