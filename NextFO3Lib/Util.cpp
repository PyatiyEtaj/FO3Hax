#include "Util.hpp"

bool Util::Console::Create()
{
	bool check = AllocConsole();
	if (check)
	{
		char wnd_title[256];
		HWND hwnd = GetForegroundWindow();
		GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));

		FILE* stream;
		freopen_s(&stream, "CONOUT$", "w", stdout);
		system("chcp 1251");
		printf_s("Console of %s has opened\n=======================================================\n", wnd_title);
	}
	_consoleIsAllocated = check;
	return check;
}

void Util::Console::Free()
{
	if (_consoleIsAllocated) FreeConsole();
	_consoleIsAllocated = false;
}

Util::Console::~Console()
{
	Free();
}

bool Util::Patterns::Compare(const std::vector<BYTE>& b, PBYTE pos)
{
	for (size_t i = 0; i < b.size(); i++)
	{
		if (b[i] != 0x0 && b[i] != pos[i])
		{
			return false;
		}
	}
	return true;
}

PBYTE Util::Patterns::FindPattern(const std::vector<BYTE>& pattern, PBYTE start, PBYTE end)
{
	end -= pattern.size();
	int endI = pattern.size() - 1;
	int middle = endI / 2;
	for (; start < end; start++)
	{
		if (Compare(pattern, start))
			return start;
	}
}

MODULEINFO Util::Patterns::GetModuleInfo(const char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(szModule);
	if (hModule == 0) return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

PBYTE Util::Patterns::FindPatternInModule(std::vector<BYTE> pattern, LPCSTR moduleName, DWORD startAdr, DWORD endAdr)
{
	HANDLE hProc = GetCurrentProcess();
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	MODULEINFO mInfo = GetModuleInfo(moduleName);
	LPVOID lpMem = (startAdr == 0) ? mInfo.lpBaseOfDll : LPVOID(startAdr);
	LPVOID   end = (endAdr == 0) ? (LPVOID)((DWORD)lpMem + mInfo.SizeOfImage) : LPVOID(endAdr);
	if (startAdr != 0 && endAdr == 0) end = si.lpMaximumApplicationAddress;
	while (lpMem < end)
	{
		VirtualQueryEx(hProc, lpMem, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		DWORD oldprotect;
		if (VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect))
		{
			PBYTE ptr = FindPattern(pattern, (PBYTE)mbi.BaseAddress, (PBYTE)((DWORD)mbi.BaseAddress + (DWORD)mbi.RegionSize));
			if (ptr != nullptr) return ptr;
			DWORD __temp;
			VirtualProtect(mbi.BaseAddress, mbi.RegionSize, oldprotect, &__temp);
		}
		lpMem = (LPVOID)((DWORD)mbi.BaseAddress + (DWORD)mbi.RegionSize);
	}

	return nullptr;
}

PBYTE Util::Patterns::FindPattern(std::vector<BYTE> pattern, LPCSTR moduleName)
{
	HANDLE hProc = GetCurrentProcess();
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	MODULEINFO mInfo = GetModuleInfo(moduleName);
	LPVOID start = mInfo.lpBaseOfDll;
	LPVOID   end = (LPVOID)((DWORD)start + mInfo.SizeOfImage);
	return FindPattern(pattern, (PBYTE)start, (PBYTE)end);
}
