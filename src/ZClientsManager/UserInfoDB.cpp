#include <afxwin.h>

#include <ZToolLib/ztl_utils.h>

#include "UserInfoDB.h"


bool ZQueryCompareUserName(const void* apExpect, const void* apAcutal, int aExtend)
{
	ZUserInfo* lpExpect = (ZUserInfo*)apExpect;
	ZUserInfo* lpActual = (ZUserInfo*)apAcutal;

	if (strcmp(lpExpect->UserName, lpActual->UserName) == 0) {
		return true;
	}
	return false;
}


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

	m_pShmObj = ztl_shm_create(aDBName.c_str(), ztl_open_or_create, ztl_read_write, false);

	if (m_pShmObj == NULL)
	{
		TRACE(_T("ZUserInfoDBText open userinfo db failed"));
		return -1;
	}

	ztl_shm_truncate(m_pShmObj, USERINFO_DB_DEFAULT_SIZE);
	ztl_shm_map_region(m_pShmObj, ztl_read_write);

	m_pBuffer = (char*)ztl_shm_get_address(m_pShmObj);
	if (m_pBuffer == NULL)
	{
		TRACE(_T("ZUserInfoDBText map region userinfo db failed"));
		return -2;
	}

	m_BufSize = USERINFO_DB_DEFAULT_SIZE;

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
		lpUserInfo->Deleted = 1;
		ztl_shm_flush_to_file(m_pShmObj, true, lpDstInfo, sizeof(ZUserInfo));

		FreeQueryRs(lpQryRs);
		return 1;
	}
	return 0;
}

ZQueryResult* ZUserInfoDBText::Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZUserInfo*    lpSrcInfo;
	ZQueryResult* lpQryRs = GetQueryRs();

	lpSrcInfo = NULL;
	while ((lpSrcInfo = NextUserInfo(lpSrcInfo)) != NULL)
	{
		if (lpSrcInfo->Deleted)
			continue;

		lpQryRs->PushBack(lpSrcInfo);
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
		if (!lpNextInfo->UserName[0])
			lpNextInfo = NULL;
	}

	return lpNextInfo;
}

ZUserInfo* ZUserInfoDBText::GetAvailUserInfo()
{
	ZUserInfo *lpCurInfo, *lpNextInfo;
	uint32_t   lBlockSize = _ZUserInfoBlockSize();

	lpCurInfo = NULL;
	while ((lpCurInfo = NextUserInfo(lpCurInfo)) != NULL)
	{
		lpNextInfo = lpCurInfo;
	}

	if (lpNextInfo->UserName[0])
		lpNextInfo = NULL;

	return lpNextInfo;
}

