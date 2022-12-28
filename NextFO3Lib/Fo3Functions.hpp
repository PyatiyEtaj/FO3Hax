#pragma once

#include <map>

#include "Types.hpp"
#include "Hook32.hpp"
#include "GlobalState.hpp"
#include "framework.hpp"

namespace FO3
{
	class Mouse {
	public:
		PDWORD X = (PDWORD)Addresses::MouseX;
		PDWORD Y = (PDWORD)Addresses::MouseY;
	};

	enum class MouseType
	{
		ContextMenu = 0,
		InvCursor = 6,
		Move = 1,
		Attack = 3,
	};

	class Functions {
	public:
		PBYTE UIDFlag = (PBYTE)Addresses::UIDFlagAdr;

	public:
		Types::PCritterCl GetChosen();
		unsigned int GetCrittersDistantion(Types::PCritterCl from, Types::PCritterCl to);
		void SetVisible(Types::PCritterCl c, bool value);
		bool GetVisible(Types::PCritterCl c);
		void DrawTextOnHead(Types::PCritterCl c);
		char* GetName(Types::PCritterCl c);
		char* SetName(Types::PCritterCl c, std::string newName);
		void PatchUIDFlagFormatBufBy(PBYTE newFormatBuff);
		void RestoreUIDFlag();
		bool IsPlayer(Types::PCritterCl c);
		unsigned int GetAttackDist(Types::PCritterCl c);
		bool IsDead(Types::PCritterCl c);
		bool IsChosen(Types::PCritterCl c);
		void SetAction(Types::PFOClient fo, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8);
		void CallLastAttackAction(const GlobalState::GlobalObjects* go);
		Types::PCritterCl GetCritter(Types::PFOClient fo, unsigned int id);
		bool IsCritterAnim(Types::PCritterCl critter);
		unsigned int GetCritterHexX(Types::PCritterCl critter);
		unsigned int GetCritterHexY(Types::PCritterCl critter);
		bool GetHexPixel(Types::PHexManager hexManager, int hexX, int hexY, unsigned short* pixelX, unsigned short* pixelY);
		bool IsFinish(Types::PCritterCl critter);
		bool IsFree(Types::PCritterCl critter);
		int RealAp(Types::PCritterCl critter);
		void SetCursorMode(FO3::MouseType type);
		void AutoClick(const GlobalState::GlobalObjects* go);
	};
}