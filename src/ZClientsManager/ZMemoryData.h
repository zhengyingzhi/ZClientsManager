#pragma once

#include <ZToolLib/ztl_palloc.h>

#include <ZToolLib/ztl_locks.h>

#include "UserInfoDB.h"
#include "StudentInfoDB.h"

#include "ZNetProtocol.h"

using namespace std;

#define ZMD_DEFAULT_SIZE    (16 * 1024)

#define ZMD_DATA_Add        1
#define ZMD_DATA_Update     2

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

/* 先将数据从数据库/网络上加载到该缓存中,
 * 本地操作数据时，再从该缓存中获取数据
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

	vector<ZUserInfo*> QueryAllUser(BOOL aWithDeleted = FALSE);
	vector<ZUserInfo*> QueryUserInfo(const ZUserInfo* apExpect, ZQueryComparePtr apCompFunc);

	/* 添加或更新数据 */
	void AddOrUpdateUserInfo(uint32_t aType, const ZUserInfo* apUserInfo, BOOL aPublishToNet = TRUE);

public:
	int OpenStuDB(const string& aDBName, const string& aServerIP, uint16_t aPort);
	int CloseStuDB();
	ZDataBase* GetStuDB() { return m_pStuDB; }

	uint32_t StuCount() { return m_CacheStuData.size(); }

	vector<ZStudentInfo*> QueryAllStudents(BOOL aWithDeleted = FALSE);
	vector<ZStudentInfo*> QueryStuInfo(const ZStudentInfo* apExpect, ZQueryComparePtr apCompFunc, int aExtend);
	vector<ZStudentInfo*> QueryStuInfoVague(const char* apFindStr);

	void AddStuInfo(const ZStudentInfo* apStuInfo, BOOL aPublishToNet);
	void UpdateStuInfo(ZStudentInfo* apTobeUpdated, const ZStudentInfo* apNewStuInfo, BOOL aPublishToNet = TRUE);

	/* 添加或更新数据 */
	void AddOrUpdateStuInfo(uint32_t aType, const ZStudentInfo* apStuInfo, BOOL aPublishToNet = TRUE);

	/* 更新到网络中 */
	int PublishToNetwork(const ZStudentInfo* apStuInfo, uint32_t aType);

private:
	ztl_pool_t*             m_Pool;

	volatile uint32_t       m_UserLock;
	ZDataBase*              m_pUserDB;
	vector<ZUserInfo*>      m_CacheUserData;

	volatile uint32_t		m_StuLock;
	ZDataBase*              m_pStuDB;
	vector<ZStudentInfo*>   m_CacheStuData;
};

