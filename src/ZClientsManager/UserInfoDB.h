#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZToolLib/ztl_shm.h>

#include "UserInfo.h"

#define USERINFO_DB_DEFAULT_NAME    "UserInfo.db"
#define USERINFO_DB_DEFAULT_SIZE    (256 * 1024)
#define USERINFO_DB_DEFAULT_QRYN    512
#define USERINFO_DB_ALIGNMENT       512

typedef struct stUserQueryResult
{
	void(*Cleanup)(struct stUserQueryResult* apRS);
	uint32_t    m_AllocedN;
	uint32_t    m_Count;
}ZUserQueryResult;
#define ZUSER_QUERY_RS_BODY(r)      (ZUserInfo*)(r + 1)


class ZUserInfoDB
{
public:
	virtual ~ZUserInfoDB(){}

	virtual int Open(const char* apName, const char* ip, uint16_t port) = 0;
	virtual int Close() = 0;

	/// insert data into data
	virtual int Insert(ZUserInfo* apUserInfo) = 0;

	virtual int Remove(ZUserInfo* apUserInfo) = 0;

	/// query by user id
	virtual ZUserQueryResult* Query(const char* apUserID, bool aIncludeDeleted) = 0;
};


class ZUserInfoDBText : public ZUserInfoDB
{
public:
	ZUserInfoDBText();
	virtual ~ZUserInfoDBText();

	virtual int Open(const char* apName, const char* ip, uint16_t port);
	virtual int Close();

	/// insert data into data
	virtual int Insert(ZUserInfo* apUserInfo);

	virtual int Remove(ZUserInfo* apUserInfo);

	/// query by user id
	virtual ZUserQueryResult* Query(const char* apUserID, bool aIncludeDeleted);

protected:
	char*       m_pBuffer;
	uint32_t    m_BufSize;

	ztl_shm_t*  m_pShmObj;

	ZUserQueryResult* m_pResult;
};

