#include "StdAfx.h"
#include "StudentInfoDB.h"

#include <ZToolLib/ztl_utils.h>

#include <vector>
using namespace std;


static void ZQryResultAssign(vector<ZStudentInfo*>& aStuVec, ZQueryResult* apQryRS)
{
	if (apQryRS->AllocedN < (uint32_t)aStuVec.size())
	{
		ZQryRsAlloc(apQryRS, aStuVec.size() + 128, ztl_align(sizeof(ZStudentInfo), STUINFO_DB_ALIGNMENT));
	}

	ZStudentInfo* lpStuInfo;
	lpStuInfo = ZDB_QRY_RS_BODY(apQryRS, ZStudentInfo);

	apQryRS->Count = aStuVec.size();
	for (size_t i = 0; i < aStuVec.size(); ++i)
	{
		memcpy(&lpStuInfo[i], aStuVec[i], sizeof(ZStudentInfo));
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
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
{
	m_IncludeDeleted = 0;

	m_pResult = ZQryRsAlloc(NULL, STUINFO_DB_DEFAULT_QRYN, 
		ztl_align(sizeof(ZQueryResult), STUINFO_DB_ALIGNMENT));
}

ZStudentInfoDBText::~ZStudentInfoDBText()
{
	Close();

	if (m_pResult)
		ZQryRsDecrementAndFree(m_pResult);

	if (m_pShmObj)
		ztl_shm_release(m_pShmObj);
}

int ZStudentInfoDBText::Open(const char* apName, const char* ip, uint16_t port)
{
	(void)ip;
	(void)port;
	if (!apName || apName[0] == '\0')
	{
		return -1;
	}

	m_pShmObj = ztl_shm_create(apName, ztl_open_or_create, ztl_read_write, false);

	if (m_pShmObj == NULL)
	{
		TRACE(_T("ZStudentInfoDBText open userinfo db failed"));
		return -1;
	}

	ztl_shm_truncate(m_pShmObj, STUINFO_DB_DEFAULT_SIZE);
	ztl_shm_map_region(m_pShmObj, ztl_read_write);

	m_pBuffer = (char*)ztl_shm_get_address(m_pShmObj);
	if (m_pBuffer == NULL)
	{
		TRACE(_T("ZStudentInfoDBText map region userinfo db failed"));
		return -2;
	}

	m_BufSize = STUINFO_DB_DEFAULT_SIZE;

	return 0;
}

int ZStudentInfoDBText::Close()
{
	if (m_pShmObj)
	{
		ztl_shm_release(m_pShmObj);
		m_pShmObj = NULL;
	}

	return 0;
}

int ZStudentInfoDBText::Insert(ZStudentInfo* apStuInfo)
{
	if (!apStuInfo->Name[0] || !apStuInfo->Telehone[0])
	{
		return -1;
	}

	ZQueryResult* lpResult;
	lpResult = QueryByName(apStuInfo->Name, apStuInfo->Telehone);
	if (lpResult)
	{
		// already exist
		return 1;
	}

	ZStudentInfo* lpSrcInfo;

	lpSrcInfo = (ZStudentInfo*)m_pBuffer;
	while (lpSrcInfo->Name[0])
	{
		lpSrcInfo += ztl_align(sizeof(ZStudentInfo), STUINFO_DB_ALIGNMENT);
	}

	memcpy(lpSrcInfo, apStuInfo, sizeof(ZStudentInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

	return 0;
}

int ZStudentInfoDBText::Update(ZStudentInfo* apStuInfo)
{
	ZQueryResult* lpResult;
	lpResult = QueryByName(apStuInfo->Name, apStuInfo->Telehone);
	if (!lpResult)
	{
		return -1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = ZDB_QRY_RS_BODY(lpResult, ZStudentInfo);
	memcpy(lpDstInfo, apStuInfo, sizeof(ZStudentInfo));

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

