#include "ZMemoryData.h"

ZMemoryData::ZMemoryData()
	: m_pUserDB()
	, m_pQryUserRs()
	, m_pStuDB()
	, m_pQryStuRs()
{
}

ZMemoryData::~ZMemoryData()
{
	CloseUserDB();
	CloseStuDB();
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

	ZUserInfo lQryCond = {};
	m_pQryUserRs = m_pUserDB->Query(&lQryCond, ZQueryCompareNothing, 0);
	return 0;
}

int ZMemoryData::CloseUserDB()
{
	if (m_pQryUserRs)
	{
		m_pUserDB->FreeQueryRs(m_pQryUserRs);
		m_pQryUserRs = NULL;
	}

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
	lVec.reserve(128);

	if (m_pQryUserRs)
	{
		ZUserInfo* lpUserInfo;
		lpUserInfo = ZDB_QRY_RS_BODY(m_pQryUserRs, ZUserInfo);

		for (uint32_t i = 0; i < m_pQryUserRs->Count; ++i)
		{
			lVec.push_back(lpUserInfo + i);
		}
	}

	return lVec;
}

vector<ZUserInfo*> ZMemoryData::QueryUserInfo(const ZUserInfo* apExpect, ZQueryComparePtr apCompFunc)
{
	ZUserInfo* lpUserInfo;
	vector<ZUserInfo*> lVec;

	if (m_pQryUserRs)
	{
		lVec.reserve(m_pQryUserRs->Count);

		lpUserInfo = ZDB_QRY_RS_BODY(m_pQryUserRs, ZUserInfo);

		for (uint32_t i = 0; i < m_pQryUserRs->Count; ++i)
		{
			if (apCompFunc(apExpect, lpUserInfo + i, 0))
			{
				lVec.push_back(lpUserInfo + i);
			}
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

	ZStudentInfo lQryCond = {};
	m_pQryStuRs = m_pStuDB->Query(&lQryCond, ZQueryCompareNothing, 0);
	return 0;
}

int ZMemoryData::CloseStuDB()
{
	if (m_pQryStuRs)
	{
		m_pStuDB->FreeQueryRs(m_pQryStuRs);
		m_pQryStuRs = NULL;
	}

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

	if (m_pQryStuRs)
	{
		lVec.reserve(m_pQryStuRs->Count);

		ZStudentInfo* lpStuInfo;
		lpStuInfo = ZDB_QRY_RS_BODY(m_pQryStuRs, ZStudentInfo);

		for (uint32_t i = 0; i < m_pQryStuRs->Count; ++i)
		{
			lVec.push_back(lpStuInfo + i);
		}
	}

	return lVec;
}

vector<ZStudentInfo*> ZMemoryData::QueryStuInfo(const ZStudentInfo* apExpect, ZQueryComparePtr apCompFunc, int aExtend)
{
	ZStudentInfo* lpStuInfo;
	vector<ZStudentInfo*> lVec;

	if (m_pQryStuRs)
	{
		lVec.reserve(m_pQryStuRs->Count);

		lpStuInfo = ZDB_QRY_RS_BODY(m_pQryStuRs, ZStudentInfo);

		for (uint32_t i = 0; i < m_pQryStuRs->Count; ++i)
		{
			if (apCompFunc(apExpect, lpStuInfo + i, aExtend))
			{
				lVec.push_back(lpStuInfo + i);
			}
		}
	}

	return lVec;
}

