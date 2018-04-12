#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZToolLib/ztl_shm.h>

#include "StudentInfo.h"
#include "ZDBCommon.h"


#define STUINFO_DB_DEFAULT_NAME     "StudentInfo.db"
#define STUINFO_DB_DEFAULT_SIZE     (4 * 1024 * 1024)
#define STUINFO_DB_DEFAULT_QRYN     512
#define STUINFO_DB_ALIGNMENT        2048

typedef struct 
{
	const char* m_pName;
	const char* m_pTelephone;
}ZQueryCondition;


typedef enum
{
	CC_Equal = 0,
	CC_BiggerThan = 1,
	CC_SmallerThan = 2
}ZCompareCond;

class ZStudentInfoDB
{
public:
	virtual ~ZStudentInfoDB() {}

	virtual int Open(const char* apName, const char* ip, uint16_t port) = 0;
	virtual int Close() = 0;

	/// insert data into db
	virtual int Insert(ZStudentInfo* apStuInfo) = 0;

	/// update data
	virtual int Update(ZStudentInfo* apStuInfo) = 0;

	/// query by name and telephone
	virtual ZQueryResult* QueryByName(const char* apName, const char* apTelephone) = 0;

	/// query by country
	virtual ZQueryResult* QueryByCountry(const char* apCountry) {
		return NULL;
	}

	/// query by collge
	virtual ZQueryResult* QueryByCollege(const char* apCountry) {
		return NULL;
	}

	/// query by score
	virtual ZQueryResult* QueryByScore(const char* apScore, ZCompareCond aCompareCond) {
		return NULL;
	}

protected:
	bool m_IncludeDeleted;
};

/* db implement by text file */
class ZStudentInfoDBText : public ZStudentInfoDB
{
public:
	ZStudentInfoDBText();
	virtual ~ZStudentInfoDBText();

	virtual int Open(const char* apName, const char* ip, uint16_t port);
	virtual int Close();

public:
	virtual int Insert(ZStudentInfo* apStuInfo);
	virtual int Update(ZStudentInfo* apStuInfo);

	/// when apName & apTelephone is null, means query all
	virtual ZQueryResult* QueryByName(const char* apName, const char* apTelephone) ;

	virtual ZQueryResult* QueryByCountry(const char* apCountry);

	virtual ZQueryResult* QueryByCollege(const char* apCollege);

	virtual ZQueryResult* QueryByScore(const char* apScore, ZCompareCond aCompareCond);

protected:
	char*       m_pBuffer;
	uint32_t    m_BufSize;

	ztl_shm_t*  m_pShmObj;

	ZQueryResult* m_pResult;
};

