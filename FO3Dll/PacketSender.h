#pragma once
#include <string>
#include <vector>
#include "framework.h"
#include "FO3Funcs.h"

enum SendType {
	Int = 1,
	Short = 2,
	Char = 3
};

struct ToSend {
	SendType Sendtype;
	union {
		unsigned int Int;
		unsigned short Short;
		unsigned char Char;
	};

	std::string ToString() const noexcept{
		switch (Sendtype)
		{
		case SendType::Int:
			return "Type [Int] value [" + std::to_string(Int) + "]";
		case SendType::Short:
			return "Type [Short] value [" + std::to_string(Short) + "]";
		default:
			return "Type [Char] value [" + std::to_string(Char) + "]";
		}
	}
};


class PacketSender {
public:
	using Batch = std::vector<std::vector<ToSend>>;
private:
	std::string filename_;
	Batch toSend_;
	void WhatToDo(const std::string& sendtype, const std::string& body) noexcept;
public:
	PacketSender(const std::string& filename) : filename_(filename){}
	PacketSender(const char* filename) : filename_(filename) {}

	Batch BatchFromText(const std::string& text) noexcept;
	Batch Create() noexcept;
	Batch GetToSend() const noexcept { return toSend_; };
};


class JustSender {
private:
	static const int maxLength_ = 65536;
	char buf_[maxLength_];
	PBYTE foclient_;
	void PushUint(uint value) const noexcept;
	void PushUchar(uchar value)  const noexcept;
	void PushUshort(ushort value)  const noexcept;
	void SendByOne(const std::vector<ToSend>& toSend) const noexcept;
public:
	void Init() { 
		std::string tmp = "[out]{\n\tint = 0xBD5A6DAA;\n\tchar = 0x1;\n\tshort = 0x1;\n}";
		memcpy(buf_, tmp.c_str(), tmp.size());
		foclient_ = (PBYTE)(*(PDWORD)GET_ADR(FO_CLIENT_ADR));
	}
	void SendFromFile(const std::string& filename = "packets.txt") const noexcept;
	void SendFromText(const std::string& text) const noexcept;
	void Draw() const noexcept;
};