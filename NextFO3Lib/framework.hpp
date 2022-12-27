#pragma once

// NAMES
#define FO3_NAME "FOnline 3.exe"
#define FO2_NAME "FOnline 2.exe"
#define FO3_WND  "FOnline 3"
////////////////////////////////

#define SKIP_ARG void*

namespace Addresses {
	const unsigned long GetChosenAdr = 0x466FB0;
	const unsigned long GetCrittersDistantionAdr = 0x4817A0;
	const unsigned long SetVisibleAdr = 0x481110;
	const unsigned long GetVisibleAdr = 0x481170;
	const unsigned long DrawTextOnHeadAdr = 0x4EBF10;
	const unsigned long GetParamAdr = 0x4E9060;
	const unsigned long DrawTextOnHeadUidFormatBufCallAdr = 0x4EC06C;
	const unsigned long DrawTextOnHeadPushCritterObject = 0x4EC066;
	const unsigned long IsPLayerAdr = 0x463600;
	const unsigned long GetAttackDistAdr = 0x4E7CD0;
	const unsigned long CritterIsDeadAdr = 0x463640;
	const unsigned long IsChosenAdr = 0x463610;
	const unsigned long SetActionAdr = 0x48BDA0;
	const unsigned long GetCritterAdr = 0x47E720;
	const unsigned long IsAnimation3dPlayingAdr = 0x406DB0;
	const unsigned long IsCritterAnimAdr = 0x46E870;
	const unsigned long CritterIsFinish = 0x463890;
	const unsigned long CritterIsFree = 0x463860;
	const unsigned long CritterRealAp = 0x463750;

	const unsigned long MouseX = 0x0087C25C;
	const unsigned long MouseY = 0x0087C260;
	const unsigned long GetMouseAdr = 0x5EAE20;
	const unsigned long GetAcceleratorNumAdr = 0x53D420;
	const unsigned long GetCrittersAdr = 0x463BE0;

	const unsigned long CritterHexXAdr = 0x4635C0;
	const unsigned long CritterHexYAdr = 0x4635D0;

	const unsigned long GetHexPixelAdr = 0x4FE580;

	const unsigned long UIDFlagAdr = 0x87C275;

	const unsigned long FOClientAdr = 0x866538;

	const unsigned long MainWindow = 0x2ED2644;
}