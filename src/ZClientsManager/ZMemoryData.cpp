#include "ZMemoryData.h"

ZMemoryData::ZMemoryData()
	: m_pUserDB()
	, m_pStuDB()
{
	m_Pool = ztl_create_pool(ZMD_DEFAULT_SIZE);
}

ZMemoryData::~ZMemoryData()
{
	CloseUserDB();
	CloseStuDB();

	if (m_Pool)
		ztl_destroy_pool(m_Pool);
}

int ZMemoryData::OpenUserDB(const string& aDBName, const string& aServerIP, uint16_t aPort)
{
	int rv;
	m_pUserDB = new ZUserInfoDBText();
	rv = m_pUserDB->Open(aDBName, aServerIP, aPort);
	if (0 != rv)
	{
		delete m_pUserDB;
		m_pUserDB = NULL;
		return rv;
	}

	// query all when opened db
	ZQueryResult*   lpQryRs;
	ZUserInfo       lQryCond = {};
	lpQryRs = m_pUserDB->Query(&lQryCond, ZQueryCompareNothing, 0);

	if (lpQryRs)
	{
		ZUserInfo *lpSrcData, *lpDstData;
		for (uint32_t i = 0; i < lpQryRs->RsCount(); ++i)
		{
			lpSrcData = lpQryRs->RsAtAsType<ZUserInfo>(i);
			if (!lpSrcData)
			{
				continue;
			}

			lpDstData = (ZUserInfo*)ztl_pcalloc(m_Pool, sizeof(ZUserInfo));
			memcpy(lpDstData, lpSrcData, sizeof(ZUserInfo));
			m_CacheUserData.push_back(lpDstData);
		}
	}

	m_pUserDB->FreeQueryRs(lpQryRs);
	return 0;
}

int ZMemoryData::CloseUserDB()
{
	if (m_pUserDB)
	{
		m_pUserDB->Close();
		delete m_pUserDB;
		m_pUserDB = NULL;
	}
	return 0;
}

vector<ZUserInfo*> ZMemoryData::QueryAllUser()
{
	vector<ZUserInfo*> lVec;
	lVec.reserve(m_CacheUserData.size());

	lVec.assign(m_CacheUserData.begin(), m_CacheUserData.end());
	return lVec;
}

vector<ZUserInfo*> ZMemoryData::QueryUserInfo(const ZUserInfo* apExpect, ZQueryComparePtr apCompFunc)
{
	ZUserInfo* lpUserInfo;
	vector<ZUserInfo*> lVec;
	lVec.reserve(128);

	for (size_t i = 0; i < m_CacheUserData.size(); ++i)
	{
		lpUserInfo = m_CacheUserData[i];
		if (apCompFunc(apExpect, lpUserInfo, 0))
		{
			lVec.push_back(lpUserInfo);
		}
	}

	return lVec;
}


int ZMemoryData::OpenStuDB(const string& aDBName, const string& aServerIP, uint16_t aPort)
{
	m_pStuDB = new ZStudentInfoDBText();
	int rv = m_pStuDB->Open(aDBName, aServerIP, aPort);
	if (0 != rv)
	{
		delete m_pStuDB;
		m_pStuDB = NULL;
		return rv;
	}

	ZQueryResult*   lpQryRs;
	ZStudentInfo    lQryCond = {};
	lpQryRs = m_pStuDB->Query(&lQryCond, ZQueryCompareNothing, 0);
	if (lpQryRs)
	{
		ZStudentInfo *lpSrcData, *lpDstData;
		for (uint32_t i = 0; i < lpQryRs->RsCount(); ++i)
		{
			lpSrcData = lpQryRs->RsAtAsType<ZStudentInfo>(i);
			if (!lpSrcData)
			{
				continue;
			}

			lpDstData = (ZStudentInfo*)ztl_pcalloc(m_Pool, sizeof(ZStudentInfo));
			memcpy(lpDstData, lpSrcData, sizeof(ZStudentInfo));
			m_CacheStuData.push_back(lpDstData);
		}
	}
	return 0;
}

int ZMemoryData::CloseStuDB()
{
	if (m_pStuDB)
	{
		m_pStuDB->Close();
		delete m_pStuDB;
		m_pStuDB = NULL;
	}
	return 0;
}

vector<ZStudentInfo*> ZMemoryData::QueryAllStudents()
{
	vector<ZStudentInfo*> lVec;
	lVec.reserve(m_CacheStuData.size());

	lVec.assign(m_CacheStuData.begin(), m_CacheStuData.end());
	return lVec;
}

vector<ZStudentInfo*> ZMemoryData::QueryStuInfo(const ZStudentInfo* apExpect, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;
	lVec.reserve(128);

	for (size_t i = 0; i < m_CacheStuData.size(); ++i)
	{
		lpStuInfo = m_CacheStuData[i];
		if (apCompFunc(apExpect, lpStuInfo, aExtend))
		{
			lVec.push_back(lpStuInfo);
		}
	}

	return lVec;
}

