#include "StdAfx.h"
#include "TaskDB.h"

#include <ZToolLib/ztl_utils.h>

#include "ZAppConfig.h"

#include <vector>
using namespace std;


bool ZQueryCompareTaskAll(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)apExpect;
	(void)apAcutal;
	(void)aExtend;
	return true;
}

bool ZQueryCompareTaskTime(const void* apExpect, const void* apAcutal, int aExtend)
{
	(void)aExtend;
	ZTaskInfo* lpExpect = (ZTaskInfo*)apExpect;
	ZTaskInfo* lpActual = (ZTaskInfo*)apAcutal;

	if (aExtend == ZTASKTIME_CMP_BiggerThan && lpExpect->TaskTime > lpActual->TaskTime)
	{
		return true;
	}
	else if (aExtend == ZTASKTIME_CMP_SmallerThan && lpExpect->TaskTime < lpActual->TaskTime)
	{
		return true;
	}
	else if (lpExpect->TaskTime == lpActual->TaskTime)
	{
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

/* 返回一个学生块信息大小 */
static uint32_t _ZTaskInfoBlockSize()
{
	uint32_t lBlockSize = ztl_align(sizeof(ZTaskInfo), TASKINFO_DB_ALIGNMENT);
	return lBlockSize;
}

static ztl_shm_t* _ZDBShmCreate(const std::string& aDBName, uint32_t aDBSize)
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


ZTaskInfoDBText::ZTaskInfoDBText()
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
{
}

ZTaskInfoDBText::~ZTaskInfoDBText()
{
	Close();

	if (m_pShmObj)
		ztl_shm_release(m_pShmObj);
}

int ZTaskInfoDBText::Open(const std::string& aDBName, const std::string&  ip, uint16_t port)
{
	(void)ip;
	(void)port;
	if (aDBName.empty())
	{
		return -1;
	}
	m_DBName = aDBName;

	if (PrivTaskShmCreate() != 0)
	{
		return -2;
	}

	return 0;
}

int ZTaskInfoDBText::Close()
{
	if (m_pShmObj)
	{
		ztl_shm_release(m_pShmObj);
		m_pShmObj = NULL;
	}

	return 0;
}

int ZTaskInfoDBText::Insert(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpQryRs;
	ZTaskInfo* lpTaskInfo;
	lpTaskInfo = (ZTaskInfo*)apDataInfo;

	if (!lpTaskInfo->TaskName[0])
	{
		return -1;
	}

	lpQryRs = Query(lpTaskInfo, ZQueryCompareTaskTime, ZTASKTIME_CMP_EQUAL);
	if (lpQryRs)
	{
		// already exist
		ztl_log_error(g_Logger, ZTL_LOG_WARN, "TaskInfoDB Insert already exist name:%s",
			lpTaskInfo->TaskName);

		FreeQueryRs(lpQryRs);
		return 1;
	}

	ZTaskInfo* lpDstInfo;
	lpDstInfo = GetAvailTaskInfo();
	if (!lpDstInfo)
	{
		ztl_log_error(g_Logger, ZTL_LOG_CRITICAL, "TaskInfoDB Insert no enough db memory for name:%s",
			lpTaskInfo->TaskName);
		return -99;
	}

	// do write data and sync
	memcpy(lpDstInfo, lpTaskInfo, sizeof(ZTaskInfo));

	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZTaskInfo));

	return 0;
}

int ZTaskInfoDBText::Update(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpQryRs;
	ZTaskInfo* lpTaskInfo;
	lpTaskInfo = (ZTaskInfo*)apDataInfo;
	if (!lpTaskInfo->TaskName[0])
	{
		return -1;
	}

	lpQryRs = Query(lpTaskInfo, ZQueryCompareTaskTime, 0);
	if (!lpQryRs)
	{
		ztl_log_error(g_Logger, ZTL_LOG_WARN, "TaskInfoDB Update not find %s",
			lpTaskInfo->TaskName);
		return -2;
	}

	ZTaskInfo* lpDstInfo;
	lpDstInfo = lpQryRs->RsAtAsType<ZTaskInfo>(0);

	// FIXME: update some fields
	memcpy(lpDstInfo, lpTaskInfo, sizeof(ZTaskInfo));

	ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZTaskInfo));

	FreeQueryRs(lpQryRs);
	return 0;
}

int ZTaskInfoDBText::Delete(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult* lpQryRs;
	ZTaskInfo* lpTaskInfo;
	lpTaskInfo = (ZTaskInfo*)apDataInfo;

	lpQryRs = Query(lpTaskInfo, ZQueryCompareTaskTime, 0);
	if (!lpQryRs)
	{
		ztl_log_error(g_Logger, ZTL_LOG_WARN, "TaskInfoDB Delete not find %s",
			lpTaskInfo->TaskName);
		return -2;
	}

	ZTaskInfo* lpDstInfo;
	lpDstInfo = lpQryRs->RsAtAsType<ZTaskInfo>(0);

	// remove it from db
	ZTaskInfo* lpNext = NULL;
	while ((lpNext = NextTaskInfo(lpDstInfo)) != NULL)
	{
		if (!lpNext->TaskName[0])
		{
			break;
		}

		memmove(lpDstInfo, lpNext, sizeof(ZTaskInfo));
		lpDstInfo = lpNext;
	}

	if (lpDstInfo)
	{
		memset(lpDstInfo, 0, sizeof(ZTaskInfo));
	}

	FreeQueryRs(lpQryRs);
	return 0;
}

ZQueryResult* ZTaskInfoDBText::Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZQueryResult* lpQryRs;
	ZTaskInfo* lpTaskInfo;

	lpQryRs = GetQueryRs();

	lpTaskInfo = NULL;
	while ((lpTaskInfo = NextTaskInfo(lpTaskInfo)) != NULL)
	{
		if (!lpTaskInfo->TaskName[0]) {
			break;
		}

		if (apCompFunc(apExpectInfo, lpTaskInfo, aExtend))
		{
			lpQryRs->PushBack(lpTaskInfo);
		}
	}

	if (lpQryRs->RsCount() == 0)
	{
		FreeQueryRs(lpQryRs);
		return NULL;
	}

	return lpQryRs;
}


ZTaskInfo* ZTaskInfoDBText::NextTaskInfo(ZTaskInfo* apCurTaskInfo, bool aAutoExpand)
{
	(void)aAutoExpand;
	ZTaskInfo* lpNextInfo;
	uint32_t lBlockSize = _ZTaskInfoBlockSize();

	if (!apCurTaskInfo)
	{
		// first
		lpNextInfo = (ZTaskInfo*)m_pBuffer;
	}
	else if ((char*)apCurTaskInfo + lBlockSize > (m_pBuffer + m_BufSize))
	{
		// memory out bound, FIXME: auto expand if need
		lpNextInfo = NULL;
	}
	else
	{
		// next
		lpNextInfo = (ZTaskInfo*)((char*)apCurTaskInfo + lBlockSize);
	}

	return lpNextInfo;
}

ZTaskInfo* ZTaskInfoDBText::GetAvailTaskInfo()
{
	uint32_t lBlockSize = _ZTaskInfoBlockSize();
	uint32_t lStuNum = 1;

	ZTaskInfo *lpCurInfo;

	lpCurInfo = NULL;
	while ((lpCurInfo = NextTaskInfo(lpCurInfo)) != NULL)
	{
		if (!lpCurInfo->TaskName[0])
			break;
		++lStuNum;
	}

	return lpCurInfo;
}

int ZTaskInfoDBText::PrivTaskShmCreate()
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
		m_BufSize = TASKINFO_DB_DEFAULT_SIZE;
	}

	m_pShmObj = _ZDBShmCreate(m_DBName, m_BufSize);
	if (m_pShmObj == NULL)
	{
		return -1;
	}

	m_pBuffer = (char*)ztl_shm_get_address(m_pShmObj);

	return 0;
}
