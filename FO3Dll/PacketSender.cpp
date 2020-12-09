#include "PacketSender.h"
#include "ImGui/imgui.h"
#include "Logger.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <sstream>

std::string ReadToEnd(const std::string& filename) {
	std::ifstream f(filename);
	std::string text, tmp;
	while (f >> tmp) {
		text += tmp + "\n";
	}
	f.close();
	return text;
}

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

std::vector<std::string> Split(const std::string& s, char delimiter, bool needtrim = false)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		if (needtrim) trim(token);
		tokens.push_back(token);
	}
	return tokens;
}


unsigned int ToUint(const std::string& s){
	if (s.size() >= 2) {
		if (s[0] == '0' && s[1] == 'x')
			return std::stoul(s, nullptr, 16);
	}
	return std::stoul(s, nullptr, 10);
};

std::vector<ToSend> CreatePackets(const std::string& body) {
	std::vector<ToSend> tosend;
	for (auto line : Split(body, ';')) {
		if (line.size() >= 4) {
			auto data = Split(line, '=', true);
			if (data.size() == 2) {
				ToSend ts;
				if (data[0] == "int") {
					ts.Sendtype = SendType::Int;
					ts.Int = (unsigned int)(ToUint(data[1]));
				}
				else if (data[0] == "short") {
					ts.Sendtype = SendType::Short;
					ts.Short = (unsigned short)(ToUint(data[1]));
				}
				else{
					ts.Sendtype = SendType::Char;
					ts.Char = (unsigned char)(ToUint(data[1]));
				}
				tosend.push_back(ts);
			}
		}
	}
	return tosend;
}

void PacketSender::WhatToDo(const std::string& sendtype, const std::string& body) noexcept {
	/*if (sendtype == "in") {
		get_to_recv
	}
	else if (sendtype == "out") {
		get_to_send
	}*/
	toSend_.push_back(CreatePackets(body));
}

PacketSender::Batch PacketSender::BatchFromText(const std::string &text) noexcept {
	std::regex re(xorstr("\\[(\\w+)\\]\\{(\[^\\}\]*)\\}"),
		std::regex_constants::ECMAScript | std::regex_constants::icase);
	auto words_begin =
		std::sregex_iterator(text.begin(), text.end(), re);
	auto words_end = std::sregex_iterator();
	std::vector<ToSend> result;
	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		std::smatch match = *i;
		std::string match_str = match[2].str();
		WhatToDo(match[1].str(), match[2].str());
	}
	return toSend_;
}

PacketSender::Batch PacketSender::Create() noexcept
{
	auto text = ReadToEnd(filename_);
	return BatchFromText(text);
}

void JustSender::SendByOne(const std::vector<ToSend>& toSend) const noexcept
{
	for (auto packet : toSend)
	{
		if (packet.Sendtype == SendType::Int)
			PushUint(packet.Int);
		else if (packet.Sendtype == SendType::Short)
			PushUshort(packet.Short);
		else if (packet.Sendtype == SendType::Char)
			PushUchar(packet.Char);
	}
	Sleep(100);
}

void JustSender::PushUint(uint value) const noexcept {
	if (foclient_)
		NetBufferPushUint(foclient_, value);
}
void JustSender::PushUchar(uchar value)  const noexcept {
	if (foclient_)
		NetBufferPushUchar(foclient_, value);
}
void JustSender::PushUshort(ushort value)  const noexcept {
	if (foclient_)
		NetBufferPushUshort(foclient_, value);
}

void JustSender::SendFromFile(const std::string& filename) const noexcept
{
	PacketSender ps("packets.txt");
	ps.Create();
	for (auto batch : ps.GetToSend()) {
		SendByOne(batch);
	}
}

void JustSender::SendFromText(const std::string& text) const noexcept
{
	PacketSender ps("null");	
	for (auto batch : ps.BatchFromText(text)) {
		SendByOne(batch);
	}
}

extern HaxSettings g_HaxSettings;

void JustSender::Draw() const noexcept
{
	if (ImGui::Begin("Sender", &g_HaxSettings.OpenSendPackets)) {
		auto size = ImGui::GetWindowSize();
		if (ImGui::Button("Send"))
			SendFromText(buf_);
		ImGui::InputTextMultiline("", const_cast<char*>(buf_), maxLength_, ImVec2(size.x - 15, size.y - 100), ImGuiInputTextFlags_AllowTabInput);
	}
	ImGui::End();
}
