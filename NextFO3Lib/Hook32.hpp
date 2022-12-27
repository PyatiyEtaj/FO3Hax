#pragma once

#include <iostream>
#include <Windows.h>

namespace HooksUtil
{
	bool static DetourFunc(PBYTE adrOld, PBYTE adrNew, bool patchByE9Jmp = false)
	{
		if (!adrOld) return false;
		DWORD off = (DWORD)adrNew - (DWORD)adrOld - 5;
		PBYTE bOff = (PBYTE)&off;
		if (patchByE9Jmp) memcpy_s(adrOld, 1, "\xE9", 1);
		memcpy_s((void*)(adrOld + 1), 4, bOff, 4);
		return true;
	}

	template<typename T, typename ...A>
	T CallFunction(void* functionPtr, A ...args) {
		constexpr auto call_void = std::is_same_v<T, void>;
		using function = T(__cdecl*)(A...);
		const auto orig = static_cast<function>(functionPtr);
		if constexpr (call_void)
		{
			orig(args...);
		}
		else
		{
			return orig(args...);
		}

		return (T)0;
	}
	
	template<typename T, typename ...A>
		T CallFunctionStdCall(void* functionPtr, A ...args) {
		constexpr auto call_void = std::is_same_v<T, void>;
		using function = T(__stdcall*)(A...);
		const auto orig = static_cast<function>(functionPtr);
		if constexpr (call_void)
		{
			orig(args...);
		}
		else
		{
			return orig(args...);
		}

		return (T)0;
	}

	template<typename T, typename ...A>
	T CallFunctionThisCall(void* functionPtr, void* thisPtr, A ...args) {
		constexpr auto call_void = std::is_same_v<T, void>;
		using function = T(__fastcall*)(void*, void*, A...);
		const auto orig = static_cast<function>(functionPtr);
		if constexpr (call_void)
		{
			orig(thisPtr, 0, args...);
		}
		else
		{
			return orig(thisPtr, 0, args...);
		}

		return (T)0;
	}
}

namespace Hooks {
	constexpr DWORD Hooks_Hooks32_BYTES_BACKUP = 24;
	constexpr DWORD Hooks_Hooks32_SAFE_BYTES_MIN = 1 + sizeof(DWORD);
	constexpr DWORD Hooks_Hooks32_SAFE_BYTES_MAX = Hooks_Hooks32_BYTES_BACKUP - Hooks_Hooks32_SAFE_BYTES_MIN;

	class Hook32
	{
	private:
		/*
		* jmp relative_adress_to_function;
		*/
		const DWORD SIZE_OF_INSTRUCTION = Hooks_Hooks32_SAFE_BYTES_MIN;

		PBYTE _originalCode = nullptr;
		PBYTE _myCode = nullptr;
		PBYTE _functionToHook = nullptr;
		PBYTE _startOriginalFunction = nullptr;
		PBYTE _newFunction = 0;

		DWORD _countOfSafeByte = Hooks_Hooks32_SAFE_BYTES_MIN;
		bool _isSetted = false;
		bool _unsetOnDie = false;

	public:
		Hook32(bool unsetOnDie)
			: _unsetOnDie(unsetOnDie)
		{}

		~Hook32()
		{
			if (_unsetOnDie)
			{
				Unset();
			}
		}

		bool Set(PBYTE functionToHook, PBYTE newFunction, DWORD countOfSafeByte = Hooks_Hooks32_SAFE_BYTES_MIN)
		{
			CheckCountOfSafeBytes(countOfSafeByte);

			if (!_isSetted)
			{
				Init(functionToHook, newFunction, countOfSafeByte);
				if (SetHook(_newFunction, _functionToHook)) {
					_startOriginalFunction = CreateJmpToOriginalFunction(_countOfSafeByte, _functionToHook, _originalCode);
					_isSetted = true;

					return true;
				}
			}

			return false;
		}

		void Unset()
		{
			if (_isSetted)
			{
				if (_originalCode != nullptr && _functionToHook != nullptr)
				{
					memcpy_s(_functionToHook, _countOfSafeByte, _originalCode, _countOfSafeByte);
				}

				if (_originalCode != nullptr)
				{
					free(_originalCode);
					_originalCode = nullptr;
				}

				if (_startOriginalFunction != nullptr)
				{
					free(_startOriginalFunction);
					_startOriginalFunction = nullptr;
				}

				if (_myCode != nullptr)
				{
					free(_myCode);
					_myCode = nullptr;
				}

				_isSetted = false;
			}
		}

		PBYTE GetStartOfOriginalFunction()
		{
			return _startOriginalFunction;
		}


	private:
		void CheckCountOfSafeBytes(DWORD countOfSafeBytes)
		{
			if (countOfSafeBytes > Hooks_Hooks32_SAFE_BYTES_MAX || countOfSafeBytes < Hooks_Hooks32_SAFE_BYTES_MIN)
				throw std::exception("Count of safe bytes issue");
		}

		void Init(PBYTE functionToHook, PBYTE newFunction, DWORD countOfSafeByte)
		{
			_originalCode = (PBYTE)calloc(countOfSafeByte, sizeof(BYTE));
			_myCode = (PBYTE)calloc(countOfSafeByte, sizeof(BYTE));
			_functionToHook = functionToHook;
			_newFunction = newFunction;
			_countOfSafeByte = countOfSafeByte;
			_isSetted = false;
		}

		PBYTE CreateJmpToOriginalFunction(DWORD safe, PBYTE adr, BYTE* originalCode)
		{
			PBYTE changedCode = (PBYTE)malloc(Hooks_Hooks32_BYTES_BACKUP);
			if (changedCode)
			{
				ZeroMemory(changedCode, Hooks_Hooks32_BYTES_BACKUP);
				memcpy_s(changedCode, safe, originalCode, safe);
				constexpr BYTE jmp[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
				DWORD relative = (DWORD)(adr + safe) - (DWORD)(changedCode + safe) - sizeof(jmp);
				*(PDWORD)(jmp + 1) = relative;
				memcpy_s(changedCode + safe, sizeof(jmp), jmp, sizeof(jmp));

				DWORD temp;
				VirtualProtect(changedCode, Hooks_Hooks32_BYTES_BACKUP, PAGE_EXECUTE_READWRITE, &temp);
			}
			return changedCode;
		}

		bool SetHook(PBYTE newFunction, PBYTE functionToHook)
		{
			DWORD oldProtectDip = 0;
			if (VirtualProtect((PBYTE)functionToHook, 8, PAGE_EXECUTE_READWRITE, &oldProtectDip))
			{
				memcpy_s(_originalCode, _countOfSafeByte, functionToHook, _countOfSafeByte);
				constexpr BYTE jmp[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
				DWORD relative = (DWORD)newFunction - (DWORD)functionToHook - sizeof(jmp);
				*(PDWORD)(jmp + 1) = relative;
				memcpy_s(functionToHook, sizeof(jmp), jmp, sizeof(jmp));
				return true;
			}

			return false;
		}
	};
}