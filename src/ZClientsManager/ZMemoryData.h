#pragma once

#include <vector>

#include "UserInfoDB.h"
#include "StudentInfoDB.h"

using namespace std;

#define ZMD_DEFAULT_SIZE    (16 * 1024)

typedef vector<ZUserInfo*>      ZUserInfoContainer;
typedef vector<ZStudentInfo*>   ZStuInfoContainer;

/* �Ƚ����ݴ����ݿ�/�����ϼ��ص���������,
 * ���ز�������ʱ���ٴӸ������л�ȡ����
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
	ZDataBase*      m_pUserDB;
	ZQueryResult*   m_pQryUserRs;

	ZDataBase*      m_pStuDB;
	ZQueryResult*   m_pQryStuRs;
};

