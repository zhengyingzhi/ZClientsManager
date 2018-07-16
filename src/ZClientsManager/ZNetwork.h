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

	bool AddUsed(uint32_t aUsedSize);

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
	static ZNetMessage* Alloc(uint32_t aAllocSize);
	static void Release(ZNetMessage* apMessage);

	static ZNetMessage* Clone(ZNetMessage* apMessage);

public:
	ZNetMessage*	m_pNext;
	uint32_t		m_Capacity;
	uint32_t		m_Size;
};


typedef void(*ZOnNetMessagePtr)(void* apUserData, ZNetMessage* apMessage);

typedef void(*ZOnLoopOncePtr)(void* apUserData);


#define ZCOMM_DEFAULT_SIZE		2048
#define ZCOMM_DEFAULT_SO_BUFSZ	(4 * 1024 * 1024)

#define ZNET_DEFAULT_PORT		40594
#define ZNET_DEFAULT_ANYIP		"0.0.0.0"
#define ZNET_DEFAULT_GROUPIP	"229.5.9.4"

#define ZNET_TYPE_UDP_MULTICAST 0
#define ZNET_TYPE_UDP_BROADCAST 1
#define ZNET_TYPE_TCP			2
class ZNetConfig
{
public:
	uint32_t	m_Type;			// 0:udp 1:tcp
	uint32_t	m_IsBroadcast;	// is udp broadcast
	uint32_t	m_PeerAddr;		// for sender peer inet address
	uint16_t	m_PeerPort;		// for sender peer port default is 40594
	uint16_t	m_BindPort;		// for receiver bind port
	uint32_t	m_BindAddr;		// for receiver 

	uint32_t	m_GroupAddr;	// for receiver, default group ip 229.5.9.4

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

    virtual bool IsRunning() {
        return m_Running > 0;
    }

public:
	void SetLoopOnceFunc(ZOnLoopOncePtr apFunc, void* apUserData);

protected:
	ZNetConfig			m_NetConf;
	ZOnLoopOncePtr		m_LoopOnce;
    void*				m_pUserData;
    volatile int32_t	m_Running;
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
	unique_ptr<thread>	m_pIOThread;
	sockhandle_t		m_Sender;
	sockhandle_t		m_Recver;
};
