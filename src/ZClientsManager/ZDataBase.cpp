#include "ZDataBase.h"
#include <windows.h>
#include <ZToolLib/ztl_atomic.h>

uint32_t ZQryRsIncrement(ZQueryResult* apQryRs)
{
	return ztl_atomic_add(&apQryRs->RefCount, 1);
}

uint32_t ZQryRsDecrement(ZQueryResult* apQryRs)
{
	uint32_t lNewValue = ztl_atomic_dec(&apQryRs->RefCount, 1);

	return lNewValue;
}


static void _ZQryRsCleanup(struct stQueryResult* apRS)
{
	free(apRS);
}

ZQueryResult* ZQryRsAlloc(ZQueryResult* apOldRs, uint32_t aAllocN, uint32_t aBodyEntitySize)
{
	if (apOldRs) {
		apOldRs = (ZQueryResult*)realloc(apOldRs, sizeof(ZQueryResult) + aAllocN * aBodyEntitySize);
	}
	else {
		apOldRs = (ZQueryResult*)calloc(1, sizeof(ZQueryResult) + aAllocN * aBodyEntitySize);
	}

	apOldRs->RefCount = 1;
	apOldRs->AllocedN = aAllocN;
	apOldRs->EntitySize = aBodyEntitySize;

	return apOldRs;
}

bool ZQueryCompareNothing(const void* apExpect, const void* apAcutal, int aExtend)
{
	return true;
}

void ZDataBase::FreeQueryRs(ZQueryResult*& apQryRs)
{
	if (!apQryRs) {
		return;
	}

	if (m_pQryRs) {
		_ZQryRsCleanup(apQryRs);
	}
	else {
		m_pQryRs = apQryRs;
		m_pQryRs->Count = 0;
	}
	apQryRs = NULL;
}
