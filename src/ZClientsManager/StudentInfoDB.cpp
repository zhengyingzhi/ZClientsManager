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

static uint32_t _ZStudentInfoBlockSize()
{
	uint32_t lBlockSize = ztl_align(sizeof(ZStudentInfo), STUINFO_DB_ALIGNMENT);
	return lBlockSize;
}

ZStudentInfoDBText::ZStudentInfoDBText()
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
{
	m_IncludeDeleted = 0;
}

ZStudentInfoDBText::~ZStudentInfoDBText()
{
	Close();

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
	ZQueryResult* lpQryRs;
	ZStudentInfo* lpStuInfo;
	lpStuInfo = (ZStudentInfo*)apDataInfo;

	if (!lpStuInfo->Name[0] || !lpStuInfo->Telehone[0])
	{
		return -1;
	}

	lpQryRs = Query(lpStuInfo, ZQueryCompareNameAndTel, 0);
	if (lpQryRs)
	{
		// already exist

		FreeQueryRs(lpQryRs);
		return 1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = GetAvailStudentInfo();
	if (!lpDstInfo)
	{
		return -99;
	}

	// do write data and sync
	memcpy(lpDstInfo, lpStuInfo, sizeof(ZStudentInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZStudentInfo));

	return 0;
}

int ZStudentInfoDBText::Update(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpQryRs;
	ZStudentInfo* lpStuInfo;
	lpStuInfo = (ZStudentInfo*)apDataInfo;
	if (!lpStuInfo->Name[0] && !lpStuInfo->Telehone[0])
	{
		return -1;
	}

	lpQryRs = Query(lpStuInfo, ZQueryCompareNameAndTel, 0);
	if (!lpQryRs)
	{
		return -2;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = lpQryRs->RsAtAsType<ZStudentInfo>(0);

	// FIXME: update some fields
	memcpy(lpDstInfo, lpStuInfo, sizeof(ZStudentInfo));

	FreeQueryRs(lpQryRs);
	return 0;
}

int ZStudentInfoDBText::Delete(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpQryRs;
	ZStudentInfo* lpStuInfo;
	lpStuInfo = (ZStudentInfo*)apDataInfo;

	lpQryRs = Query(lpStuInfo, ZQueryCompareNameAndTel, 0);
	if (!lpQryRs)
	{
		return -1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = lpQryRs->RsAtAsType<ZStudentInfo>(0);

	// market as deleted
	lpDstInfo->Flag |= ZSI_FLAG_Deleted;
	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZStudentInfo));

	FreeQueryRs(lpQryRs);
	return 0;
}

ZQueryResult* ZStudentInfoDBText::Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZQueryResult* lpQryRs;
	ZStudentInfo* lpStuInfo;

	lpQryRs = GetQueryRs();

	lpStuInfo = NULL;
	while ((lpStuInfo = NextStudentInfo(lpStuInfo)) != NULL)
	{
		if (!lpStuInfo->Name[0]) {
			continue;
		}

		if (apCompFunc(apExpectInfo, lpStuInfo, aExtend))
		{
			if (lpStuInfo->Flag & ZSI_FLAG_Deleted) {
				continue;
			}

			lpQryRs->PushBack(lpStuInfo);
		}
	}

	if (lpQryRs->RsCount() == 0)
	{
		FreeQueryRs(lpQryRs);
		return NULL;
	}

	return lpQryRs;
}


ZStudentInfo* ZStudentInfoDBText::NextStudentInfo(ZStudentInfo* apCurStuInfo, bool aAutoExpand)
{
	(void)aAutoExpand;
	ZStudentInfo* lpNextInfo;
	uint32_t lBlockSize = _ZStudentInfoBlockSize();

	if (!apCurStuInfo)
	{
		// first
		lpNextInfo = (ZStudentInfo*)m_pBuffer;
	}
	else if ((char*)apCurStuInfo + lBlockSize > (m_pBuffer + m_BufSize))
	{
		// memory out bound, FIXME: auto expand if need
		lpNextInfo = NULL;
	}
	else
	{
		// next
		lpNextInfo = (ZStudentInfo*)((char*)apCurStuInfo + lBlockSize);
		if (!lpNextInfo->Name[0])
			lpNextInfo = NULL;
	}

	return lpNextInfo;
}

ZStudentInfo* ZStudentInfoDBText::GetAvailStudentInfo()
{
	uint32_t lBlockSize = _ZStudentInfoBlockSize();
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
