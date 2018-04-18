#pragma once

#include <thread>

#include <stdint.h>

#include <ZToolLib/ztl_network.h>

using namespace std;


/* 一个Message封装类
 */
class ZNetMessage
{
public:
	ZNetMessage();
	~ZNetMessage();

	int Append(void* apData, uint32_t aDataSize);

public:
	char* GetRawBegin() {
		return (char*)(this + 1);
	}

	uint32_t Capacity() const {
		return m_Capacity;
	}

	uint32_t Size() const {
		return m_Size;
	}

	uint32_t SizeAvail() const {
		return m_Capacity - m_Size;
	}

public:
	static ZNetMessage* Alloc(uint32_t aMsgSize);
	static void Release(ZNetMessage* apMessage);

	static ZNetMessage* Clone(ZNetMessage* apMessage);

public:
	ZNetMessage*	m_pNext;
	uint32_t		m_Capacity;
	uint32_t		m_Size;
};


#define ZCOMM_DEFAULT_SIZE	2048


typedef void(*ZOnNetMessagePtr)(void* apUserData, ZNetMessage* apMessage);

class ZNetConfig
{
public:
	uint32_t	m_Type;			// 0:udp 1:tcp
	uint16_t	m_ServerPort;	// default is 40594
	uint32_t	m_ServerAddr;
	uint32_t	m_GroupAddr;	// default group ip 229.5.9.4

	void*				m_pUserData;
	ZOnNetMessagePtr	m_pFunc;
};

/* 网络通信基类
 */
class ZNetCommBase
{
public:
	ZNetCommBase();
	virtual ~ZNetCommBase();

	virtual int Init(const ZNetConfig& aNetConf) = 0;
	virtual int Start() = 0;
	virtual int Stop() = 0;

	virtual int DirectRecv(char* apRawBuf, uint32_t* apBytesRecv) = 0;
	virtual int DirectSend(const char* apRawData, uint32_t aRawSize) = 0;

protected:
	ZNetConfig			m_NetConf;
	sockhandle_t		m_Sock;
};

/* 基于UDP的通信封装
 */
class ZUdpComm : public ZNetCommBase
{
public:
	ZUdpComm();
	virtual ~ZUdpComm();

public:
	virtual int Init(const ZNetConfig& aNetConf);
	virtual int Start();
	virtual int Stop();

	virtual int DirectRecv(char* apRawBuf, uint32_t* apBytesRecv);
	virtual int DirectSend(const char* apRawData, uint32_t aRawSize);

protected:
	virtual void Run();

protected:
	int32_t				m_Running;
	unique_ptr<thread>	m_pIOThread;
	sockhandle_t		m_Sender;
};
