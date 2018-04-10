#pragma once

#include <string>

#include "UserInfo.h"
#include "StudentInfo.h"


class ZInfoEncrypt
{
public:
	static std::string Encrypt(int aEncryptMethod, const char* key, const char* apRawData, int aRawLength);

	static std::string DeEncrypt(int aEncryptMethod, const char* key, const char* apEncryptData, int aEncryptLength);
};
