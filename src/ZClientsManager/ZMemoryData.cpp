#include "ZMemoryData.h"

#include "ZNetProtocol.h"

#include "ZUtility.h"

#include "ZAppConfig.h"


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
		ztl_log_error(g_Logger, ZTL_LOG_CRITICAL, "OpenUserDB failed rv:%d", rv);

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

vector<ZUserInfo*> ZMemoryData::QueryAllUser(BOOL aWithDeleted)
{
	ZLockScope lk(&m_UserLock);

	vector<ZUserInfo*> lVec;
	lVec.reserve(m_CacheUserData.size());

	for (size_t i = 0; i < m_CacheUserData.size(); ++i)
	{
		if (!aWithDeleted && m_CacheUserData[i]->Flags & ZUSER_FLAG_Deleted) {
			continue;
		}

		lVec.push_back(m_CacheUserData[i]);
	}

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


void ZMemoryData::AddOrUpdateUserInfo(uint32_t aType, const ZUserInfo* apUserInfo)
{
	vector<ZUserInfo*> lVec = QueryUserInfo(apUserInfo, ZQueryCompareUserName);

	ZLockScope lk(&m_UserLock);
	if (!lVec.empty())
	{
		ZUserInfoCopy(lVec[0], apUserInfo);
	}
	else
	{
		ZUserInfo* lpDstData;
		lpDstData = (ZUserInfo*)ztl_pcalloc(m_Pool, sizeof(ZUserInfo));
		memcpy(lpDstData, apUserInfo, sizeof(ZUserInfo));
		m_CacheUserData.push_back(lpDstData);
	}
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
	lpQryRs = m_pStuDB->Query(&lQryCond, ZQueryCompareNothing, CC_WithDeleted);
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

vector<ZStudentInfo*> ZMemoryData::QueryAllStudents(BOOL aWithDeleted)
{
	ZLockScope lk(&m_StuLock);

	vector<ZStudentInfo*> lVec;
	lVec.reserve(m_CacheStuData.size());

	for (size_t i = 0; i < m_CacheStuData.size(); ++i)
	{
		if (!aWithDeleted && m_CacheStuData[i]->Flag & ZSI_FLAG_Deleted) {
			continue;
		}
		lVec.push_back(m_CacheStuData[i]);
	}
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

/* 模糊查询学生信息 */
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


void ZMemoryData::AddStuInfo(const ZStudentInfo* apStuInfo, BOOL aPublishToNet)
{
	ZLockScope lk(&m_StuLock);

	int rv;

	ZStudentInfo* lpDstData;
	lpDstData = (ZStudentInfo*)ztl_pcalloc(m_Pool, sizeof(ZStudentInfo));
	memcpy(lpDstData, apStuInfo, sizeof(ZStudentInfo));
	m_CacheStuData.push_back(lpDstData);

	if (aPublishToNet && g_pNetComm)
	{
		// make a net message packet, and send out
		char lMessagePacket[4096] = "";
		int  lLength = ZStuInfoFixString(apStuInfo, lMessagePacket, ZNetProtocol::NetMessagePreSize());

		ZNetMessage* lpMessage;
		lpMessage = ZNetProtocol::MakeNetMessage(ZNET_T_PublishAdd, ZNET_MSG_StuInfo, lMessagePacket, lLength);

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

void ZMemoryData::UpdateStuInfo(ZStudentInfo* apTobeUpdated, const ZStudentInfo* apNewStuInfo)
{
	if (apTobeUpdated == NULL)
	{
		vector<ZStudentInfo*> lVec;
		lVec = QueryStuInfo(apNewStuInfo, ZQueryCompareNameAndTel, 0);
		if (!lVec.empty())
		{
			ZStuInfoCopy(lVec[0], apNewStuInfo);
		}
	}
	else
	{
		ZLockScope lk(&m_StuLock);
		ZStuInfoCopy(apTobeUpdated, apNewStuInfo);
	}
}


void ZMemoryData::AddOrUpdateStuInfo(uint32_t aType, const ZStudentInfo* apStuInfo)
{
	// search firstly and add or update
	vector<ZStudentInfo*> lVec;
	lVec = QueryStuInfo(apStuInfo, ZQueryCompareNameAndTel, CC_WithDeleted);

	if (lVec.empty())
	{
		// insert new
		m_pStuDB->Insert((void*)apStuInfo, sizeof(ZStudentInfo));

		AddStuInfo(apStuInfo, TRUE);
	}
	else
	{
		// try check data or update 
		//uint32_t lLocalCS = ZGetCheckSum(lVec[0], sizeof(ZStudentInfo), 0);
		//uint32_t lNewCS = ZGetCheckSum(apStuInfo, sizeof(ZStudentInfo), 0);
		//if (lLocalCS == lNewCS)
		if (strcmp(apStuInfo->Name, lVec[0]->Name) == 0 &&
			strcmp(apStuInfo->Telehone, lVec[0]->Telehone) == 0 &&
			apStuInfo->UpdateTime == lVec[0]->UpdateTime)
		{
			ztl_log_error(g_Logger, ZTL_LOG_DEBUG, "ZMemoryData::AddOrUpdateStuInfo same checksum for name:%s", apStuInfo->Name);
			return;
		}

		m_pStuDB->Update((void*)apStuInfo, sizeof(ZStudentInfo));

		UpdateStuInfo(lVec[0], apStuInfo);
	}
}
