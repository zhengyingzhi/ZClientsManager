#pragma once

#include <string>
using namespace std;

#define ZAPP_CONFIG_NAME		"ZAppConfig.txt"
#define ZAPP_CONFIG_DELIMITER	'='
#define ZAPP_CONFIG_COMMENT		'#'
#define ZAPP_CONFIG_LINEFEED	"\r\n"


class ZAppConfigs
{
public:
	ZAppConfigs();
	~ZAppConfigs();

	bool ReadAppConfig(const string& aConfName);

	void WriteAppConfig(const string& aConfName);

public:
	void SetUserID(const string& aUserID);
	void SetPassword(const string& aPassword);
	void SetSavePasswd(uint32_t aSavePasswd);

public:
	string ToString();

public:
	char		m_UserID[24];
	char		m_Password[24];
	uint32_t	m_SavePasswd;		// 记住密码

	uint16_t	m_NetType;			// 1:tcp 2:udp-multicast 3:udp-broadcast
	uint16_t	m_MainPort;			// 主端口
	char		m_CastIP[16];		// 组播/广播/服务器IP
};
