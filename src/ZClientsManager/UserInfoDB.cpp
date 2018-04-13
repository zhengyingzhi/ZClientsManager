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


ZUserInfoDBText::ZUserInfoDBText()
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
{
	m_pQryRs = ZQryRsAlloc(NULL, USERINFO_DB_DEFAULT_QRYN, 
		ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT));
}

ZUserInfoDBText::~ZUserInfoDBText()
{
	if (m_pQryRs)
		FreeQueryRs(m_pQryRs);

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
	ZQueryResult*   lpResult;
	lpResult = Query(lpUserInfo, ZQueryCompareUserName, 0);
	if (lpResult)
	{
		// already exist
		FreeQueryRs(lpResult);
		return 1;
	}

	lpDstInfo = GetAvailUserInfo();
	if (!lpDstInfo) {
		return -99;
	}

	memcpy(lpDstInfo, lpUserInfo, sizeof(ZUserInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);
	return 0;
}

int ZUserInfoDBText::Update(void* apDataInfo, uint32_t aDataSize)
{
	ZUserInfo* lpUserInfo;

	lpUserInfo = (ZUserInfo*)apDataInfo;
	if (!lpUserInfo->UserName[0])
	{
		return -1;
	}

	ZUserInfo*      lpDstInfo;
	ZQueryResult*   lpResult;

	lpResult = Query(lpUserInfo, ZQueryCompareUserName, 0);
	if (lpResult)
	{
		lpDstInfo = ZDB_QRY_RS_BODY(lpResult, ZUserInfo);

		memcpy(lpDstInfo, lpUserInfo, sizeof(ZUserInfo));
		ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

		FreeQueryRs(lpResult);
		return 1;
	}

	return 0;
}

int ZUserInfoDBText::Delete(void* apDataInfo, uint32_t aDataSize)
{
	ZUserInfo*      lpUserInfo;
	ZUserInfo*      lpDstInfo;
	ZQueryResult*   lpResult;

	lpUserInfo = (ZUserInfo*)apDataInfo;
	if (!lpUserInfo->UserName[0])
	{
		return -1;
	}

	lpResult = Query(lpUserInfo, ZQueryCompareUserName, 0);
	if (lpResult)
	{
		lpDstInfo = ZDB_QRY_RS_BODY(lpResult, ZUserInfo);

		// market as deleted
		lpUserInfo->Deleted = 1;
		ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

		FreeQueryRs(lpResult);
		return 1;
	}
	return 0;
}

ZQueryResult* ZUserInfoDBText::Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZUserInfo* lpSrcInfo;
	ZUserInfo* lpDstInfo;

	ZQueryResult* lpQryRs = m_pQryRs;
	if (!lpQryRs)
	{
		lpQryRs = ZQryRsAlloc(NULL, USERINFO_DB_DEFAULT_QRYN,
			ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT));
	}
	else
	{
		m_pQryRs = NULL;
	}

	lpQryRs->Count = 0;

	lpSrcInfo = NULL;
	while ((lpSrcInfo = NextUserInfo(lpSrcInfo)) != NULL)
	{
		if (lpSrcInfo->Deleted)
			continue;

		if (lpQryRs->Count >= lpQryRs->AllocedN)
		{
			lpQryRs = ZQryRsAlloc(lpQryRs, USERINFO_DB_DEFAULT_QRYN + 128,
				ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT));
		}

		lpDstInfo = ZDB_QRY_RS_BODY(lpQryRs, ZUserInfo);
		memcpy(&lpDstInfo[lpQryRs->Count], lpSrcInfo, sizeof(ZUserInfo));
		lpQryRs->Count++;
	}

	if (lpQryRs->Count == 0)
	{
		FreeQueryRs(lpQryRs);
		return NULL;
	}

	return lpQryRs;
}



ZUserInfo* ZUserInfoDBText::NextUserInfo(ZUserInfo* apCurUserInfo)
{
	uint32_t lOffset = ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
	ZUserInfo* lpNextInfo;

	if (!apCurUserInfo)
	{
		lpNextInfo = (ZUserInfo*)m_pBuffer;
	}
	else if ((char*)apCurUserInfo + lOffset > (m_pBuffer + m_BufSize))
	{
		lpNextInfo = NULL;
	}
	else
	{
		lpNextInfo = (ZUserInfo*)((char*)apCurUserInfo + lOffset);
		if (!lpNextInfo->UserName[0])
			lpNextInfo = NULL;
	}

	return lpNextInfo;
}

ZUserInfo* ZUserInfoDBText::GetAvailUserInfo()
{
	uint32_t lOffset = ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
	ZUserInfo *lpCurInfo, *lpNextInfo;

	lpCurInfo = NULL;
	while ((lpCurInfo = NextUserInfo(lpCurInfo)) != NULL)
	{
		lpNextInfo = lpCurInfo;
	}

	if (lpNextInfo->UserName[0])
		lpNextInfo = NULL;

	return lpNextInfo;
}

