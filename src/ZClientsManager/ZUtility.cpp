#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <functional>
#include <sstream>

#include <Windows.h>

#include "ZUtility.h"

#include <ZToolLib/ztl_base64.h>
#include <ZToolLib/ztl_utils.h>


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
	if (aTime == 0) {
		return "";
	}

	char lBuffer[64] = "";
	time_t lTime = (time_t)aTime;
	struct tm ltm;
	localtime_s(&ltm, &lTime);
	sprintf(lBuffer, "%04d-%02d-%02d %02d:%02d:%02d",
		ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
		ltm.tm_hour, ltm.tm_min, ltm.tm_sec);

	return std::string(lBuffer);
}

void ZLog2DebugView(const char* fmt, ...)
{
	std::string lNowStr = ZConvStdTimeStr(time_t(0));

	int  lLength = 0;
	char lBuffer[4000] = "";

	strcpy(lBuffer + lLength, lNowStr.c_str());
	lLength += lNowStr.length();

	va_list args;
	va_start(args, fmt);
	lLength += vsnprintf(lBuffer + lLength, sizeof(lBuffer) - lLength - 2, fmt, args);
	va_end(args);

	lBuffer[lLength++] = '\r';
	lBuffer[lLength++] = '\n';

	OutputDebugString(lBuffer);
}


char* ZPasswordChange(char* apData)
{
	unsigned char *p;
	unsigned char uch, umask, index = 0;
	p = (unsigned char *)apData;
	while ((uch = *p))
	{
		umask = 0xFF - index;
		*p++ = uch  ^ umask;
		index++;

		if (index > 8)
			index = 0;
	}

	return apData;
}

std::string ZConvDataToBase64(const char* apRawData, uint32_t aRawSize, bool aDoSimpleChange)
{
	char lRawData[4096] = "";
	memcpy(lRawData, apRawData, aRawSize);
	if (aDoSimpleChange) {
		//apRawData = ZPasswordChange(lRawData);
	}

	char lOutData[4096] = "";
	uint32_t lOutSize = sizeof(lOutData);
	ztl_base64_encode(lRawData, aRawSize, lOutData, &lOutSize);

	return std::string(lOutData);
}

std::string ZConvBase64ToData(const char* apBaseData, uint32_t aBaseDataSize, bool aDoSimpleChange)
{
	char lOutData[4096] = "";
	uint32_t lOutSize = sizeof(lOutData);
	ztl_base64_decode(apBaseData, strlen(apBaseData), lOutData, &lOutSize);

	if (aDoSimpleChange) {
		//apBaseData = ZPasswordChange(lOutData);
	}

	return std::string(lOutData);
}

uint32_t ZGetCheckSum(const void* apRawData, uint32_t aRawSize, int aRefNum)
{
	if (aRefNum <= 7)
		aRefNum = 10;

	uint32_t lModNum = 1;
	while (aRefNum--)
		lModNum = lModNum << 1;

	uint32_t lIndex = 0;
	uint32_t lSum = 0;
	unsigned char* lpRawData = (unsigned char*)apRawData;
	while (lIndex < aRawSize) {
		lSum += uint32_t(lpRawData[lIndex++]);
	}

	return lSum & (lModNum - 1);
}

std::vector<std::string>
ZStringSplit(const std::string& subject, char delimiter)
{
	std::vector<std::string> items;
	std::istringstream stream(subject);
	std::string item;
	while (std::getline(stream, item, delimiter))
		items.push_back(std::move(item));
	return items;
}
