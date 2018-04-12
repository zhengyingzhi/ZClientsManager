#include "StdAfx.h"
#include "StudentInfoDB.h"

#include <ZToolLib/ztl_utils.h>

#include <vector>
using namespace std;


bool ZQueryCompareNameAndTel(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (lpExpect->Name[0] && lpActual->Name[0])
	{
		// query by both
		if (strcmp(lpExpect->Name, lpActual->Name) == 0 &&
			strcmp(lpExpect->Telehone, lpActual->Telehone) == 0)
		{
			return true;
		}
		return false;
	}
	else if (lpExpect->Name[0] && !lpExpect->Telehone[0])
	{
		// query by name
		if (strcmp(lpExpect->Name, lpActual->Name) == 0)
		{
			return true;
		}
		return false;
	}
	else if (!lpExpect->Name[0] && lpExpect->Telehone[0])
	{
		// query by telephone
		if (strcmp(lpExpect->Telehone, lpActual->Telehone) == 0)
		{
			return true;
		}
	}
	return false;
}

bool ZQueryCompareCountry(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strstr(lpActual->Country, lpExpect->Country))
	{
		return true;
	}

	return false;
}

bool ZQueryCompareCollege(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strcmp(lpActual->College, lpExpect->College))
	{
		return true;
	}

	return false;
}

bool ZQueryCompareScore(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (aExtend == CC_BiggerThan)
	{
		if (lpExpect->LanguageScore > lpActual->LanguageScore)
			return true;
	}
	else if (aExtend == CC_SmallerThan)
	{
		if (lpExpect->LanguageScore < lpActual->LanguageScore)
			return true;
	}
	else
	{
		if (lpExpect->LanguageScore == lpActual->LanguageScore)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

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

	m_pQryRs = ZQryRsAlloc(NULL, STUINFO_DB_DEFAULT_QRYN,
		ztl_align(sizeof(ZQueryResult), STUINFO_DB_ALIGNMENT));
}

ZStudentInfoDBText::~ZStudentInfoDBText()
{
	Close();

	if (m_pQryRs)
		FreeQueryRs(m_pQryRs);

	if (m_pShmObj)
		ztl_shm_release(m_pShmObj);
}

int ZStudentInfoDBText::Open(const std::string& aDBName, const std::string&  ip, uint16_t port)
{
	(void)ip;
	(void)port;
	if (aDBName.empty())
	{
		return -1;
	}
	m_DBName = aDBName;

	m_pShmObj = ztl_shm_create(aDBName.c_str(), ztl_open_or_create, ztl_read_write, false);

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

int ZStudentInfoDBText::Insert(void* apDataInfo, uint32_t aDataSize)
{
	ZStudentInfo* lpStuInfo;
	lpStuInfo = (ZStudentInfo*)apDataInfo;

	if (!lpStuInfo->Name[0] || !lpStuInfo->Telehone[0])
	{
		return -1;
	}

	ZQueryResult* lpResult;
	lpResult = Query(lpStuInfo, ZQueryCompareNameAndTel, 0);
	if (lpResult)
	{
		// already exist

		FreeQueryRs(lpResult);
		return 1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = GetAvailStudentInfo();
	if (!lpDstInfo)
	{
		return -99;
	}

	memcpy(lpDstInfo, lpStuInfo, sizeof(ZStudentInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

	return 0;
}

int ZStudentInfoDBText::Update(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpResult;
	ZStudentInfo* lpStuInfo;
	lpStuInfo = (ZStudentInfo*)apDataInfo;

	lpResult = Query(lpStuInfo, ZQueryCompareNameAndTel, 0);
	if (!lpResult)
	{
		return -1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = ZDB_QRY_RS_BODY(lpResult, ZStudentInfo);
	memcpy(lpDstInfo, lpStuInfo, sizeof(ZStudentInfo));

	FreeQueryRs(lpResult);
	return 0;
}

int ZStudentInfoDBText::Delete(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpResult;
	ZStudentInfo* lpStuInfo;
	lpStuInfo = (ZStudentInfo*)apDataInfo;

	lpResult = Query(lpStuInfo, ZQueryCompareNameAndTel, 0);
	if (!lpResult)
	{
		return -1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = ZDB_QRY_RS_BODY(lpResult, ZStudentInfo);

	// market as deleted
	lpDstInfo->Flag |= ZSI_FLAG_Deleted;

	FreeQueryRs(lpResult);
	return 0;
}

ZQueryResult* ZStudentInfoDBText::Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZQueryResult* lpQryRs;
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;
	lVec.reserve(128);

	lpStuInfo = NULL;
	while ((lpStuInfo = NextStudentInfo(lpStuInfo)) != NULL)
	{
		if (apCompFunc(apExpectInfo, lpStuInfo, aExtend))
		{
			if (lpStuInfo->Flag & ZSI_FLAG_Deleted) {
				continue;
			}

			lVec.push_back(lpStuInfo);
		}
	}

	if (lVec.empty())
	{
		return NULL;
	}

	ZQryResultAssign(lVec, m_pQryRs);
	lpQryRs = m_pQryRs;
	m_pQryRs = NULL;
	return lpQryRs;
}


ZStudentInfo* ZStudentInfoDBText::NextStudentInfo(ZStudentInfo* apCurStuInfo)
{
	uint32_t lOffset = ztl_align(sizeof(ZStudentInfo), STUINFO_DB_ALIGNMENT);
	ZStudentInfo* lpNextInfo;

	if (!apCurStuInfo)
	{
		lpNextInfo =(ZStudentInfo*)m_pBuffer;
	}
	else if ((char*)apCurStuInfo + lOffset > (m_pBuffer + m_BufSize))
	{
		lpNextInfo = NULL;
	}
	else
	{
		lpNextInfo = (ZStudentInfo*)((char*)apCurStuInfo + lOffset);
		if (!lpNextInfo->Name[0])
			lpNextInfo = NULL;
	}

	return lpNextInfo;
}

ZStudentInfo* ZStudentInfoDBText::GetAvailStudentInfo()
{
	uint32_t lOffset = ztl_align(sizeof(ZStudentInfo), STUINFO_DB_ALIGNMENT);
	ZStudentInfo *lpCurInfo, *lpNextInfo;

	lpCurInfo = NULL;
	while ((lpCurInfo = NextStudentInfo(lpCurInfo)) != NULL)
	{
		lpNextInfo = lpCurInfo;
	}

	if (lpNextInfo->Name[0])
		lpNextInfo = NULL;

	return lpNextInfo;
}
