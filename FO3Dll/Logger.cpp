#include "Logger.h"
#include "ImGui/imgui.h"

ImGuiTextBuffer _bufferIn, _bufferOut;
bool _scrollToBottomIn = false, _scrollToBottomOut = false;
void Logger::ClearIn()
{ 
    _bufferIn.clear(); 
}

void Logger::ClearOut()
{
    _bufferOut.clear();
}

void Logger::AddIn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _bufferIn.appendfv(fmt, args);
    va_end(args);
    _scrollToBottomIn = true;
}

void Logger::AddOut(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _bufferOut.appendfv(fmt, args);
    va_end(args);
    _scrollToBottomOut = true;
}



void Logger::Draw(const char* title, HaxSettings* haxSettings)
{
    if (ImGui::Begin(title, &haxSettings->OpenPackets)) {
        auto size = ImGui::GetWindowSize();
        if (ImGui::BeginChild("InPackets", ImVec2(size.x / 2 - 10, 0), true)) {
            ImGui::Text("# In packets");

            // start button
            if (ImGui::Button("Start") && !haxSettings->StartInPackets)
                haxSettings->StartInPackets = true;
            ImGui::SameLine();
            // stop button
            if (ImGui::Button("Stop") && haxSettings->StartInPackets)
                haxSettings->StartInPackets = false;
            ImGui::SameLine();
            ImGui::TextColored(
                haxSettings->StartInPackets ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1),
                haxSettings->StartInPackets 
                ? "IN PROCESS    " 
                : "STOPPED       ");
            ImGui::SameLine();
            if (ImGui::Button("Clear"))
                Logger::ClearIn();


            ImGui::Separator();
            ImGui::TextUnformatted(_bufferIn.begin());
            if (_scrollToBottomIn) {
                ImGui::SetScrollHere(1.0f);
                _scrollToBottomIn = false;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        if (ImGui::BeginChild("OutPackets", ImVec2(size.x / 2 - 15, 0), true)) {
            ImGui::Text("# Out packets");

            // start button
            if (ImGui::Button("Start") && !haxSettings->StartOutPackets)
                haxSettings->StartOutPackets = true;
            ImGui::SameLine();
            // stop button
            if (ImGui::Button("Stop") && haxSettings->StartOutPackets)
                haxSettings->StartOutPackets = false;
            ImGui::SameLine();
            ImGui::TextColored(
                haxSettings->StartOutPackets ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1),
                haxSettings->StartOutPackets
                ? "IN PROCESS    "
                : "STOPPED       ");
            ImGui::SameLine();
            if (ImGui::Button("Clear"))
                Logger::ClearOut();

            ImGui::Separator();
            ImGui::TextUnformatted(_bufferOut.begin());
            if (_scrollToBottomOut) {
                ImGui::SetScrollHere(1.0f);
                _scrollToBottomOut = false;
            }
            ImGui::EndChild();
        }
    }    
    ImGui::End();
}
