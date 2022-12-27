#pragma once

#include <string>

#include "Types.hpp"
#include "GlobalState.hpp"
#include "Fo3Functions.hpp"

extern GlobalState::KeyBoard Keyboard;
extern GlobalState::GlobalObjects GObjects;
extern FO3::Functions Fo3Functions;

namespace Features {
	const char* DrawTextOnHeadFormatBuffer(char* pattern, Types::PCritterCl critter)
	{
		auto chosen = Fo3Functions.GetChosen();

		if (chosen == nullptr || critter == nullptr || Fo3Functions.IsDead(critter)) return "";

		if (Fo3Functions.IsChosen(critter))
		{
			//auto dist = Fo3Functions.GetAttackDist(chosen);
			auto realAp = Fo3Functions.RealAp(chosen);
			GObjects.FormatBuffer = "  [" + std::to_string(realAp) + "]";
		}
		else
		{
			auto dist = Fo3Functions.GetCrittersDistantion(chosen, critter);
			GObjects.FormatBuffer = "  [" + std::to_string(dist) + "]";
		}

		return GObjects.FormatBuffer.c_str();
	}

	unsigned long __fastcall GetAttackDistHooked(Types::PCritterCl critter, SKIP_ARG)
	{
		if (Keyboard.Shift.IsDown()) return 1;

		return HooksUtil::CallFunctionThisCall<unsigned long>(GObjects.OneHexHook->GetStartOfOriginalFunction(), critter);
	}

	void __fastcall SetAction(Types::PFOClient this_, SKIP_ARG, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8)
	{
		GObjects.Client = this_;
		if (a2 == 6)
		{
			GObjects.SetActionArgs[0] = a2;
			GObjects.SetActionArgs[1] = a3;
			GObjects.SetActionArgs[2] = a4;
			GObjects.SetActionArgs[3] = a5;
			GObjects.SetActionArgs[4] = a6;
			GObjects.SetActionArgs[5] = a7;
			GObjects.SetActionArgs[6] = a8;
		}
		HooksUtil::CallFunctionThisCall<void>(GObjects.SetActionHook->GetStartOfOriginalFunction(), this_, a2, a3, a4, a5, a6, a7, a8);
	}

	void Activate()
	{
		Fo3Functions.PatchUIDFlagFormatBufBy((PBYTE)DrawTextOnHeadFormatBuffer);

		GObjects.OneHexHook->Set((PBYTE)Addresses::GetAttackDistAdr, (PBYTE)GetAttackDistHooked, 5);

		GObjects.SetActionHook->Set((PBYTE)Addresses::SetActionAdr, (PBYTE)SetAction, 6);
	}
}