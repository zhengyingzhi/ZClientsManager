#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ZToolLib/ztl_shm.h>

#include "StudentInfo.h"
#include "ZDataBase.h"


#define TASKINFO_DB_DEFAULT_NAME    "ZTaskInfo.db"
#define TASKINFO_DB_DEFAULT_SIZE    (4 * 1024)
#define TASKINFO_DB_DEFAULT_QRYN    8
#define TASKINFO_DB_ALIGNMENT       64

#define ZTASKTIME_CMP_EQUAL         0
#define ZTASKTIME_CMP_BiggerThan    1
#define ZTASKTIME_CMP_SmallerThan   2

typedef struct tagZTaskInfo
{
	uint32_t   TaskTime;
	uint32_t   Flag;
	char       TaskName[120];
	char       TaskContent[128];
}ZTaskInfo;

/* some default compare functions */
bool ZQueryCompareTaskAll(const void* apExpect, const void* apAcutal, int aExtend);
bool ZQueryCompareTaskTime(const void* apExpect, const void* apAcutal, int aExtend);

/* task info db implement by text file
*/
class ZTaskInfoDBText : public ZDataBase
{
public:
	ZTaskInfoDBText();
	virtual ~ZTaskInfoDBText();

public:
	virtual int Open(const std::string& aDBName, const std::string& ip, uint16_t port);
	virtual int Close();

	virtual int Insert(void* apDataInfo, uint32_t aDataSize);
	virtual int Update(void* apDataInfo, uint32_t aDataSize);
	virtual int Delete(void* apDataInfo, uint32_t aDataSize);

	virtual ZQueryResult* Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend);

protected:
	ZTaskInfo* NextTaskInfo(ZTaskInfo* apCurTaskInfo, bool aAutoExpand = false);
	ZTaskInfo* GetAvailTaskInfo();

private:
	int PrivTaskShmCreate();

protected:
	char*       m_pBuffer;
	uint32_t    m_BufSize;

	ztl_shm_t*  m_pShmObj;
};
