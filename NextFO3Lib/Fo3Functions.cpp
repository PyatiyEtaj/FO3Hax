#include "Fo3Functions.hpp"

Types::PCritterCl FO3::Functions::GetChosen()
{
	return HooksUtil::CallFunction<Types::PCritterCl>((void*)Addresses::GetChosenAdr);
}

unsigned int FO3::Functions::GetCrittersDistantion(Types::PCritterCl from, Types::PCritterCl to)
{
	return HooksUtil::CallFunction<unsigned int>((void*)Addresses::GetCrittersDistantionAdr, from, to);
}

void FO3::Functions::SetVisible(Types::PCritterCl c, bool value)
{
	HooksUtil::CallFunction<void>((void*)Addresses::SetVisibleAdr, c, value);
}

bool FO3::Functions::GetVisible(Types::PCritterCl c)
{
	return HooksUtil::CallFunction<bool>((void*)Addresses::GetVisibleAdr, c);
}

void FO3::Functions::DrawTextOnHead(Types::PCritterCl c)
{
	HooksUtil::CallFunctionThisCall<void>((void*)Addresses::DrawTextOnHeadAdr, c);
}

char* FO3::Functions::GetName(Types::PCritterCl c)
{
	return (char*)*((DWORD*)c + 1530);
}

char* FO3::Functions::SetName(Types::PCritterCl c, std::string newName)
{
	if (newName.size() < 0 || newName.size() > 31)
	{
		return GetName(c);
	}
	auto name = GetName(c);

	memcpy_s(name, 32, newName.data(), newName.size());

	return name;
}

void FO3::Functions::PatchUIDFlagFormatBufBy(PBYTE newFormatBuff)
{
	DWORD old;
	auto pushEax = (PBYTE)Addresses::DrawTextOnHeadPushCritterObject;
	if (VirtualProtect(pushEax, 0x16, PAGE_EXECUTE_READWRITE, &old))
	{
		*pushEax = 0x56; // patch to push esi -- push object Critter
		HooksUtil::DetourFunc((PBYTE)Addresses::DrawTextOnHeadUidFormatBufCallAdr, newFormatBuff);
		*UIDFlag = TRUE;
	}
}

void FO3::Functions::RestoreUIDFlag()
{
	*UIDFlag = FALSE;
}

bool FO3::Functions::IsPlayer(Types::PCritterCl c)
{
	return HooksUtil::CallFunctionThisCall<bool>((void*)Addresses::IsPLayerAdr, c);
}

unsigned int FO3::Functions::GetAttackDist(Types::PCritterCl c)
{
	return HooksUtil::CallFunctionThisCall<unsigned int>((void*)Addresses::GetAttackDistAdr, c);
}

bool FO3::Functions::IsDead(Types::PCritterCl c)
{
	return HooksUtil::CallFunctionThisCall<bool>((void*)Addresses::CritterIsDeadAdr, c);
}

bool FO3::Functions::IsChosen(Types::PCritterCl c)
{
	return HooksUtil::CallFunctionThisCall<bool>((void*)Addresses::IsChosenAdr, c);
}

void FO3::Functions::SetAction(Types::PFOClient fo, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8)
{
	HooksUtil::CallFunctionThisCall<void>((void*)Addresses::SetActionAdr, fo, a2, a3, a4, a5, a6, a7, a8);
}

void FO3::Functions::CallLastAttackAction(const GlobalState::GlobalObjects* go)
{
	if (go->Client != nullptr)
	{
		auto chosen = GetChosen();
		if (chosen != nullptr /*&& !functions.IsCritterAnim(chosen)*/)
		{
			auto idCritter = go->SetActionArgs[4];
			auto critter = GetCritter(go->Client, idCritter);
			if (critter != nullptr /*&& GetAttackDist(chosen) >= GetCrittersDistantion(chosen, critter)*/)
			{
				SetAction(
					go->Client,
					go->SetActionArgs[0],
					go->SetActionArgs[1],
					go->SetActionArgs[2],
					go->SetActionArgs[3],
					go->SetActionArgs[4],
					go->SetActionArgs[5],
					go->SetActionArgs[6]);
			}
		}
	}
}

Types::PCritterCl FO3::Functions::GetCritter(Types::PFOClient fo, unsigned int id)
{
	return HooksUtil::CallFunctionThisCall<Types::PCritterCl>((void*)Addresses::GetCritterAdr, fo, id);
}

bool FO3::Functions::IsCritterAnim(Types::PCritterCl critter)
{
	return HooksUtil::CallFunctionThisCall<bool>((void*)Addresses::IsCritterAnimAdr, critter);
}

unsigned int FO3::Functions::GetCritterHexX(Types::PCritterCl critter)
{
	return HooksUtil::CallFunctionThisCall<unsigned int>((void*)Addresses::CritterHexXAdr, critter);
}

unsigned int FO3::Functions::GetCritterHexY(Types::PCritterCl critter)
{
	return HooksUtil::CallFunctionThisCall<unsigned int>((void*)Addresses::CritterHexYAdr, critter);
}

bool FO3::Functions::GetHexPixel(Types::PHexManager hexManager, int hexX, int hexY, unsigned short* pixelX, unsigned short* pixelY)
{
	return HooksUtil::CallFunctionThisCall<bool>((void*)Addresses::GetHexPixelAdr, hexManager, hexX, hexY, pixelX, pixelY);
}
