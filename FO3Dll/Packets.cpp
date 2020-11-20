#include "Packets.h"
#include "ImGui/imgui.h"

ImGuiTextBuffer _bufferIn, _bufferOut;
bool _scrollToBottomIn = false, _scrollToBottomOut = false;
void Packets::ClearIn()
{ 
    _bufferIn.clear(); 
}

void Packets::ClearOut()
{
    _bufferOut.clear();
}

void Packets::AddIn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _bufferIn.appendfv(fmt, args);
    va_end(args);
    _scrollToBottomIn = true;
}

void Packets::AddOut(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _bufferOut.appendfv(fmt, args);
    va_end(args);
    _scrollToBottomOut = true;
}

extern HaxSettings g_HaxSettings;

void Packets::Draw(const char* title)
{
    if (ImGui::Begin(title, &g_HaxSettings.OpenPackets)) {
        auto size = ImGui::GetWindowSize();
        if (ImGui::BeginChild("InPackets", ImVec2(size.x / 2 - 10, 0), true)) {
            ImGui::Text("# In packets");

            // start button
            if (ImGui::Button("Start") && !g_HaxSettings.StartInPackets)
                g_HaxSettings.StartInPackets = true;
            ImGui::SameLine();
            // stop button
            if (ImGui::Button("Stop") && g_HaxSettings.StartInPackets)
                g_HaxSettings.StartInPackets = false;
            ImGui::SameLine();
            ImGui::TextColored(
                g_HaxSettings.StartInPackets ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1),
                g_HaxSettings.StartInPackets 
                ? "IN PROCESS    " 
                : "STOPPED       ");
            ImGui::SameLine();
            if (ImGui::Button("Clear"))
                Packets::ClearIn();


            //ImGui::Separator();
            //ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(g_HaxSettings.Colors[0]*255, g_HaxSettings.Colors[1] * 255, g_HaxSettings.Colors[2] * 255, 100));
            //ImGui::BeginChild("InPacketsText", ImVec2(0.0, 0.0), true, ImGuiWindowFlags_HorizontalScrollbar);
            //ImGui::TextUnformatted(_bufferIn.begin());
            ImGui::InputTextMultiline("[in] packets", (char*)_bufferIn.begin(), _bufferIn.size(), ImVec2(size.x / 2 - 15, size.y - 100), ImGuiInputTextFlags_ReadOnly);
            if (_scrollToBottomIn) {
                ImGui::SetScrollHere(1.0f);
                _scrollToBottomIn = false;
            }
            //ImGui::EndChild();
            //ImGui::PopStyleColor();

            ImGui::EndChild();
        }
        ImGui::SameLine();
        if (ImGui::BeginChild("OutPackets", ImVec2(size.x / 2 - 15, 0), true)) {
            ImGui::Text("# Out packets");

            // start button
            if (ImGui::Button("Start") && !g_HaxSettings.StartOutPackets)
                g_HaxSettings.StartOutPackets = true;
            ImGui::SameLine();
            // stop button
            if (ImGui::Button("Stop") && g_HaxSettings.StartOutPackets)
                g_HaxSettings.StartOutPackets = false;
            ImGui::SameLine();
            ImGui::TextColored(
                g_HaxSettings.StartOutPackets ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1),
                g_HaxSettings.StartOutPackets
                ? "IN PROCESS    "
                : "STOPPED       ");
            ImGui::SameLine();
            if (ImGui::Button("Clear"))
                Packets::ClearOut();

            //ImGui::Separator();
            //ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(g_HaxSettings.Colors[0] * 255, g_HaxSettings.Colors[1] * 255, g_HaxSettings.Colors[2] * 255, 100));
            //ImGui::BeginChild("OutPacketsText", ImVec2(0.0, 0.0), true, ImGuiWindowFlags_HorizontalScrollbar);
            //ImGui::LogText(_bufferOut.begin());
            //ImGui::TextUnformatted(_bufferOut.begin());
            ImGui::InputTextMultiline("[out] packets", (char*)_bufferOut.begin(), _bufferOut.size(), ImVec2(size.x / 2 - 15, size.y-100), ImGuiInputTextFlags_ReadOnly);
            if (_scrollToBottomOut) {
                ImGui::SetScrollHere(1.0f);
                _scrollToBottomOut = false;
            }
            //ImGui::EndChild();
            //ImGui::PopStyleColor();

            ImGui::EndChild();
        }
    }    
    ImGui::End();
}
