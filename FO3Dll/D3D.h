#pragma once
#include "HookSetter.h"
#include "framework.h"
#include "TextDrawer.h"

namespace D3D
{
	void SetOutputText(const std::string& str);
	void SetOutputText(const std::string&& str);
	void SetOutputText(const char* str);
	void Constructor(LPCSTR wndName);
	void Destructor();
}

