#include "ZMemoryData.h"

extern ZNetCommBase*	g_pNetComm;

ZMemoryData::ZMemoryData()
	: m_UserLock()
	, m_pUserDB()
	, m_StuLock()
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
	ZLockScope lk(&m_UserLock);
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
	ZLockScope lk(&m_UserLock);

	vector<ZUserInfo*> lVec;
	lVec.reserve(m_CacheUserData.size());

	lVec.assign(m_CacheUserData.begin(), m_CacheUserData.end());
	return lVec;
}

vector<ZUserInfo*> ZMemoryData::QueryUserInfo(const ZUserInfo* apExpect, ZQueryComparePtr apCompFunc)
{
	ZLockScope lk(&m_UserLock);

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

void ZMemoryData::AddUserInfo(const ZUserInfo* apUserInfo)
{
	ZLockScope lk(&m_UserLock);

	ZUserInfo* lpDstData;
	lpDstData = (ZUserInfo*)ztl_pcalloc(m_Pool, sizeof(ZUserInfo));
	memcpy(lpDstData, apUserInfo, sizeof(ZUserInfo));
	m_CacheUserData.push_back(lpDstData);
}

void ZMemoryData::AddOrUpdateUserInfo(const ZUserInfo* apUserInfo)
{
	// todo: add or update
}


//////////////////////////////////////////////////////////////////////////

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
	ZLockScope lk(&m_StuLock);

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
	ZLockScope lk(&m_StuLock);

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

	ZLockScope lk(&m_StuLock);

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

vector<ZStudentInfo*> ZMemoryData::QueryStuInfoVague(const char* apFindStr)
{
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;
	lVec.reserve(128);

	ZLockScope lk(&m_StuLock);

	for (size_t i = 0; i < m_CacheStuData.size(); ++i)
	{
		lpStuInfo = m_CacheStuData[i];

		if (strcmp(lpStuInfo->Name, apFindStr) == 0 ||
			strcmp(lpStuInfo->QQ, apFindStr) == 0 ||
			strcmp(lpStuInfo->Class, apFindStr) == 0 ||
			strstr(lpStuInfo->CollegeFrom, apFindStr) ||
			strstr(lpStuInfo->CollegeTo, apFindStr) ||
			strstr(lpStuInfo->MajorFrom, apFindStr) ||
			strstr(lpStuInfo->MajorTo, apFindStr) ||
			strstr(lpStuInfo->Country, apFindStr) ||
			strstr(lpStuInfo->Status, apFindStr) || 
			strcmp(lpStuInfo->IDNumber, apFindStr) == 0)
		{
			lVec.push_back(lpStuInfo);
		}
	}

	return lVec;
}


void ZMemoryData::AddStuInfo(const ZStudentInfo* apStuInfo)
{
	ZLockScope lk(&m_StuLock);

	int rv;

	ZStudentInfo* lpDstData;
	lpDstData = (ZStudentInfo*)ztl_pcalloc(m_Pool, sizeof(ZStudentInfo));
	memcpy(lpDstData, apStuInfo, sizeof(ZStudentInfo));
	m_CacheStuData.push_back(lpDstData);

	if (g_pNetComm)
	{
		// make a net message packet, and send out
		ZNetMessage* lpMessage;
		lpMessage = ZNetProtocol::MakeNetMessage(ZNET_T_PublishAdd, ZNET_MSG_StuInfo, apStuInfo, sizeof(ZStudentInfo));

		rv = g_pNetComm->DirectSend(lpMessage->GetRawBegin(), lpMessage->Size());
		if (rv < 0)
		{
			char lErrorMsg[512] = "";
			sprintf(lErrorMsg, "AddStuInfo DirectSend() failed %d", get_errno());
			OutputDebugString(lErrorMsg);
		}

		ZNetMessage::Release(lpMessage);
	}
}


void ZMemoryData::AddOrUpdateStuInfo(const ZStudentInfo* apStuInfo)
{}
