#pragma once
#include <Windows.h>
#include <vector>
#include <iomanip>
#include <Psapi.h>

namespace Util
{
	class Console
	{
	private:
		bool _consoleIsAllocated = false;
	public:
		bool Create();
		void Free();

		~Console();
	};

	namespace Patterns {
		bool Compare(const std::vector<BYTE>& b, PBYTE pos);

		PBYTE FindPattern(const std::vector<BYTE>& pattern, PBYTE start, PBYTE end);

		MODULEINFO GetModuleInfo(const char* szModule);

		PBYTE FindPatternInModule(
			std::vector<BYTE> pattern,
			LPCSTR moduleName,
			DWORD startAdr, DWORD endAdr
		);

		PBYTE FindPattern(std::vector<BYTE> pattern, LPCSTR moduleName);
	}
};