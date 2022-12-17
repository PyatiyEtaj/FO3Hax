#pragma once

#include "Util.hpp"
#include "Types.hpp"
#include "Hook32.hpp"

namespace GlobalState
{
	class Settings {
	public:
		bool IsInjected = false;
		bool IsConsoleActivate = false;
		Util::Console* Console = new Util::Console();

		~Settings()
		{
			if (Console != nullptr)
			{
				delete Console;
			}
		}
	};

	class KeyBoard {
	public:
		bool Shift = false;
		bool LeftControl = false;
		bool Space = false;
	};

	class GlobalObjects {
	public:
		Types::PFOClient Client = nullptr;

		Hooks::Hook32* OneHexHook = new Hooks::Hook32(true);

		Hooks::Hook32* SetActionHook = new Hooks::Hook32(true);
		unsigned int SetActionArgs[7] = {0};

		~GlobalObjects()
		{
			delete OneHexHook;
			delete SetActionHook;
		}
	};
}