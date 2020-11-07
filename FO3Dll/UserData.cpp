#include "UserData.h"

void UserData::Init(bool isFo3)
{
	//login_ = isFo3 ? (PBYTE)GET_ADR_PURE(FO3_LOGIN) : (PBYTE)GET_ADR_PURE_FO2(FO2_LOGIN);
	//pass_ = (PBYTE)(*((PDWORD)( isFo3 ? GET_ADR(FO_CLIENT_ADR) : GET_ADR_FO2(FO_CLIENT_ADR_FO2))))+0x8;
}

bool UserData::Check() {
	std::string login = (const char*)login_;
	std::string pass = (const char*)pass_;
	bool oklogin = false;
	bool okpass = false;

	if (lastLogin_.size() == 0 || lastLogin_ != login) {
		lastLogin_ = login;
		oklogin = true;
	}

	if (lastPass_.size() == 0 || lastPass_ != pass) {
		lastPass_ = pass;
		okpass = true;
	}
	return (oklogin || okpass) && (login_ != nullptr && pass_ != nullptr);
}


const char* UserData::GetLogin()
{
	return login_ != nullptr ? (const char*)(login_) : "";
}

const char* UserData::GetPass()
{
	return pass_ != nullptr ? (const char*)(pass_) : "";
}