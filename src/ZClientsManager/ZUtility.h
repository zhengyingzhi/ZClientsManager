#pragma once

#include <string>
#include <vector>

using namespace std;

/* return time format 20180401094032 */
uint64_t ZCurrentDateTime();

uint32_t ZCurrentDate();
uint32_t ZCurrentTime();

/* convert time to string */
std::string ZConvDateTimeStr(uint64_t aTime);
std::string ZConvStdTimeStr(time_t aTime);

std::string ZConvStr2StdDate(uint64_t aTime);

time_t ZConvStr2StdTime(const std::string& aTimeStr);

void ZLog2DebugView(const char* fmt, ...);
#define ZDebug(fmt,...) ZLog2DebugView(fmt,__VA_ARGS__)


/* simple password change */
char* ZPasswordChange(char* apData);

/* convert to base64 */
std::string ZConvDataToBase64(const char* apRawData, uint32_t aRawSize, bool aDoSimpleChange);

std::string ZConvBase64ToData(const char* apBaseData, uint32_t aBaseDataSize, bool aDoSimpleChange);

uint32_t ZGetCheckSum(const void* apRawData, uint32_t aRawSize, int aRefNum);

std::vector<std::string> ZStringSplit(const std::string& subject, char delimiter);

