#pragma once

// NAMES
#define FO3_NAME "FOnline 3.exe"
#define FO2_NAME "FOnline 2.exe"
#define FO3_WND  "FOnline 3"
////////////////////////////////

#define SKIP_ARG void*

namespace Addresses {
	const unsigned long GetChosenAdr = 0x468F80;				//FOClient::SScriptFunc::Global_GetChosen(void) .text 00466FB0 00000027
	const unsigned long GetCrittersDistantionAdr = 0x4833B0;	//FOClient::SScriptFunc::Global_GetCrittersDistantion(CritterCl *,CritterCl *) .text 004817A0 000000C7 00000004 00000008 R . . . B T .
	const unsigned long SetVisibleAdr = 0x482D20;				//FOClient::SScriptFunc::Crit_SetVisible(CritterCl *,bool) .text 00481110 0000005E 00000004 00000005 R . . . B T .
	const unsigned long GetVisibleAdr = 0x482D80;				//FOClient::SScriptFunc::Crit_GetVisible(CritterCl *) .text 00481170 00000041 00000004 00000004 R . . . B T .
	const unsigned long DrawTextOnHeadAdr = 0x4EF540;			//CritterCl::DrawTextOnHead(void) .text 004EBF10 000002D0 00002034 00000000 R . . . B T .
	const unsigned long GetParamAdr = 0x4EC230;					//CritterCl::GetParam(uint) .text 004E9060 00000076 0000000C 00000004 R . . . B T .
	const unsigned long DrawTextOnHeadUidFormatBufCallAdr = 0x4EF69C;	//?FormatBuf@Str@@YAPBDPBDZZ ; Str::FormatBuf(char const *,...)
	const unsigned long DrawTextOnHeadPushCritterObject = 0x4EF696;
	const unsigned long IsPLayerAdr = 0x465390;					//CritterCl::IsPlayer(void) .text 00463600 0000000B   R . . . . T .
	const unsigned long GetAttackDistAdr = 0x4EAE70;			//CritterCl::GetAttackDist(void) .text 004E7CD0 00000046 0000000C 00000000 R . . . B T .
	const unsigned long CritterIsDeadAdr = 0x4653D0;			//CritterCl::IsDead(void) .text 00463640 0000000D   R . . . . T .
	const unsigned long IsChosenAdr = 0x4653A0;					//CritterCl::IsChosen(void) .text 00463610 0000000D   R . . . . T .
	const unsigned long SetActionAdr = 0x48DE40;				//FOClient::SetAction(uint,uint,uint,uint,uint,uint,uint) .text 0048BDA0 0000003A 00000004 0000001C R . . . B T .
	const unsigned long GetCritterAdr = 0x4802C0;				//FOClient::GetCritter(uint) .text 0047E720 0000000C 00000004 00000000 R . . . B T .
	const unsigned long IsAnimation3dPlayingAdr = 0x4065D0;		//Animation3d::IsAnimationPlaying(void) .text 00406DB0 00000014 00000004 00000000 R . . . . T .
	const unsigned long IsCritterAnimAdr = 0x470530;			//CritterCl::IsAnim(void) .text 0046E870 00000026   R . . . . T .
	const unsigned long CritterIsFinish = 0x465620;				//CritterCl::IsFinish(void) .text 00463890 00000024 00000004 00000000 R . . . . T .
	const unsigned long CritterIsFree = 0x4655F0;				//CritterCl::IsFree(void) .text 00463860 00000019 00000004 00000000 R . . . . T .
	const unsigned long CritterRealAp = 0x4654E0;				//CritterCl::GetRealAp(void) .text 00463750 00000007   R . . . . T .
//.data:0087C25A                                         ; GetClientOptions(void)+102w ...
//.data:0087C25B byte_87C25B     db ?                    ; DATA XREF: GraphicLoader::LoadModel(IDirect3DDevice9 *,char const *)+222r
//.data:0087C25B                                         ; FOClient::ReloadScripts(void)+62C7o ...
//.data:0087C25C ; Script *dword_87C25C
//.data:0087C25C dword_87C25C    dd ?                    ; DATA XREF: FOClient::ProcessMouseScroll(void)+11r
//.data:0087C25C                                         ; FOClient::IsCurInRect(FlexRect<int> const &,int,int)+21r ...
//.data:0087C260 ; Script *dword_87C260
//.data:0087C260 dword_87C260    dd ?                    ; DATA XREF: FOClient::ProcessMouseScroll(void)+32r
//.data:0087C260                                         ; FOClient::IsCurInRect(FlexRect<int> const &,int,int)+33r ...
//.data:0087C264 dword_87C264    dd ?                    ; DATA XREF: FOClient::ScreenQuake(int,uint)+Dw
//; Script *dword_				FOClient::ProcessMouseScroll(void)
	const unsigned long MouseX = 0x0085B0BC;
	const unsigned long MouseY = 0x0085B0C0;
	const unsigned long GetMouseAdr = 0x5ED0F0;					//Fl::get_mouse(int &,int &) .text 005EAE20 00000024 0000000C 00000008 R . . . B T .
	const unsigned long GetAcceleratorNumAdr = 0x53F6D0;		//Timer::GetAcceleratorNum(void) .text 0053D420 00000006   R . . . . T .
	const unsigned long GetCrittersAdr = 0x465970;				//HexManager::GetCritters(void) .text 00463BE0 00000007   R . . . . . .

	const unsigned long CritterHexXAdr = 0x465350;				//CritterCl::GetHexX(void) .text 004635C0 00000005   R . . . . T .
	const unsigned long CritterHexYAdr = 0x465360;				//CritterCl::GetHexY(void) .text 004635D0 00000005   R . . . . T .

	const unsigned long GetHexPixelAdr = 0x501780;				//HexManager::GetHexPixel(int,int,ushort &,ushort &) .text 004FE580 00000248 0000002C 00000010 R . . . B T .

	const unsigned long UIDFlagAdr = 0x85B0D5;					//; FOClient::DropScroll(void)+1Aw ... 3 lines up

	const unsigned long FOClientAdr = 0x845398;					//.data:00866538 ; FOClient *FOClient::Self

	const unsigned long MainWindow = 0x02EB14A4;					//.data:02ED2644 ; struct Fl_Window *MainWindow

	const unsigned long SetCursorMode = 0x469F00;				//FOClient::SScriptFunc::Global_ChangeCursor(int) .text 00467F40 00000014 00000004 00000004 R . . . B T .

	const unsigned long GameLMouseDown = 0x4DA390;				//FOClient::GameLMouseDown(void) .text 004D7200 00000388 00000014 00000000 R . . . B T .

	const unsigned long GetCritterPixel = 0x509B80;				//HexManager::GetCritterPixel(int,int,bool) .text 00506960 0000029E 00000038 00000009 R . . . B T .
}
