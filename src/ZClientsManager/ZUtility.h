#pragma once

#include <string>

using namespace std;

/* return time format 20180401094032 */
uint64_t ZCurrentDateTime();

uint32_t ZCurrentDate();
uint32_t ZCurrentTime();

/* convert time to string */
std::string ZConvDateTimeStr(uint64_t aTime);
std::string ZConvStdTimeStr(time_t aTime);
