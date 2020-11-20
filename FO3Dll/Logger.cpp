#include "Logger.h"
#include "ImGui/imgui.h"

ImGuiTextBuffer _buffer;
bool _scrollToBottom = false;
extern HaxSettings g_HaxSettings;

void Logger::Clear()
{
    _buffer.clear();
}

void Logger::Add(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _buffer.appendfv(fmt, args);
    va_end(args);
    _scrollToBottom = true;
}

void Logger::Draw(const char* title)
{
    if (ImGui::Begin(title, &g_HaxSettings.OpenLog)) {
        auto size = ImGui::GetWindowSize();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "LOG    ");
        ImGui::SameLine();
        if (ImGui::Button("Clear"))
            Logger::Clear();
        if (ImGui::BeginChild("LogText", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
            ImGui::TextUnformatted(_buffer.begin());
            ImGui::EndChild();
        }
    }
    ImGui::End();
}
