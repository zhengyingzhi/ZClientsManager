#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StudentInfo.h"

typedef struct 
{
	const char* m_pName;
	const char* m_pTelephone;
}ZQueryCondition;

typedef struct stQueryResult
{
	void(*Cleanup)(struct stQueryResult* apRS);
	uint32_t m_AllocedN;
	uint32_t m_Count;
	ZStudentInfo m_StuInfo[1];
}ZQueryResult;

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

	/// insert data into data
	virtual int Insert(ZStudentInfo* apStuInfo) = 0;

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

	/// when apName & apTelephone is null, means query all
	virtual ZQueryResult* QueryByName(const char* apName, const char* apTelephone) ;

	virtual ZQueryResult* QueryByCountry(const char* apCountry);

	virtual ZQueryResult* QueryByCollege(const char* apCollege);

	virtual ZQueryResult* QueryByScore(const char* apScore, ZCompareCond aCompareCond);

protected:
	void Sync();

protected:
	FILE*       m_fp;
	char*       m_pBuffer;
	uint32_t    m_BufSize;
	uint32_t    m_BufPos;

	ZQueryResult* m_pResult;
};

