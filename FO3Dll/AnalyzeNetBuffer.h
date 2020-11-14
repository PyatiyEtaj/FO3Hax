#pragma once
#include "framework.h"

namespace AnalyzeNetBuffer {
    void SetOutputDebugMessages(bool flag);
    void InitializationOfAnalyzer(HaxSettings* haxSettings);
    void Unset();
    void SendMsg();
};