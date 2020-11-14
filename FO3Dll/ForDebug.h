#pragma once
#include "framework.h"
#include <sstream>
#include <iomanip>
#include <string>

namespace ForDebug
{
	HMODULE GetModulesNames(std::string moduleName = "NULL");
	bool CrtConsole();
	void FreeCons();
	void PrinterBYTE(PBYTE b, int length);
}
