#pragma once

#include <ZToolLib/ztl_palloc.h>

#include <ZToolLib/ztl_locks.h>

#include "UserInfoDB.h"
#include "StudentInfoDB.h"

#include "ZNetProtocol.h"

using namespace std;

#define ZMD_DEFAULT_SIZE    (16 * 1024)

typedef vector<ZUserInfo*>      ZUserInfoContainer;
typedef vector<ZStudentInfo*>   ZStuInfoContainer;

class ZLockScope
{
public:
	ZLockScope(volatile uint32_t* apLock) : m_pLock(apLock) {
		ztl_spinlock(apLock, 1, 2048);
	}
	~ZLockScope() {
		ztl_unlock(m_pLock);
	}

	volatile uint32_t* m_pLock;
};

/* �Ƚ����ݴ����ݿ�/�����ϼ��ص��û�����,
 * ���ز�������ʱ���ٴӸû����л�ȡ����
 */
class ZMemoryData
{
public:
	ZMemoryData();
	~ZMemoryData();

public:
	int OpenUserDB(const string& aDBName, const string& aServerIP, uint16_t aPort);
	int CloseUserDB();
	ZDataBase* GetUserDB() { return m_pUserDB; }

	uint32_t UserCount() { return m_CacheUserData.size(); }

	vector<ZUserInfo*> QueryAllUser();
	vector<ZUserInfo*> QueryUserInfo(const ZUserInfo* apExpect, ZQueryComparePtr apCompFunc);

	void AddUserInfo(const ZUserInfo* apUserInfo);

	/* ��ӻ�������� */
	void AddOrUpdateUserInfo(const ZUserInfo* apUserInfo);

public:
	int OpenStuDB(const string& aDBName, const string& aServerIP, uint16_t aPort);
	int CloseStuDB();
	ZDataBase* GetStuDB() { return m_pStuDB; }

	uint32_t StuCount() { return m_CacheStuData.size(); }

	vector<ZStudentInfo*> QueryAllStudents();
	vector<ZStudentInfo*> QueryStuInfo(const ZStudentInfo* apExpect, ZQueryComparePtr apCompFunc, int aExtend);
	vector<ZStudentInfo*> QueryStuInfoVague(const char* apFindStr);

	void AddStuInfo(const ZStudentInfo* apStuInfo);

	/* ��ӻ�������� */
	void AddOrUpdateStuInfo(const ZStudentInfo* apStuInfo);

private:
	ztl_pool_t*             m_Pool;

	volatile uint32_t		m_UserLock;
	ZDataBase*              m_pUserDB;
	vector<ZUserInfo*>      m_CacheUserData;

	volatile uint32_t		m_StuLock;
	ZDataBase*              m_pStuDB;
	vector<ZStudentInfo*>   m_CacheStuData;
};

