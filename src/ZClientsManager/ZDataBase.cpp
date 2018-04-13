#include "ZDataBase.h"
#include <windows.h>
#include <ZToolLib/ztl_atomic.h>

bool ZQueryCompareNothing(const void* apExpect, const void* apAcutal, int aExtend)
{
	return true;
}


ZQueryResult::ZQueryResult()
{
	m_RefCount = 0;
	m_RsVec.reserve(256);
}

ZQueryResult::~ZQueryResult()
{}

void ZQueryResult::Clear()
{
	m_RefCount = 0;
	m_RsVec.clear();
}

uint32_t ZQueryResult::IncrementRef(uint32_t aAddVal)
{
	return ztl_atomic_add(&m_RefCount, 1);
}

uint32_t ZQueryResult::DecrementRef(uint32_t aDecVal)
{
	return ztl_atomic_dec(&m_RefCount, aDecVal);
}

void ZQueryResult::PushBack(void* apDBData)
{
	m_RsVec.push_back(apDBData);
}

void* ZQueryResult::RsAt(uint32_t aIndex)
{
	if (aIndex < m_RsVec.size()) {
		return m_RsVec[aIndex];
	}
	return NULL;
}

uint32_t ZQueryResult::RsCount()
{
	return (uint32_t)m_RsVec.size();
}


//////////////////////////////////////////////////////////////////////////

ZDataBase::ZDataBase()
	: m_DBName()
	, m_DBServerIP()
	, m_DBServerPort()
	, m_pQryRs()
{
	m_pQryRs = new ZQueryResult();
}

ZDataBase::~ZDataBase()
{
	if (m_pQryRs) {
		delete m_pQryRs;
	}
}

ZQueryResult* ZDataBase::GetQueryRs()
{
	ZQueryResult* lpRetRs;
	if (m_pQryRs) {
		lpRetRs = m_pQryRs;
		m_pQryRs = NULL;
	}
	else {
		lpRetRs = new ZQueryResult();
	}

	lpRetRs->Clear();
	return lpRetRs;
}

void ZDataBase::FreeQueryRs(ZQueryResult*& apQryRs)
{
	if (!apQryRs) {
		return;
	}

	if (m_pQryRs) {
		delete apQryRs;
	}
	else {
		m_pQryRs = apQryRs;
		m_pQryRs->Clear();
	}
	apQryRs = NULL;
}
