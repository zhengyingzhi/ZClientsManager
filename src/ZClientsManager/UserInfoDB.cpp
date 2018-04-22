#include <afxwin.h>

#include <ZToolLib/ztl_utils.h>

#include "UserInfoDB.h"


/* 比较名字 */
bool ZQueryCompareUserName(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZUserInfo* lpExpect = (ZUserInfo*)apExpect;
	ZUserInfo* lpActual = (ZUserInfo*)apAcutal;

	if (strcmp(lpExpect->UserName, lpActual->UserName) == 0) {
		return true;
	}
	return false;
}

/* 比较电话 */
bool ZQueryCompareUserTel(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZUserInfo* lpExpect = (ZUserInfo*)apExpect;
	ZUserInfo* lpActual = (ZUserInfo*)apAcutal;

	if (strlen(lpExpect->Telephone) > 0 && strlen(lpExpect->Telephone) <= 4) {
		if (strcmp(lpActual->Telephone + 7, lpExpect->Telephone) == 0) {
			return true;
		}
	}
	if (strcmp(lpExpect->Telephone, lpActual->Telephone) == 0) {
		return true;
	}
	return false;
}

/* 比较QQ */
bool ZQueryCompareUserQQ(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZUserInfo* lpExpect = (ZUserInfo*)apExpect;
	ZUserInfo* lpActual = (ZUserInfo*)apAcutal;

	if (strcmp(lpExpect->QQ, lpActual->QQ) == 0) {
		return true;
	}
	return false;
}

/* 比较身份证号 */
bool ZQueryCompareUserIDNum(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZUserInfo* lpExpect = (ZUserInfo*)apExpect;
	ZUserInfo* lpActual = (ZUserInfo*)apAcutal;

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



/* 一个账户信息块大小 */
static uint32_t _ZUserInfoBlockSize()
{
	uint32_t lBlockSize = ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
	return lBlockSize;
}


ZUserInfoDBText::ZUserInfoDBText()
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
{
}

ZUserInfoDBText::~ZUserInfoDBText()
{
	if (m_pShmObj)
		ztl_shm_release(m_pShmObj);
}

int ZUserInfoDBText::Open(const std::string& aDBName, const std::string& ip, uint16_t port)
{
	(void)ip;
	(void)port;
	if (aDBName.empty())
	{
		return -1;
	}
	m_DBName = aDBName;

	if (PrivUserShmCreate() != 0)
	{
		return -2;
	}

	return 0;
}

int ZUserInfoDBText::Close()
{
	if (m_pShmObj)
	{
		ztl_shm_release(m_pShmObj);
		m_pShmObj = NULL;
	}

	return 0;
}

int ZUserInfoDBText::Insert(void* apDataInfo, uint32_t aDataSize)
{
	ZUserInfo* lpUserInfo;

	lpUserInfo = (ZUserInfo*)apDataInfo;
	if (!lpUserInfo->UserName[0])
	{
		return -1;
	}

	ZUserInfo*      lpDstInfo;
	ZQueryResult*   lpQryRs;
	lpQryRs = Query(lpUserInfo, ZQueryCompareUserName, 0);
	if (lpQryRs)
	{
		// already exist
		FreeQueryRs(lpQryRs);
		return 1;
	}

	lpDstInfo = GetAvailUserInfo();
	if (!lpDstInfo) {
		return -99;
	}

	// FIXME: only fill some fields
	memcpy(lpDstInfo, lpUserInfo, sizeof(ZUserInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

	return 0;
}

int ZUserInfoDBText::Update(void* apDataInfo, uint32_t aDataSize)
{
	ZQueryResult*   lpQryRs;
	ZUserInfo*      lpUserInfo;
	ZUserInfo*      lpDstInfo;

	lpUserInfo = (ZUserInfo*)apDataInfo;
	if (!lpUserInfo->UserName[0])
	{
		return -1;
	}

	lpQryRs = Query(lpUserInfo, ZQueryCompareUserName, 0);
	if (!lpQryRs)
	{
		return -2;
	}

	lpDstInfo = lpQryRs->RsAtAsType<ZUserInfo>(0);

	memcpy(lpDstInfo, lpUserInfo, sizeof(ZUserInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

	FreeQueryRs(lpQryRs);
	return 0;
}

int ZUserInfoDBText::Delete(void* apDataInfo, uint32_t aDataSize)
{
	ZUserInfo*      lpUserInfo;
	ZUserInfo*      lpDstInfo;
	ZQueryResult*   lpQryRs;

	lpUserInfo = (ZUserInfo*)apDataInfo;
	if (!lpUserInfo->UserName[0])
	{
		return -1;
	}

	lpQryRs = Query(lpUserInfo, ZQueryCompareUserName, 0);
	if (lpQryRs)
	{
		lpDstInfo = lpQryRs->RsAtAsType<ZUserInfo>(0);

		// market as deleted
		lpUserInfo->Flags |= ZUSER_FLAG_Deleted;
		ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZUserInfo));

		FreeQueryRs(lpQryRs);
		return 1;
	}
	return 0;
}

ZQueryResult* ZUserInfoDBText::Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZUserInfo*    lpDBInfo;
	ZQueryResult* lpQryRs = GetQueryRs();

	lpDBInfo = NULL;
	while ((lpDBInfo = NextUserInfo(lpDBInfo)) != NULL)
	{
		if (!lpDBInfo->UserName[0]) {
			break;
		}

		if (lpDBInfo->Flags & ZUSER_FLAG_Deleted) {
			continue;
		}

		if (apCompFunc(apExpectInfo, lpDBInfo, 0))
		{
			lpQryRs->PushBack(lpDBInfo);
		}
	}

	if (lpQryRs->RsCount() == 0)
	{
		FreeQueryRs(lpQryRs);
		return NULL;
	}

	return lpQryRs;
}



ZUserInfo* ZUserInfoDBText::NextUserInfo(ZUserInfo* apCurUserInfo, bool aAutoExpand)
{
	(void)aAutoExpand;
	ZUserInfo* lpNextInfo;
	uint32_t   lBlockSize = _ZUserInfoBlockSize();

	if (!apCurUserInfo)
	{
		// first
		lpNextInfo = (ZUserInfo*)m_pBuffer;
	}
	else if ((char*)apCurUserInfo + lBlockSize > (m_pBuffer + m_BufSize))
	{
		// memory out bound
		lpNextInfo = NULL;
	}
	else
	{
		// next
		lpNextInfo = (ZUserInfo*)((char*)apCurUserInfo + lBlockSize);
	}

	return lpNextInfo;
}

ZUserInfo* ZUserInfoDBText::GetAvailUserInfo()
{
	ZUserInfo*  lpCurInfo;
	uint32_t    lBlockSize = _ZUserInfoBlockSize();
	uint32_t    lUserNum = 1;

	lpCurInfo = NULL;
	while ((lpCurInfo = NextUserInfo(lpCurInfo)) != NULL)
	{
		if (!lpCurInfo->UserName[0])
			break;
		++lUserNum;
	}

	lpCurInfo->Number = lUserNum;

	return lpCurInfo;
}


extern ztl_shm_t* ZDBShmCreate(const std::string& aDBName, uint32_t aDBSize);

int ZUserInfoDBText::PrivUserShmCreate()
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
		m_BufSize = USERINFO_DB_DEFAULT_SIZE;
	}

	m_pShmObj = ZDBShmCreate(m_DBName, m_BufSize);
	if (m_pShmObj == NULL)
	{
		return -1;
	}

	m_pBuffer = (char*)ztl_shm_get_address(m_pShmObj);

	return 0;
}
