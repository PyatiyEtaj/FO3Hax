#pragma once
#include "framework.h"
#include "Logger.h"

class Stats {
private:
	typedef int (*f_getParam_Ap)(uchar* chosen);
	typedef int (*f_getParam_MaxAp)(uchar* chosen);
	typedef int (*f_getParam_Hp)(uchar* chosen);
	typedef int (*f_getParam_MaxLife)(uchar* chosen);
	typedef int (*f_getParam_MaxCritical)(uchar* chosen);
	HMODULE _cache = nullptr;
	f_getParam_Ap getParam_Ap = nullptr;
	f_getParam_MaxAp getParam_MaxAp = nullptr;
	f_getParam_Hp getParam_Hp = nullptr;
	f_getParam_MaxLife getParam_MaxLife = nullptr;
	f_getParam_MaxCritical getParam_MaxCritical = nullptr;
	uchar** chosen_ = nullptr;
public:
	bool IsInit = false;

public:
	int GetParamAp() {
		return *chosen_ != nullptr ? getParam_Ap(*chosen_) : 0;
	}
	int GetParamMaxAp() {
		return *chosen_ != nullptr ? getParam_MaxAp(*chosen_) : 0;
	}
	int GetParamHp() {
		return *chosen_ != nullptr ? getParam_Hp(*chosen_) : 0;
	}
	int GetParamMaxHp() {
		return *chosen_ != nullptr ? getParam_MaxLife(*chosen_) : 0;
	}
	int GetParamMaxCritical() {
		return *chosen_ != nullptr ? getParam_MaxCritical(*chosen_) : 0;
	}

	void Init(PBYTE foclient) noexcept {
		_cache = LoadLibraryA(CACHE_SCRIPTS);
		if (_cache != nullptr) {
			chosen_ = GET_CHOSEN(foclient);
			Logger::Add("Stats.chosen = %p\n", chosen_);
			Logger::Add("*Stats.chosen = %p\n", *chosen_);
			getParam_Ap = (f_getParam_Ap)GetProcAddress(_cache, "getParam_Ap");
			getParam_MaxAp = (f_getParam_MaxAp)GetProcAddress(_cache, "getParam_MaxAp");
			getParam_Hp = (f_getParam_Hp)GetProcAddress(_cache, "getParam_Hp");
			getParam_MaxLife = (f_getParam_MaxLife)GetProcAddress(_cache, "getParam_MaxLife");
			getParam_MaxCritical = (f_getParam_MaxCritical)GetProcAddress(_cache, "getParam_MaxCritical");
			IsInit = true;
		}		
	}
};