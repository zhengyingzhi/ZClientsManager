#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZToolLib/ztl_shm.h>

#include "StudentInfo.h"
#include "ZDataBase.h"


#define STUINFO_DB_DEFAULT_NAME     "StudentInfo.db"
#define STUINFO_DB_DEFAULT_SIZE     (1 * 1024 * 1024)
#define STUINFO_DB_DEFAULT_QRYN     512
#define STUINFO_DB_ALIGNMENT        1024

typedef enum
{
	CC_Equal = 0,
	CC_BiggerThan = 1,
	CC_SmallerThan = 2,
	CC_WithDeleted = 3
}ZCompareCond;


/* some default compare functions */
bool ZQueryCompareNumber(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareNameAndTel(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareStuQQ(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareCountry(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareCollege(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareCollege(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareScore(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareStatus(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareSource(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareEMail(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareStuIDNum(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareStuTime(const void* apExpect, const void* apAcutal, int aExtend);

/* student info db implement by text file
 */
class ZStudentInfoDBText : public ZDataBase
{
public:
	ZStudentInfoDBText();
	virtual ~ZStudentInfoDBText();

public:
	virtual int Open(const std::string& aDBName, const std::string& ip, uint16_t port);
	virtual int Close();

	virtual int Insert(void* apDataInfo, uint32_t aDataSize);
	virtual int Update(void* apDataInfo, uint32_t aDataSize);
	virtual int Delete(void* apDataInfo, uint32_t aDataSize);

	virtual ZQueryResult* Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend);

protected:
	ZStudentInfo* NextStudentInfo(ZStudentInfo* apCurStuInfo, bool aAutoExpand = false);
	ZStudentInfo* GetAvailStudentInfo();

private:
	int PrivStuShmCreate();

protected:
	char*       m_pBuffer;
	uint32_t    m_BufSize;

	ztl_shm_t*  m_pShmObj;

protected:
	bool m_IncludeDeleted;
};

