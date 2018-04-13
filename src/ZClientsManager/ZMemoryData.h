#pragma once

#include <ZToolLib/ztl_palloc.h>

#include "UserInfoDB.h"
#include "StudentInfoDB.h"

using namespace std;

#define ZMD_DEFAULT_SIZE    (16 * 1024)

typedef vector<ZUserInfo*>      ZUserInfoContainer;
typedef vector<ZStudentInfo*>   ZStuInfoContainer;

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

	vector<ZUserInfo*> QueryAllUser();
	vector<ZUserInfo*> QueryUserInfo(const ZUserInfo* apExpect, ZQueryComparePtr apCompFunc);

public:
	int OpenStuDB(const string& aDBName, const string& aServerIP, uint16_t aPort);
	int CloseStuDB();
	ZDataBase* GetStuDB() { return m_pStuDB; }

	vector<ZStudentInfo*> QueryAllStudents();
	vector<ZStudentInfo*> QueryStuInfo(const ZStudentInfo* apExpect, ZQueryComparePtr apCompFunc, int aExtend);

private:
	ztl_pool_t*             m_Pool;

	ZDataBase*              m_pUserDB;
	vector<ZUserInfo*>      m_CacheUserData;

	ZDataBase*              m_pStuDB;
	vector<ZStudentInfo*>   m_CacheStuData;
};

