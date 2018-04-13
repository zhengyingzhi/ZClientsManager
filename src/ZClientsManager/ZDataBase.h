#pragma once

#include <stdint.h>

#include <vector>
#include <string>
using namespace std;


/* query compare function pointer type */
typedef bool(*ZQueryComparePtr)(const void* apExpected, const void* apAcutal, int aExtend);

/* a default query all data compare function, 
 * always return true
 */
bool ZQueryCompareNothing(const void* apExpect, const void* apAcutal, int aExtend);

class ZQueryResult
{
public:
	ZQueryResult();
	~ZQueryResult();

	void Clear();

public:
	uint32_t IncrementRef(uint32_t aAddVal = 1);
	uint32_t DecrementRef(uint32_t aDecVal = 1);

public:
	void  PushBack(void* apDBData);

	void* RsAt(uint32_t aIndex);

	template<typename Ty>
	Ty* RsAtAsType(uint32_t aIndex) {
		return (Ty*)RsAt(aIndex);
	}

	uint32_t RsCount();

public:
	uint32_t        m_RefCount;
	vector<void*>   m_RsVec;
};

class ZDataBase
{
public:
	ZDataBase();
	virtual ~ZDataBase();

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
	/* tye get the cached query result rs */
	ZQueryResult* GetQueryRs();

	/* free the query retured object */
	void FreeQueryRs(ZQueryResult*& apQryRs);

public:
	const std::string& DBName() const { return m_DBName; }

protected:
	std::string     m_DBName;
	std::string     m_DBServerIP;
	uint16_t        m_DBServerPort;

	ZQueryResult*   m_pQryRs;
};
