#pragma once
#include "httplib.h"
#include <iostream>
#include "json.hpp"


struct Offset {
public:
	std::string Name;
	unsigned int Value;
	Offset(std::string&& name, std::string&& val):
		Name(name), Value(std::stoul(val, nullptr, 16))
	{}

}; 

struct User {
public:
	std::string Login;
	std::string Pass ;

	User(const char* login, const char* pass) :
		Login(login), Pass(pass)
	{}

	std::string ToJson() {
		char key = rand() % 127 + 1;
		std::string tojson;
		tojson.push_back(key);
		tojson += xorstr("{\"login\":\"") + Login + xorstr("\", \"pass\":\"") + Pass + "\"}";
		tojson.push_back(1);
		for (size_t i = 1; i < tojson.size()-1; i++)
		{
			tojson[i] ^= key;
		}
		return tojson;
	}

};

class Fo3Net {
private:
	using json = nlohmann::json;
	httplib::Client cli_;
public:
	Fo3Net(const char* url) :
		cli_(httplib::Client(url, 80))
	{}

	std::vector<Offset> Getoffsets() {
		std::vector<Offset> vec;
		auto res = cli_.Get(xorstr("/offs"));
		if (res) {
			if (res->status == 200) {
				//std::cout << res->get_header_value("Content-Type") << std::endl;
				json j = json::parse(res->body);
				for (auto item : j)
					vec.push_back(
						Offset(item["name"].get<std::string>(), item["value"].get<std::string>())
					);
			}
			//else
			//	std::cout << "status[" << res->status << "] msg[" << res->body << "]\n";
		}
		//else {
		//	std::cout << "error code: " << res.error() << std::endl;
		//}
		return vec;
	}


	bool Postloginpass(User user) {
		std::vector<Offset> vec;
		httplib::Headers hdr;
		//std::cout << "\nUserData:" <<user.ToJson() << std::endl;
		auto res = cli_.Post(xorstr("/127dcng172cdgn126ngcd812gdc812cd12dc12cdlj1"), hdr, user.ToJson(), xorstr("text/plain")/*xorstr("application/json; charset=utf-8")*/);
		/*if (res) {
			std::cout << "status[" << res->status << "] msg[" << res->body << "]\n";
		}
		else {
			std::cout << "error code: " << res.error() << std::endl;
		}*/
		return res->status == 200;
	}

};