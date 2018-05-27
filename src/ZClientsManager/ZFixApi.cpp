#include <ZToolLib/ztl_utils.h>

#include "ZFixApi.h"

int zfix_str_delimiter(char* apSrc, int aLength, char** apRetArr, int aArrSize, char aDelimiter);

ZFixApi::ZFixApi()
	: m_Buffer()
	, m_pBuff(m_Buffer)
	, m_Length()
	, m_Capacity(sizeof(m_Buffer))
{}

ZFixApi::~ZFixApi()
{}

void ZFixApi::Clear()
{
	m_pBuff = m_Buffer;
	m_Length = 0;
	m_FixMap.clear();
}

uint32_t ZFixApi::AvailSize()
{
	if (m_Capacity <= m_Length)
		return 0;
	return m_Capacity - m_Length;
}

void ZFixApi::SetBuffer(char* apRawBuffer)
{
	m_pBuff = apRawBuffer;
}

void ZFixApi::SetLength(uint32_t aRawLength)
{
	m_Length = aRawLength;
	m_Capacity = aRawLength;
}


char* ZFixApi::Data() const
{
	return m_pBuff;
}

uint32_t ZFixApi::Length() const
{
	return m_Length;
}


/* -------- setter ---------*/
void ZFixApi::SetItem(uint32_t aID, char aValue)
{
	m_Length += snprintf(m_pBuff + Length(), AvailSize(), "%d=%c%c", aID, aValue, ZFIX_DELIMITER);
}

void ZFixApi::SetItem(uint32_t aID, const uint16_t aValue)
{
	m_Length += snprintf(m_pBuff + Length(), AvailSize(), "%d=%d%c", aID, aValue, ZFIX_DELIMITER);
}

void ZFixApi::SetItem(uint32_t aID, const int32_t aValue)
{
	m_Length += snprintf(m_pBuff + Length(), AvailSize(), "%d=%d%c", aID, aValue, ZFIX_DELIMITER);
}

void ZFixApi::SetItem(uint32_t aID, const uint32_t aValue)
{
	m_Length += snprintf(m_pBuff + Length(), AvailSize(), "%d=%u%c", aID, aValue, ZFIX_DELIMITER);
}

void ZFixApi::SetItem(uint32_t aID, int64_t aValue)
{
	m_Length += snprintf(m_pBuff + Length(), AvailSize(), "%d=%lld%d", aID, aValue, ZFIX_DELIMITER);
}

void ZFixApi::SetItem(uint32_t aID, const char* apValue, int aLength)
{
	if (aLength < 0)
		aLength = strlen(apValue);

	if (AvailSize() <= (uint32_t)aLength) {
		return;
	}

	m_Length += snprintf(m_pBuff + Length(), AvailSize(), "%d=", aID);
	memcpy(m_pBuff + m_Length, apValue, aLength);
	m_Length += aLength;
	m_pBuff[m_Length] = ZFIX_DELIMITER;
	m_Length += 1;
}

void ZFixApi::SetItem(uint32_t aID, const std::string& aValue)
{
	SetItem(aID, aValue.c_str(), aValue.length());
}


/* -------- getter ---------*/
bool ZFixApi::GetItem(uint32_t aID, char& aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	aOutValue = atoi(GetValueFromMap(aID));
	return true;
}

bool ZFixApi::GetItem(uint32_t aID, uint16_t& aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	aOutValue = atoi(GetValueFromMap(aID));
	return true;
}

bool ZFixApi::GetItem(uint32_t aID, int32_t& aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	aOutValue = atoi(GetValueFromMap(aID));
	return true;
}

bool ZFixApi::GetItem(uint32_t aID, uint32_t& aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	aOutValue = atoi(GetValueFromMap(aID));
	return true;
}

bool ZFixApi::GetItem(uint32_t aID, int64_t& aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	aOutValue = atoll(GetValueFromMap(aID));
	return true;
}

bool ZFixApi::GetItem(uint32_t aID, char* aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	strcpy(aOutValue, GetValueFromMap(aID));
	return true;
}

bool ZFixApi::GetItem(uint32_t aID, string& aOutValue)
{
	if (!HaveItem(aID)) {
		return false;
	}

	aOutValue = string(GetValueFromMap(aID));
	return true;
}


bool ZFixApi::HaveItem(uint32_t aID)
{
	if (m_FixMap.empty())
	{
		DecodeData2Map();
	}

	if (m_FixMap.count(aID))
	{
		return true;
	}

	return false;
}

char* ZFixApi::GetValueFromMap(uint32_t aID)
{
	if (m_FixMap.count(aID)) {
		return m_FixMap[aID];
	}
	return "";
}

bool ZFixApi::GetKeyAndValue(char* apData, char** appValue)
{
	char* lpFind = strchr(apData, '=');
	if (!lpFind) {
		return false;
	}

	*appValue = lpFind + 1;
	return true;
}

int ZFixApi::DecodeData2Map()
{
	if (m_Length == 0) {
		return -1;
	}

	int lArrSize = 128;
	char* lpArr[128] = {};
	lArrSize = zfix_str_delimiter(m_pBuff, m_Length, lpArr, lArrSize, ZFIX_DELIMITER);
	if (lArrSize == 0) {
		return -2;
	}

	uint32_t lKey = 0;
	char* lpValue = nullptr;
	char* lpData = nullptr;
	for (int i = 0; i < lArrSize; ++i)
	{
		lpData = lpArr[i];

		if (!GetKeyAndValue(lpData, &lpValue)) {
			continue;
		}

		lKey = atoi(lpData);
		m_FixMap[lKey] = lpValue;
	}

	return 0;
}


int zfix_str_delimiter(char* apSrc, int aLength, char** apRetArr, int aArrSize, char aDelimiter)
{
	if (!apSrc) {
		return 0;
	}

	char* lpEnd = apSrc + aLength;
	char* lpCur = apSrc;
	int n = 0;
	while (n < aArrSize)
	{
		apRetArr[n++] = lpCur;

		while (*lpCur != aDelimiter)
		{
			++lpCur;
			if (lpCur >= lpEnd) {
				lpCur = NULL;
				break;
			}
		}

		if (!lpCur) {
			break;
		}

		*lpCur++ = 0x00;
	}
	return n;
}
