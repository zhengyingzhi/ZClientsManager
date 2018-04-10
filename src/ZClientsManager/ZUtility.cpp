#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ZUtility.h"


uint64_t GetCurrentTime()
{
	return time(0);
}


std::string ConvTimeStr(uint64_t aTime)
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

