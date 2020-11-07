#include "FO3Funcs.h"

typedef void(__stdcall* f_WriteLog)(const char* str, ...);
typedef int* (__stdcall* f_NetBufferPushUint)(uint);
typedef int* (__stdcall* f_NetBufferPushUchar)(uchar);
typedef int* (__stdcall* f_NetBufferPushUshort)(ushort);
typedef void(__stdcall* f_ParseSocket)();
typedef void(__stdcall* f_NetOutput)();
typedef void(__stdcall* f_NetSendRefereshMe)();
typedef PBYTE(__stdcall* f_GetChosen)();
typedef bool(*f_IsCritterBusy)(uchar* critter);
typedef uchar* (_cdecl*f_GetCritterById)(ushort id);
typedef bool(__fastcall* f_FindPath)(PBYTE hexManager, PASS_ARG, PBYTE cr, ushort start_x, ushort start_y, ushort* end_x, ushort* end_y, std::vector<uchar>* steps, int cut);
typedef bool(__fastcall* f_TransitCritter)(PBYTE hexManager, PASS_ARG, PBYTE cr, int hx, int hy, bool animate, bool force);
typedef void(* f_MoveHexByDirUnsafe)(int* hx, int* hy, uchar dir);
typedef uint(__stdcall* f_DistGame)(int x1, int y1, int x2, int y2);
typedef int(* f_GetGameTime)();
typedef int(__fastcall*f_GetParam)(int*, PASS_ARG, int);

/*f_NetBufferPushUint netBufferPushUint_ = f_NetBufferPushUint((PBYTE)GET_ADR(NETBUFFER_PUSH_UINT));
f_NetBufferPushUint netBufferPushUchar_ = f_NetBufferPushUint((PBYTE)GET_ADR(NETBUFFER_PUSH_UCHAR));
f_NetBufferPushUint netBufferPushUshort_ = f_NetBufferPushUint((PBYTE)GET_ADR(NETBUFFER_PUSH_USHORT));
f_ParseSocket parseSocket_ = f_ParseSocket((PBYTE)GET_ADR(PARSE_SOCKET));
f_NetOutput netOutput_ = f_NetOutput((PBYTE)GET_ADR(NET_OUTPUT));
f_NetSendRefereshMe refreshMe_ = f_NetSendRefereshMe((PBYTE)GET_ADR(FO_REFRESH_ME));
f_GetChosen getChosen_ = f_GetChosen((PBYTE)GET_ADR(FO_GET_CHOSEN));
f_FindPath findPAth_ = f_FindPath((PBYTE)GET_ADR(FO_FIND_PATH));
f_TransitCritter transitCritter_ = f_TransitCritter((PBYTE)GET_ADR(FO_TRANSIT_CRITTER));
f_MoveHexByDirUnsafe moveHexByDirUnsafe_ = f_MoveHexByDirUnsafe((PBYTE)GET_ADR(FO_MOVE_HEX_BY_DIR_UNSAFE));
f_IsCritterBusy isCritterBusy_ = f_IsCritterBusy((PBYTE)GET_ADR(FO_IS_CRITTER_BUSY));
f_GetCritterById getCritterById_ = f_GetCritterById((PBYTE)GET_ADR(FO_GET_CRITTER_BY_ID));
f_GetGameTime getGameTime_ = f_GetGameTime((PBYTE)GET_ADR(0x5365D0));
f_GetParam getParam_ = f_GetParam((PBYTE)GET_ADR(0x4E3F60));*/
f_NetBufferPushUint netBufferPushUint_;
f_NetBufferPushUint netBufferPushUchar_;
f_NetBufferPushUint netBufferPushUshort_;
f_ParseSocket parseSocket_;
f_NetOutput netOutput_;
f_NetSendRefereshMe refreshMe_;
f_GetChosen getChosen_;
f_FindPath findPAth_;
f_TransitCritter transitCritter_;
f_MoveHexByDirUnsafe moveHexByDirUnsafe_;
f_IsCritterBusy isCritterBusy_;
f_GetCritterById getCritterById_;
f_GetGameTime getGameTime_;
f_GetParam getParam_;
void InitFo3Funcs() {
    netBufferPushUint_ = f_NetBufferPushUint((PBYTE)GET_ADR(NETBUFFER_PUSH_UINT));
    netBufferPushUchar_ = f_NetBufferPushUint((PBYTE)GET_ADR(NETBUFFER_PUSH_UCHAR));
    netBufferPushUshort_ = f_NetBufferPushUint((PBYTE)GET_ADR(NETBUFFER_PUSH_USHORT));
    parseSocket_ = f_ParseSocket((PBYTE)GET_ADR(PARSE_SOCKET));
    netOutput_ = f_NetOutput((PBYTE)GET_ADR(NET_OUTPUT));
    refreshMe_ = f_NetSendRefereshMe((PBYTE)GET_ADR(FO_REFRESH_ME));
    getChosen_ = f_GetChosen((PBYTE)GET_ADR(FO_GET_CHOSEN));
    findPAth_ = f_FindPath((PBYTE)GET_ADR(FO_FIND_PATH));
    transitCritter_ = f_TransitCritter((PBYTE)GET_ADR(FO_TRANSIT_CRITTER));
    moveHexByDirUnsafe_ = f_MoveHexByDirUnsafe((PBYTE)GET_ADR(FO_MOVE_HEX_BY_DIR_UNSAFE));
    isCritterBusy_ = f_IsCritterBusy((PBYTE)GET_ADR(FO_IS_CRITTER_BUSY));
    getCritterById_ = f_GetCritterById((PBYTE)GET_ADR(FO_GET_CRITTER_BY_ID));
    getGameTime_ = f_GetGameTime((PBYTE)GET_ADR(0x5365D0));
    getParam_ = f_GetParam((PBYTE)GET_ADR(0x4E3F60));
}


void ParseSocket(PBYTE foclient) {
    _asm {
        mov ecx, [foclient]
        mov ecx, [ecx]
    }
    parseSocket_();
}

void RefreshMe(PBYTE foclient) {
    _asm {
        mov ecx, [foclient]
    }
    refreshMe_();
}

void NetOutput(PBYTE foclient) {
    _asm {
        mov ecx, [foclient]
    }
    netOutput_();
}

void NetBufferPushUint(PBYTE foclient, uint value) {
    _asm {
        mov ecx, [foclient]
        lea ecx, [ecx + 340h]
    }
    netBufferPushUint_(value);
}

void NetBufferPushUchar(PBYTE foclient, uchar value) {
    _asm {
        mov ecx, [foclient]
        lea ecx, [ecx + 340h]
    }
    netBufferPushUchar_(value);
}
void NetBufferPushUshort(PBYTE foclient, ushort value) {
    _asm {
        mov ecx, [foclient]
        lea ecx, [ecx + 340h]
    }
    netBufferPushUshort_(value);
}

PBYTE GetFOClient()
{
    return (PBYTE)(*(PDWORD)GET_ADR( FO_CLIENT_ADR));
}

PBYTE GetChosen()
{
    return getChosen_();
}

bool __fastcall FindPath(PBYTE hexManager, PASS_ARG, PBYTE cr, ushort start_x, ushort start_y, ushort* end_x, ushort* end_y, std::vector<uchar>* steps, int cut)
{
    return findPAth_(hexManager, 0, cr, start_x, start_y, end_x, end_y, steps, cut);
}

bool __fastcall TransitCritter(PBYTE hexManager, PASS_ARG, PBYTE cr, int hx, int hy, bool animate, bool force)
{
    return transitCritter_(hexManager, 0, cr, hx, hy, animate, force);
}

void MoveHexByDirUnsafe(int* hx, int* hy, uchar dir)
{
    moveHexByDirUnsafe_(hx, hy, dir);
}

bool IsCritterBusy(uchar* critter)
{
    return isCritterBusy_(critter);
}

uchar* _cdecl GetCritterById(ushort id)
{
    return getCritterById_(id);
}

int GetGameTime()
{
    return getGameTime_();
}

int GetParam(int* critter, int param)
{
	return getParam_(critter, 0, param);
}

