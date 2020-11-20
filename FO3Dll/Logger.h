#pragma once
#include <FO3\FO3Dll\framework.h>

namespace Logger
{
    void Clear();
    void Add(const char* fmt, ...);
    void Draw(const char* title);
};