#include "ZInfoEncrypt.h"

static char* ZSimpleEncrypt(char* apData)
{
	unsigned char *p;
	unsigned char uch, umask, index = 0;
	p = (unsigned char *)apData;
	while ((uch = *p))
	{
		umask = 0xFF - index;
		*p++ = uch  ^ umask;
		index++;

		if (index > 8)
			index = 0;
	}

	return apData;
}

std::string ZInfoEncrypt::Encrypt(int aEncryptMethod, const char* key, const char* apRawData, int aRawLength)
{
	return std::string(apRawData);
}

std::string ZInfoEncrypt::DeEncrypt(int aEncryptMethod, const char* key, const char* apEncryptData, int aEncryptLength)
{
	return std::string(apEncryptData);
}
