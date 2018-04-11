#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ZUtility.h"

uint32_t ZCurrentDate()
{
	time_t lNow = time(0);
	struct tm ltm;
	localtime_s(&ltm, &lNow);
	return (ltm.tm_year + 1900) * 10000 + (ltm.tm_mon + 1) * 100 + ltm.tm_mday;
}


uint32_t ZCurrentTime()
{
	time_t lNow = time(0);
	struct tm ltm;
	localtime_s(&ltm, &lNow);
	return ltm.tm_hour * 10000 + ltm.tm_min * 100 + ltm.tm_sec;
}

uint64_t ZCurrentDateTime()
{
	uint64_t lDateTime;
	lDateTime = ZCurrentDate();
	lDateTime *= 100000000;
	lDateTime += ZCurrentTime();
	return lDateTime;
}


std::string ZConvDateTimeStr(uint64_t aTime)
{
	char lBuffer[64] = "";

	uint32_t lDate = uint32_t(aTime / 100000000);
	uint32_t lTime = uint32_t(aTime % 100000000);

	sprintf(lBuffer, "%d %d", lDate, lTime);

	return std::string(lBuffer);
}

std::string ZConvStdTimeStr(time_t aTime)
{
	char lBuffer[64] = "";
	time_t lTime = (time_t)aTime;
	struct tm ltm;
	localtime_s(&ltm, &lTime);
	sprintf(lBuffer, "%04d-%02d-%02d %02d:%02d:%02d",
		ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
		ltm.tm_hour, ltm.tm_min, ltm.tm_sec);

	return std::string(lBuffer);
}

