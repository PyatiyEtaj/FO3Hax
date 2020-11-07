#include "utils.h"

void DetourFunc(PBYTE adrOld, DWORD adrNew, bool needE9)
{
	if (!adrOld) { printf_s("detourFunc failed\n"); return; }
	DWORD off = adrNew - (DWORD)adrOld - 5;
	PBYTE bOff = (PBYTE)&off;
	if (needE9) memcpy_s((void*)(adrOld), 1, "\xE9", 1);
	memcpy_s((void*)(adrOld + 1), 4, bOff, 4);
}

std::vector<BYTE> ReadFileHex(LPCSTR path)
{
	std::ifstream file(path);
	std::vector<BYTE> bytes;
	if (file.is_open())
	{
		int buf;
		while (file >> std::hex >> buf)
		{
			bytes.push_back(BYTE(buf));
		}
		file.close();
	}
	return bytes;
}

std::vector<BYTE> CrtVec(const char* str, int length)
{
	std::vector<BYTE> vec;
	for (int i = 0; i < length; i++)
	{
		vec.emplace_back(str[i]);
	}
	return vec;
}

void __printer(const BYTE* b, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf_s("%X ", b[i]);
	}
	printf_s("\n");
}


void __printerFile(const BYTE* b, int length, LPCSTR name)
{
	std::ofstream f(name);
	for (int i = 0; i < length; i++)
	{
		f << std::hex << b[i];
	}
	f << "\n";
	f.close();
}

bool __compare(const std::vector<BYTE>& b, PBYTE pos)
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

PBYTE FindPattern(const std::vector<BYTE>& pattern, PBYTE start, PBYTE end)
{
	end -= pattern.size();
	int endI = pattern.size() - 1;
	int middle = endI / 2;
	for (; start < end; start++)
	{
		//if (!(start[0] == pattern[0] || (pattern[0] == 0x0))) continue;
		//if (!(start[middle] == pattern[middle] || (pattern[middle] == 0x0))) { start += middle-1; continue; }
		//if (!(start[endI] == pattern[endI] || (pattern[endI] == 0x0))) { start += endI-1; continue; }

		if (__compare(pattern, start)) 
			return start;
	}
	return nullptr;
}

MODULEINFO GetModuleInfo(const char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(szModule);
	if (hModule == 0) return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

PBYTE FindPatternInModule(
	std::vector<BYTE> pattern,
	LPCSTR moduleName,
	DWORD startAdr, DWORD endAdr
)
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

PBYTE FindPatternLight(std::vector<BYTE> pattern, LPCSTR moduleName)
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

void MakeBin(PBYTE buff, SIZE_T sz, LPCSTR name)
{
	DWORD old = 0;
	if (VirtualProtect(buff, sz, PAGE_EXECUTE_READWRITE, &old)) {
		std::ofstream f(name, std::ios::binary);

		for (SIZE_T i = 0; i < sz; i++)
		{
			f.write((char*)&buff[i], sizeof(unsigned char));
		}
		f.close();
		DWORD __temp;
		VirtualProtect(buff, sz, old, &__temp);
	}
	
}