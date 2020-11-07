#pragma once

namespace AnalyzeNetBuffer {
    void SetOutputDebugMessages(bool flag);
    void InitializationOfAnalyzer(bool* input, bool* output);
    void Unset();
    void SendMsg();
};