#pragma once
#include "HookSetter.h"
#include "framework.h"
#include "TextDrawer.h"

//#pragma comment(lib, "d3d9")
//#pragma comment(lib, "d3dx9")

namespace D3D
{
	void SetOutputText(const std::string& str);
	void SetOutputText(const std::string&& str);
	void SetOutputText(const char* str);
	void Constructor(LPCSTR wndName, bool* NeedMove1Hex);
	void Destructor();
}

