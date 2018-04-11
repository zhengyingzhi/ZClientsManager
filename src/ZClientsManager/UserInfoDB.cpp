#include <afxwin.h>

#include <ZToolLib/ztl_utils.h>

#include "UserInfoDB.h"


static void ZUserQryResultCleanupEmpty(ZUserQueryResult* apRS)
{
	free(apRS);
}

static ZUserQueryResult* ZUserQryResultAlloc(ZUserQueryResult* apRS, uint32_t aNewAllocN)
{
	ZUserQueryResult* lpResult;

	if (apRS == NULL)
	{
		lpResult = (ZUserQueryResult*)calloc(1, sizeof(ZUserQueryResult) + USERINFO_DB_DEFAULT_QRYN * sizeof(ZUserInfo));
		lpResult->m_AllocedN = USERINFO_DB_DEFAULT_QRYN;
		lpResult->Cleanup = ZUserQryResultCleanupEmpty;
	}
	else
	{
		lpResult = (ZUserQueryResult*)realloc(apRS, sizeof(ZUserQueryResult) + aNewAllocN * sizeof(ZUserInfo));
		lpResult->m_AllocedN = aNewAllocN;
		lpResult->Cleanup = ZUserQryResultCleanupEmpty;
	}
	return lpResult;
}


ZUserInfoDBText::ZUserInfoDBText()
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
	, m_pResult(NULL)
{
	m_pResult = ZUserQryResultAlloc(NULL, USERINFO_DB_DEFAULT_QRYN);
}

ZUserInfoDBText::~ZUserInfoDBText()
{
	if (m_pResult)
		m_pResult->Cleanup(m_pResult);
}

int ZUserInfoDBText::Open(const char* apName, const char* ip, uint16_t port)
{
	m_pShmObj = ztl_shm_create(apName, ztl_open_or_create, ztl_read_write, false);

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

int ZUserInfoDBText::Insert(ZUserInfo* apUserInfo)
{
	if (apUserInfo->UserName[0] == '\0')
	{
		return -1;
	}

	ZUserQueryResult* lpResult;
	lpResult = Query(apUserInfo->UserName, false);
	if (lpResult)
	{
		// already exist
		return 1;
	}

	ZUserInfo* lpSrcInfo;

	lpSrcInfo = (ZUserInfo*)m_pBuffer;
	while (lpSrcInfo->UserName[0])
	{
		lpSrcInfo += ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
	}

	memcpy(lpSrcInfo, apUserInfo, sizeof(ZUserInfo));
	ztl_shm_flush_to_file(m_pShmObj, true, NULL, 0);

	return 0;
}

int ZUserInfoDBText::Remove(ZUserInfo* apUserInfo)
{
	if (apUserInfo->UserName[0] == '\0')
	{
		return -1;
	}

	ZUserQueryResult* lpResult = Query(apUserInfo->UserName, false);
	if (lpResult)
	{
		ZUserInfo* lpUserInfo;
		lpUserInfo = ZUSER_QUERY_RS_BODY(lpResult);

		// market as deleted
		lpUserInfo->Deleted = 1;
	}

	return 0;
}

ZUserQueryResult* ZUserInfoDBText::Query(const char* apUserID, bool aIncludeDeleted)
{
	ZUserInfo* lpSrcInfo;
	ZUserInfo* lpDstInfo;

	m_pResult->m_Count = 0;

	lpSrcInfo = (ZUserInfo*)m_pBuffer;
	while (lpSrcInfo->UserName[0] && (char*)lpSrcInfo < (m_pBuffer + m_BufSize))
	{
		if (apUserID[0] != '\0' && strcmp(lpSrcInfo->UserName, apUserID) != 0)
		{
			lpSrcInfo += ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
			continue;
		}

		if (!aIncludeDeleted && lpSrcInfo->Deleted)
		{
			lpSrcInfo += ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
			continue;
		}

		if (m_pResult->m_Count >= m_pResult->m_AllocedN)
		{
			m_pResult = ZUserQryResultAlloc(m_pResult, USERINFO_DB_DEFAULT_QRYN + 128);
		}

		lpDstInfo = ZUSER_QUERY_RS_BODY(m_pResult);
		memcpy(&lpDstInfo[m_pResult->m_Count], lpSrcInfo, sizeof(ZUserInfo));
		m_pResult->m_Count++;

		lpSrcInfo += ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
	}

	if (m_pResult->m_Count == 0)
	{
		return NULL;
	}

	return m_pResult;
}

