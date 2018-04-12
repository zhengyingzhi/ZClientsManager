#pragma once

#include <stdint.h>

#include <string>
using namespace std;

typedef struct stQueryResult
{
	volatile uint32_t RefCount;
	uint32_t Count;
	uint32_t AllocedN;
	uint32_t EntitySize;
}ZQueryResult;
#define ZDB_QRY_RS_BODY(h,type)    (type*)(h + 1)


/* refcount */
uint32_t ZQryRsIncrement(ZQueryResult* apQryRs);
uint32_t ZQryRsDecrement(ZQueryResult*& apQryRs);

/* memory allocated */
ZQueryResult* ZQryRsAlloc(ZQueryResult* apOldRs, uint32_t aAllocN, uint32_t aBodyEntitySize);

/* query compare function pointer type */
typedef bool(*ZQueryComparePtr)(const void* apExpected, const void* apAcutal, int aExtend);

/* a default query all data compare function, 
 * always return true
 */
bool ZQueryCompareNothing(const void* apExpect, const void* apAcutal, int aExtend);

class ZDataBase
{
public:
	virtual ~ZDataBase() {}

	virtual int Open(const std::string& aDBName, const std::string& ip, uint16_t port) = 0;
	virtual int Close() = 0;

	/* insert data into db */
	virtual int Insert(void* apDataInfo, uint32_t aDataSize) = 0;

	/* update data into db */
	virtual int Update(void* apDataInfo, uint32_t aDataSize) = 0;

	/* delete data from db */
	virtual int Delete(void* apDataInfo, uint32_t aDataSize) = 0;

	/* query data from db */
	virtual ZQueryResult* Query(void* apExpectInfo, ZQueryComparePtr apCompFunc, int aExtend) = 0;

public:
	/* free the query retured object */
	void FreeQueryRs(ZQueryResult*& apQryRs);

public:
	const std::string& DBName() const { return m_DBName; }

protected:
	std::string     m_DBName;
	std::string     m_DBServerIP;
	uint16_t        m_DBServerPort;

	ZQueryResult*	m_pQryRs;
};
