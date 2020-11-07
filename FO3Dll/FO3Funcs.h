#pragma once
#include "framework.h"

void ParseSocket(PBYTE foclient);
void RefreshMe(PBYTE foclient);
void NetOutput(PBYTE foclient);
void NetBufferPushUint(PBYTE foclient, uint value);
void NetBufferPushUchar(PBYTE foclient, uchar value);
void NetBufferPushUshort(PBYTE foclient, ushort value);
PBYTE GetFOClient();
PBYTE GetChosen();
bool __fastcall FindPath(PBYTE hexManager, PASS_ARG, PBYTE cr, ushort start_x, ushort start_y, ushort* end_x, ushort* end_y, std::vector<uchar>* steps, int cut);
bool __fastcall TransitCritter(PBYTE hexManager, PASS_ARG, PBYTE cr, int hx, int hy, bool animate, bool force);
void MoveHexByDirUnsafe(int* hx, int* hy, uchar dir);
bool IsCritterBusy(uchar* critter);
uchar* _cdecl GetCritterById(ushort id);
int GetGameTime();
int GetParam(int* critter, int param);
void InitFo3Funcs();