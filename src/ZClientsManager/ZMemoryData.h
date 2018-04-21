#pragma once

#include <ZToolLib/ztl_palloc.h>

#include "UserInfoDB.h"
#include "StudentInfoDB.h"

#include "ZNetProtocol.h"

using namespace std;

#define ZMD_DEFAULT_SIZE    (16 * 1024)

typedef vector<ZUserInfo*>      ZUserInfoContainer;
typedef vector<ZStudentInfo*>   ZStuInfoContainer;

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

public:
	int OpenStuDB(const string& aDBName, const string& aServerIP, uint16_t aPort);
	int CloseStuDB();
	ZDataBase* GetStuDB() { return m_pStuDB; }

	uint32_t StuCount() { return m_CacheStuData.size(); }

	vector<ZStudentInfo*> QueryAllStudents();
	vector<ZStudentInfo*> QueryStuInfo(const ZStudentInfo* apExpect, ZQueryComparePtr apCompFunc, int aExtend);

	void AddStuInfo(const ZStudentInfo* apStuInfo);

private:
	ztl_pool_t*             m_Pool;

	ZDataBase*              m_pUserDB;
	vector<ZUserInfo*>      m_CacheUserData;

	ZDataBase*              m_pStuDB;
	vector<ZStudentInfo*>   m_CacheStuData;
};

