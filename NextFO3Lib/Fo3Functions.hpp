#pragma once

#include "Types.hpp"
#include "Hook32.hpp"
#include "GlobalState.hpp"
#include "framework.hpp"

namespace FO3
{
	class Fo3Functions {
	public:
		PBYTE UIDFlag = (PBYTE)0x87C275;

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
	};
}