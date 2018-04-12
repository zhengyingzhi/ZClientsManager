#pragma once

#include <stdint.h>

typedef struct stQueryResult
{
	void(*Cleanup)(struct stQueryResult* apRS);
	volatile uint32_t RefCount;
	uint32_t Count;
	uint32_t AllocedN;
	uint32_t EntitySize;
}ZQueryResult;
#define ZDB_QRY_RS_BODY(h,type)    (type*)(h + 1)


/* refcount */
uint32_t ZQryRsIncrement(ZQueryResult* apQryRs);
uint32_t ZQryRsDecrementAndFree(ZQueryResult*& apQryRs);

/* memory allocated */
ZQueryResult* ZQryRsAlloc(ZQueryResult* apOldRs, uint32_t aAllocN, uint32_t aBodyEntitySize);

