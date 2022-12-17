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
	const unsigned long DrawTextOnHeadAdr = 0x4EBD10;
	const unsigned long GetParamAdr = 0x4E8E60;
	const unsigned long DrawTextOnHeadUidFormatBufCallAdr = 0x4EBE6C;
	const unsigned long IsPLayerAdr = 0x463600;
	const unsigned long GetAttackDistAdr = 0x4E7AD0;
	const unsigned long CritterIsDeadAdr = 0x463640;
	const unsigned long IsChosenAdr = 0x463610;
	const unsigned long SetActionAdr = 0x48BDA0;
	const unsigned long GetCritterAdr = 0x47E720;
	const unsigned long IsAnimation3dPlayingAdr = 0x406DB0;
	const unsigned long IsCritterAnimAdr = 0x46E870;
}