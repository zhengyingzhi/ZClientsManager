#include "StdAfx.h"
#include "StudentInfoDB.h"

#include "ZUtility.h"

#include <ZToolLib/ztl_utils.h>

#include "ZAppConfig.h"

#include <vector>
using namespace std;


/* 比较Number */
bool ZQueryCompareNumber(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;
	if (lpExpect->Number == lpActual->Number)
	{
		return true;
	}
	return false;
}

/* 比较名字和电话 */
bool ZQueryCompareNameAndTel(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (lpExpect->Name[0] && lpExpect->Telehone[0])
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
		if (strlen(lpExpect->Telehone) == 4 && 
			strcmp(lpExpect->Telehone, lpActual->Telehone + 7) == 0) {
			return true;
		}
		else if (strcmp(lpExpect->Telehone, lpActual->Telehone) == 0)
		{
			return true;
		}
	}
	return false;
}

/* 比较QQ */
bool ZQueryCompareStuQQ(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strcmp(lpActual->QQ, lpExpect->QQ))
	{
		return true;
	}

	return false;
}

/* 比较国家 */
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

/* 比较大学 */
bool ZQueryCompareCollege(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strstr(lpActual->CollegeFrom, lpExpect->CollegeFrom) ||
		strstr(lpActual->CollegeTo, lpExpect->CollegeTo))
	{
		return true;
	}

	return false;
}

/* 比较时间，aExtend为比较条件*/
bool ZQueryCompareStuTime(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (aExtend == CC_BiggerThan && lpExpect->InsertTime > lpActual->InsertTime)
	{
		return true;
	}
	else if (aExtend == CC_SmallerThan && lpExpect->InsertTime < lpActual->InsertTime)
	{
		return true;
	}
	else
	{
		// only compare with date
		std::string lExpectDate = ZConvStr2StdDate(lpExpect->InsertTime);
		std::string lActualDate = ZConvStr2StdDate(lpActual->InsertTime);
		if (lExpectDate == lActualDate)
			return true;
	}
	return false;
}

/* 比较分数，aExtend为比较条件 */
bool ZQueryCompareScore(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (aExtend == CC_BiggerThan && lpExpect->LanguageScore > lpActual->LanguageScore)
	{
		return true;
	}
	else if (aExtend == CC_SmallerThan && lpExpect->LanguageScore < lpActual->LanguageScore)
	{
		return true;
	}
	else
	{
		if (lpExpect->LanguageScore == lpActual->LanguageScore)
			return true;
	}
	return false;
}

/* 比较状态 */
bool ZQueryCompareStatus(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strstr(lpActual->Status, lpExpect->Status))
	{
		return true;
	}

	return false;
}

/* 比较来源 */
bool ZQueryCompareSource(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strstr(lpActual->Source, lpExpect->Source))
	{
		return true;
	}

	return false;
}

/* 比较邮箱地址 */
bool ZQueryCompareEMail(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strstr(lpActual->EMail, lpExpect->EMail))
	{
		return true;
	}

	return false;
}

/* 比较身份证号 */
bool ZQueryCompareStuIDNum(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZStudentInfo* lpExpect = (ZStudentInfo*)apExpect;
	ZStudentInfo* lpActual = (ZStudentInfo*)apAcutal;

	if (strlen(lpExpect->IDNumber) > 0 && strlen(lpExpect->IDNumber) <= 6) {
		if (strstr(lpActual->IDNumber + 12, lpExpect->IDNumber)) {
			return true;
		}
	}
	else if (strcmp(lpExpect->IDNumber, lpActual->IDNumber) == 0) {
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

/* 返回一个学生块信息大小 */
static uint32_t _ZStudentInfoBlockSize()
{
	uint32_t lBlockSize = ztl_align(sizeof(ZStudentInfo), STUINFO_DB_ALIGNMENT);
	return lBlockSize;
}

ztl_shm_t* ZDBShmCreate(const std::string& aDBName, uint32_t aDBSize)
{
	ztl_shm_t* lpShmObj;
	lpShmObj = ztl_shm_create(aDBName.c_str(), ztl_open_or_create, ztl_read_write, false);

	if (lpShmObj == NULL)
	{
		TRACE(_T("ZDBShmCreate ztl_shm_create() failed"));
		return NULL;
	}

	ztl_shm_truncate(lpShmObj, aDBSize);
	ztl_shm_map_region(lpShmObj, ztl_read_write);

	if (ztl_shm_get_address(lpShmObj) == NULL)
	{
		TRACE(_T("ZDBShmCreate ztl_shm_get_address() failed"));

		ztl_shm_release(lpShmObj);
		return NULL;
	}

	return lpShmObj;
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

	if (PrivStuShmCreate() != 0)
	{
		return -2;
	}

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
		ztl_log_error(g_Logger, ZTL_LOG_WARN, "StudentInfoDB Insert already exist name:%s, college:%s",
			lpStuInfo->Name, lpStuInfo->CollegeFrom);

		FreeQueryRs(lpQryRs);
		return 1;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = GetAvailStudentInfo();
	if (!lpDstInfo)
	{
		ztl_log_error(g_Logger, ZTL_LOG_CRITICAL, "StudentInfoDB Insert no enough db memory for name:%s, college:%s",
			lpStuInfo->Name, lpStuInfo->CollegeFrom);
		return -99;
	}

	// do write data and sync
	//memcpy(lpDstInfo, lpStuInfo, sizeof(ZStudentInfo));
	ZStuInfoCopy(lpDstInfo, lpStuInfo);

	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZStudentInfo));

	char lFixString[4096] = "";
	int  lLength = ZStuInfoFixString(lpStuInfo, lFixString, 0);
	lFixString[lLength] = '\0';
	ztl_log_error(g_Logger, ZTL_LOG_INFO, "StudentInfoDB Insert\r\n%s", lFixString);

	// got number after db inserted
	lpStuInfo->Number = lpDstInfo->Number;

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
		ztl_log_error(g_Logger, ZTL_LOG_WARN, "StudentInfoDB Update not find %s,%s", 
			lpStuInfo->Name, lpStuInfo->Telehone);
		return -2;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = lpQryRs->RsAtAsType<ZStudentInfo>(0);

	// FIXME: update some fields
	ZStuInfoCopy(lpDstInfo, lpStuInfo);

	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZStudentInfo));

	char lFixString[4096] = "";
	int  lLength = ZStuInfoFixString(lpStuInfo, lFixString, 0);
	lFixString[lLength] = '\0';
	ztl_log_error(g_Logger, ZTL_LOG_INFO, "StudentInfoDB Update\r\n%s", lFixString);

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
		ztl_log_error(g_Logger, ZTL_LOG_WARN, "StudentInfoDB Delete not find %s,%s",
			lpStuInfo->Name, lpStuInfo->Telehone);
		return -2;
	}

	ZStudentInfo* lpDstInfo;
	lpDstInfo = lpQryRs->RsAtAsType<ZStudentInfo>(0);

	// market as deleted
	lpDstInfo->Flag |= ZSI_FLAG_Deleted;
	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZStudentInfo));

	ztl_log_error(g_Logger, ZTL_LOG_INFO, "StudentInfoDB Delete name:%s, college:%s-->>%s", 
		lpDstInfo->Name, lpDstInfo->CollegeFrom, lpDstInfo->CollegeTo);

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
			break;
		}

		if (aExtend != CC_WithDeleted && lpStuInfo->Flag & ZSI_FLAG_Deleted) {
			continue;
		}

		if (apCompFunc(apExpectInfo, lpStuInfo, aExtend))
		{
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
	}

	return lpNextInfo;
}

ZStudentInfo* ZStudentInfoDBText::GetAvailStudentInfo()
{
	uint32_t lBlockSize = _ZStudentInfoBlockSize();
	uint32_t lStuNum = 1;

	ZStudentInfo *lpCurInfo;

	lpCurInfo = NULL;
	while ((lpCurInfo = NextStudentInfo(lpCurInfo)) != NULL)
	{
		if (!lpCurInfo->Name[0])
			break;
		++lStuNum;
	}

	lpCurInfo->Number = lStuNum;

	return lpCurInfo;
}

int ZStudentInfoDBText::PrivStuShmCreate()
{
	if (m_pShmObj)
	{
		ztl_shm_release(m_pShmObj);
		m_pShmObj = NULL;
		m_pBuffer = NULL;
		m_BufSize = m_BufSize * 2;
	}
	else
	{
		m_BufSize = STUINFO_DB_DEFAULT_SIZE;
	}

	m_pShmObj = ZDBShmCreate(m_DBName, m_BufSize);
	if (m_pShmObj == NULL)
	{
		return -1;
	}

	m_pBuffer = (char*)ztl_shm_get_address(m_pShmObj);

	return 0;
}
