#include <afxwin.h>

#include <ZToolLib/ztl_utils.h>

#include "UserInfoDB.h"



ZUserInfoDBText::ZUserInfoDBText()
	: m_pBuffer(NULL)
	, m_BufSize(0)
	, m_pShmObj(NULL)
	, m_pResult(NULL)
{
	m_pResult = ZQryRsAlloc(NULL, USERINFO_DB_DEFAULT_QRYN, 
		ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT));
}

ZUserInfoDBText::~ZUserInfoDBText()
{
	if (m_pResult)
		ZQryRsDecrementAndFree(m_pResult);

	if (m_pShmObj)
		ztl_shm_release(m_pShmObj);
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

	ZQueryResult* lpResult;
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

	ZQueryResult* lpResult = Query(apUserInfo->UserName, false);
	if (lpResult)
	{
		ZUserInfo* lpUserInfo;
		lpUserInfo = ZDB_QRY_RS_BODY(lpResult, ZUserInfo);

		// market as deleted
		lpUserInfo->Deleted = 1;
	}

	return 0;
}

ZQueryResult* ZUserInfoDBText::Query(const char* apUserID, bool aIncludeDeleted)
{
	ZUserInfo* lpSrcInfo;
	ZUserInfo* lpDstInfo;

	m_pResult->Count = 0;

	lpSrcInfo = (ZUserInfo*)m_pBuffer;
	while ((char*)lpSrcInfo < (m_pBuffer + m_BufSize))
	{
		if (!lpSrcInfo->UserName[0]) {
			break;
		}

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

		if (m_pResult->Count >= m_pResult->AllocedN)
		{
			m_pResult = ZQryRsAlloc(m_pResult, USERINFO_DB_DEFAULT_QRYN + 128,
				ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT));
		}

		lpDstInfo = ZDB_QRY_RS_BODY(m_pResult, ZUserInfo);
		memcpy(&lpDstInfo[m_pResult->Count], lpSrcInfo, sizeof(ZUserInfo));
		m_pResult->Count++;

		lpSrcInfo += ztl_align(sizeof(ZUserInfo), USERINFO_DB_ALIGNMENT);
	}

	if (m_pResult->Count == 0)
	{
		return NULL;
	}

	return m_pResult;
}

