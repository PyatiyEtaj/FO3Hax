#pragma once
#include "framework.h"

class UserData {
private :
	PBYTE  login_;
	PBYTE  pass_;
	std::string lastLogin_;
	std::string lastPass_;
	bool changedLogin_ = false;
	bool changedPass_ = false;
public:
	void Init(bool isFo3);
	bool Check();
	const char* GetLogin();
	const char* GetPass();
};