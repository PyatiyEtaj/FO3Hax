#pragma once
#include <FO3\FO3Dll\framework.h>

namespace Logger
{
    void ClearIn();
    void ClearOut();
    void AddIn(const char* fmt, ...);
    void AddOut(const char* fmt, ...);
    void Draw(const char* title, HaxSettings* haxSettings);
};