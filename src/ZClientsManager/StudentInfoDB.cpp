#include "StdAfx.h"
#include "StudentInfoDB.h"
#include <vector>
using namespace std;

#define DEFAULT_DB_BUFFER_SIZE  (8 * 1024 * 1024)
#define DEFAULT_DB_QUERYED_N    256


static void ZQryResultCleanupEmpty(struct stQueryResult* apRS)
{
	(void)apRS;
}

static void ZQryResultCleanup(struct stQueryResult* apRS)
{
	free(apRS);
}

static void ZQryResultAssign(vector<ZStudentInfo*>& aStuVec, ZQueryResult* apQryRS)
{
	if (apQryRS->m_AllocedN < (uint32_t)aStuVec.size())
	{
		ZQryResultCleanup(apQryRS);
		apQryRS = (ZQueryResult*)calloc(1, (aStuVec.size() + 128) * sizeof(ZQueryResult));
		apQryRS->Cleanup = ZQryResultCleanupEmpty;
		apQryRS->m_AllocedN = aStuVec.size() + 128;
	}

	apQryRS->m_Count = aStuVec.size();
	for (size_t i = 0; i < aStuVec.size(); ++i)
	{
		memcpy(&apQryRS->m_StuInfo[i], aStuVec[i], sizeof(ZStudentInfo));
	}
}

#if 0
typedef bool(*QueryCompareFunc)(ZStudentInfo* , const char* , const char* );

static bool QueryByNameFunc(ZStudentInfo* apStuInfo, const char* apName, const char* apTelephone)
{
	return false;
}

static bool QueryByCountryFunc(ZStudentInfo* apStuInfo, const char* apCountry, const char* apReservce)
{
	return false;
}

static bool QueryByCollegeFunc(ZStudentInfo* apStuInfo, const char* apCollege, const char* apReservce)
{
	return false;
}
#endif//0

//////////////////////////////////////////////////////////////////////////

ZStudentInfoDBText::ZStudentInfoDBText()
	: m_fp(NULL)
	, m_pBuffer(NULL)
	, m_BufSize(0)
	, m_BufPos(0)
{
	m_pResult = (ZQueryResult*)calloc(1, DEFAULT_DB_QUERYED_N * sizeof(ZQueryResult));
	m_pResult->m_AllocedN = DEFAULT_DB_QUERYED_N;
	m_pResult->Cleanup = ZQryResultCleanupEmpty;
}

ZStudentInfoDBText::~ZStudentInfoDBText()
{
	Close();

	if (m_pBuffer)
	{
		free(m_pBuffer);
	}

	if (m_pResult)
	{
		free(m_pResult);
	}
}

int ZStudentInfoDBText::Open(const char* apName, const char* ip, uint16_t port)
{
	(void)ip;
	(void)port;
	if (!apName || apName[0] == '\0')
	{
		return -1;
	}

	m_fp = fopen(apName, "ab+");
	if (m_fp == NULL)
	{
		return -2;
	}

	m_BufSize = DEFAULT_DB_BUFFER_SIZE;
	m_pBuffer = (char*)malloc(m_BufSize);
	memset(m_pBuffer, 0, m_BufSize);

	Sync();

	return 0;
}

int ZStudentInfoDBText::Close()
{
	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	memset(m_pBuffer, 0, m_BufSize);

	return 0;
}

int ZStudentInfoDBText::Insert(ZStudentInfo* apStuInfo)
{
	if (!m_fp)
	{
		return -1;
	}

	fseek(m_fp, 0, SEEK_END);
	fwrite(apStuInfo, sizeof(ZStudentInfo), 1, m_fp);
	fflush(m_fp);

	memcpy(m_pBuffer + m_BufPos, apStuInfo, sizeof(ZStudentInfo));
	m_BufPos += sizeof(ZStudentInfo);

	return 0;
}

ZQueryResult* ZStudentInfoDBText::QueryByName(const char* apName, const char* apTelephone)
{
	bool lFind;
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;
	lVec.reserve(128);

	lFind = false;
	lpStuInfo = (ZStudentInfo*)m_pBuffer;
	while (lpStuInfo->Name[0])
	{
		if (apName == NULL || apTelephone == NULL)
		{
			// 查询所有
			lFind = true;
			lVec.push_back(lpStuInfo);
		}
		else if (apName[0] == '\0' && strcmp(lpStuInfo->Telehone, apTelephone) == 0)
		{
			// 查询名字符合的
			lFind = true;
			lVec.push_back(lpStuInfo);
		}
		else if (strcmp(lpStuInfo->Name, apName) == 0 && apTelephone[0] == '\0')
		{
			// 查询电话符合的
			lFind = true;
			lVec.push_back(lpStuInfo);
		}
		else if (strcmp(lpStuInfo->Name, apName) == 0 &&
			strcmp(lpStuInfo->Telehone, apTelephone) == 0)
		{
			// // 查询同时符合的
			lFind = true;
			lVec.push_back(lpStuInfo);
			break;
		}

		++lpStuInfo;
	}

	if (!lFind)
	{
		return NULL;
	}

	ZQryResultAssign(lVec, m_pResult);
	return m_pResult;
}

ZQueryResult* ZStudentInfoDBText::QueryByCountry(const char* apCountry)
{
	if (apCountry == NULL || apCountry[0] == '\0')
	{
		return NULL;
	}

	bool lFind;
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;
	lVec.reserve(32);

	lFind = false;
	lpStuInfo = (ZStudentInfo*)m_pBuffer;
	while (lpStuInfo->Name[0])
	{
		if (strcmp(lpStuInfo->Country, apCountry) == 0)
		{
			lFind = true;
			lVec.push_back(lpStuInfo);
		}

		++lpStuInfo;
	}

	if (!lFind)
	{
		return NULL;
	}

	ZQryResultAssign(lVec, m_pResult);
	return m_pResult;
}

ZQueryResult* ZStudentInfoDBText::QueryByCollege(const char* apCollege)
{
	if (apCollege == NULL || apCollege[0] == '\0')
	{
		return NULL;
	}

	bool lFind;
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;
	lVec.reserve(32);

	lFind = false;
	lpStuInfo = (ZStudentInfo*)m_pBuffer;
	while (lpStuInfo->Name[0])
	{
		if (strstr(lpStuInfo->College, apCollege))
		{
			lFind = true;
			lVec.push_back(lpStuInfo);
		}

		++lpStuInfo;
	}

	if (!lFind)
	{
		return NULL;
	}

	ZQryResultAssign(lVec, m_pResult);
	return m_pResult;
}

ZQueryResult* ZStudentInfoDBText::QueryByScore(const char* apScore, ZCompareCond aCompareCond)
{
	return NULL;
}

void ZStudentInfoDBText::Sync()
{
	// todo: check file size firstly
	uint32_t lFileSize = 0;
	fseek(m_fp, 0, SEEK_END);
	lFileSize = ftell(m_fp);

	fseek(m_fp, 0, SEEK_SET);
	m_BufPos = 0;
	while (fread(m_pBuffer + m_BufPos, sizeof(ZStudentInfo), 1, m_fp))
	{
		m_BufPos += sizeof(ZStudentInfo);
	}
}
