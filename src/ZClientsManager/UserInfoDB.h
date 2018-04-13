#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZToolLib/ztl_shm.h>

#include "UserInfo.h"
#include "ZDataBase.h"

#define USERINFO_DB_DEFAULT_NAME    "UserInfo.db"
#define USERINFO_DB_DEFAULT_SIZE    (256 * 1024)
#define USERINFO_DB_DEFAULT_QRYN    512
#define USERINFO_DB_ALIGNMENT       512


bool ZQueryCompareUserName(const void* apExpect, const void* apAcutal, int aExtend);

class ZUserInfoDBText : public ZDataBase
{
public:
	ZUserInfoDBText();
	virtual ~ZUserInfoDBText();

	virtual int Open(const std::string& aDBName, const std::string& ip, uint16_t port);
	virtual int Close();

	virtual int Insert(void* apDataInfo, uint32_t aDataSize);
	virtual int Update(void* apDataInfo, uint32_t aDataSize);
	virtual int Delete(void* apDataInfo, uint32_t aDataSize);

	virtual ZQueryResult* Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend);

protected:
	ZUserInfo* NextUserInfo(ZUserInfo* apCurStuInfo, bool aAutoExpand = false);
	ZUserInfo* GetAvailUserInfo();

protected:
	char*       m_pBuffer;
	uint32_t    m_BufSize;

	ztl_shm_t*  m_pShmObj;
};

