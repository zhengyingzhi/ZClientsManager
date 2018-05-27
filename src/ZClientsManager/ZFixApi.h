#pragma once

#include <string>
#include <unordered_map>

#include "ZFieldDict.h"

using namespace std;


#define ZFIX_BUF_SIZE	4000
#define ZFIX_DELIMITER	0x01

class ZFixApi
{
public:
	ZFixApi();
	~ZFixApi();

	/* when Clear, we should re-call SetBuffer if need */
	void Clear();

	void SetBuffer(char* apRawBuffer);
	void SetLength(uint32_t aRawLength);

	char*       Data() const;
	uint32_t    Length() const;

public:
	void SetItem(uint32_t aID, char aValue);
	void SetItem(uint32_t aID, const uint16_t aValue);
	void SetItem(uint32_t aID, const int32_t aValue);
	void SetItem(uint32_t aID, const uint32_t aValue);
	void SetItem(uint32_t aID, int64_t aValue);
	void SetItem(uint32_t aID, const char* apValue, int aLength = -1);
	void SetItem(uint32_t aID, const std::string& aValue);

	bool GetItem(uint32_t aID, char& aOutValue);
	bool GetItem(uint32_t aID, uint16_t& aOutValue);
	bool GetItem(uint32_t aID, int32_t& aOutValue);
	bool GetItem(uint32_t aID, uint32_t& aOutValue);
	bool GetItem(uint32_t aID, int64_t& aOutValue);
	bool GetItem(uint32_t aID, char* aOutValue);
	bool GetItem(uint32_t aID, string& aOutValue);

	bool HaveItem(uint32_t aID);

protected:
	uint32_t AvailSize();

	char* GetValueFromMap(uint32_t aID);
	bool GetKeyAndValue(char* apData, char** appValue);
	int  DecodeData2Map();

private:
	char        m_Buffer[ZFIX_BUF_SIZE];
	char*       m_pBuff;
	uint32_t    m_Length;       // data length without pre padding size
	uint32_t	m_Capacity;

	unordered_map<uint32_t, char*> m_FixMap;
};


