#include <assert.h>

#include <stdio.h>

#include <ZToolLib/ztl_config.h>

#include "ZAppConfig.h"

#include "ZNetwork.h"


ZAppConfigs::ZAppConfigs()
	: m_UserID()
	, m_Password()
	, m_SavePasswd()
	, m_NetType(2)
	, m_MainPort(ZNET_DEFAULT_PORT)
	, m_CastIP(ZNET_DEFAULT_GROUPIP)
{}

ZAppConfigs::~ZAppConfigs()
{
}

bool ZAppConfigs::ReadAppConfig(const string& aConfName)
{
	ztl_config_t* zconf;
	zconf = ztl_config_open(aConfName.c_str(), ZAPP_CONFIG_COMMENT, ZAPP_CONFIG_DELIMITER);
	if (!zconf)
	{
		return false;
	}

	int lLength = 0;
	char* lpReadValue = nullptr;
	if (ztl_config_read_str(zconf, "UserID", &lpReadValue, &lLength))
	{
		strncpy(m_UserID, lpReadValue, sizeof(m_UserID) - 1);
	}

	if (ztl_config_read_str(zconf, "Password", &lpReadValue, &lLength))
	{
		strncpy(m_Password, lpReadValue, sizeof(m_Password) - 1);
	}

	ztl_config_read_int32(zconf, "SavePassword", &m_SavePasswd);
	ztl_config_read_int16(zconf, "NetType", &m_NetType);
	ztl_config_read_int16(zconf, "MainPort", &m_MainPort);

	if (ztl_config_read_str(zconf, "CastIP", &lpReadValue, &lLength))
	{
		strncpy(m_CastIP, lpReadValue, sizeof(m_CastIP) - 1);
	}

	ztl_config_close(zconf);

	return true;
}

void ZAppConfigs::WriteAppConfig(const string& aConfName)
{
	assert(!aConfName.empty());
	string lString;
	lString = ToString();

	FILE* fp = fopen(aConfName.c_str(), "w");
	if (fp)
	{
		fputs(lString.c_str(), fp);
		fflush(fp);
		fclose(fp);

		OutputDebugString("ZAppConfig WriteAppConfig()");
	}
}

void ZAppConfigs::SetUserID(const string& aUserID)
{
	strncpy(m_UserID, aUserID.c_str(), sizeof(m_UserID) - 1);
	m_UserID[aUserID.length()] = '\0';
}

void ZAppConfigs::SetPassword(const string& aPassword)
{
	strncpy(m_Password, aPassword.c_str(), sizeof(m_Password) - 1);
	m_Password[aPassword.length()] = '\0';
}

void ZAppConfigs::SetSavePasswd(uint32_t aSavePasswd)
{
	m_SavePasswd = aSavePasswd;
}


string ZAppConfigs::ToString()
{
	const char* lpLineFeed = "\r\n";

	int  lLength = 0;
	char lBuffer[1000] = "";

	lLength += sprintf(lBuffer + lLength, "UserID%c%s" ZAPP_CONFIG_LINEFEED, ZAPP_CONFIG_DELIMITER, m_UserID);
	lLength += sprintf(lBuffer + lLength, "Password%c%s" ZAPP_CONFIG_LINEFEED, ZAPP_CONFIG_DELIMITER, m_UserID);
	lLength += sprintf(lBuffer + lLength, "SavePasswd%c%d" ZAPP_CONFIG_LINEFEED, ZAPP_CONFIG_DELIMITER, m_SavePasswd);
	lLength += sprintf(lBuffer + lLength, "NetType%c%d" ZAPP_CONFIG_LINEFEED, ZAPP_CONFIG_DELIMITER, m_NetType);
	lLength += sprintf(lBuffer + lLength, "MainPort%c%d" ZAPP_CONFIG_LINEFEED, ZAPP_CONFIG_DELIMITER, m_MainPort);
	lLength += sprintf(lBuffer + lLength, "CastIP%c%s" ZAPP_CONFIG_LINEFEED, ZAPP_CONFIG_DELIMITER, m_CastIP);

	lBuffer[lLength] = '\0';

	return string(lBuffer);
}

