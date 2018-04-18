#include <stdint.h>
#include <string.h>

#include <new>

#include <ZToolLib/ztl_utils.h>

#include "ZNetwork.h"

ZNetMessage::ZNetMessage()
	: m_Capacity(0)
	, m_Size(0)
{
}

ZNetMessage::~ZNetMessage()
{}

int ZNetMessage::Append(void* apData, uint32_t aDataSize)
{
	if (m_Size + aDataSize > m_Capacity)
	{
		return -1;
	}

	memcpy(GetRawBegin() + m_Size, apData, aDataSize);
	m_Size += aDataSize;

	return 0;
}

ZNetMessage* ZNetMessage::Alloc(uint32_t aMsgSize)
{
	ZNetMessage* lpRet;
	uint32_t lCapacity = ztl_align(aMsgSize, 64);

	char* lpRaw = new char[lCapacity + sizeof(ZNetMessage) + 4];
	memset(lpRaw, 0, lCapacity);

	lpRet = new (lpRaw) ZNetMessage();

	lpRet->m_Capacity = lCapacity;
	lpRet->m_Size = 0;

	return lpRet;
}


void ZNetMessage::Release(ZNetMessage* apMessage)
{
	apMessage->~ZNetMessage();
	delete apMessage;
}

ZNetMessage* ZNetMessage::Clone(ZNetMessage* apMessage)
{
	ZNetMessage* lpClone;
	lpClone = Alloc(apMessage->Capacity());

	lpClone->Append(apMessage->GetRawBegin(), apMessage->Size());
	return lpClone;
}


//////////////////////////////////////////////////////////////////////////
ZNetCommBase::ZNetCommBase()
	: m_NetConf()
	, m_Sock(INVALID_SOCKET)
{}

ZNetCommBase::~ZNetCommBase()
{
	if (m_Sock != INVALID_SOCKET)
	{
		close_socket(m_Sock);
	}

}


ZUdpComm::ZUdpComm()
	: m_Running(0)
	, m_pIOThread()
	, m_Sender(INVALID_SOCKET)
{}

ZUdpComm::~ZUdpComm()
{
	if (m_Sender != INVALID_SOCKET)
	{
		close_socket(m_Sender);
	}
}

int ZUdpComm::Init(const ZNetConfig& aNetConf)
{
	m_NetConf = aNetConf;

	// receiver
	m_Sock = create_socket(SOCK_DGRAM);
	set_reuseaddr(m_Sock, true);
	set_rcv_buffsize(m_Sock, 4 * 1024 * 1024);

	set_multicase_interface(m_Sock, "");
	enable_multicast_loopback(m_Sock, false);
	set_multicast_ttl(m_Sender, 4);

	char lMultiIP[32] = "";
	inetaddr_to_string(lMultiIP, sizeof(lMultiIP), aNetConf.m_GroupAddr);
	if (0 != join_multicast(m_Sock, lMultiIP, ""))
	{
		char lErrorMsg[512] = "";
		sprintf(lErrorMsg, "receiver join_multicast failed:%d", get_errno());
		OutputDebugString(lErrorMsg);

		return -1;
	}

	// sender
	m_Sender = create_socket(SOCK_DGRAM);
	enable_multicast_loopback(m_Sender, false);
	set_multicast_ttl(m_Sender, 4);
	inetaddr_to_string(lMultiIP, sizeof(lMultiIP), aNetConf.m_GroupAddr);
	if (0 != join_multicast(m_Sender, lMultiIP, ""))
	{
		char lErrorMsg[512] = "";
		sprintf(lErrorMsg, "sender join_multicast failed:%d", get_errno());
		OutputDebugString(lErrorMsg);

		return -2;
	}

	return 0;
}

int ZUdpComm::Start()
{
	if (m_Running) {
		return -1;
	}

	m_Running = 1;
	m_pIOThread.reset(new thread(&ZUdpComm::Run, this));
	return 0;
}

int ZUdpComm::Stop()
{
	if (m_Running == 0) {
		return -1;
	}

	m_Running = 0;
	if (m_pIOThread)
	{
		m_pIOThread->join();
	}
	return 0;
}

int ZUdpComm::DirectRecv(char* apRawBuf, uint32_t* apBytesRecv)
{
	struct sockaddr_in lPeerAddr = {};
	int rv = udp_recv(m_Sock, apRawBuf, *apBytesRecv, (sockaddr_in*)&lPeerAddr, 100);
	if (rv > 0)
	{
		*apBytesRecv = rv;
	}
	else
	{
		*apBytesRecv = 0;
		return get_errno();
	}

	// FIXME: 过滤掉本机发出去的信息

	return 0;
}

int ZUdpComm::DirectSend(const char* apRawData, uint32_t aRawSize)
{
	struct sockaddr_in lToAddr = {};
	lToAddr.sin_family = AF_INET;
	lToAddr.sin_port = htons(m_NetConf.m_ServerPort);
	lToAddr.sin_addr.s_addr = m_NetConf.m_GroupAddr;
	return udp_send(m_Sender, apRawData, aRawSize, &lToAddr);
}

void ZUdpComm::Run()
{
	int rv = 0;
	uint32_t lBytesRecv = 0;

	ZNetMessage* lpMessage = nullptr;
	while (m_Running)
	{
		if (lpMessage == nullptr)
		{
			lBytesRecv = 0;
			lpMessage = ZNetMessage::Alloc(ZCOMM_DEFAULT_SIZE);
		}

		lBytesRecv = ZCOMM_DEFAULT_SIZE;
		rv = DirectRecv(lpMessage->GetRawBegin(), &lBytesRecv);
		if (rv != 0)
		{
			// try auto reconnect
			continue;
		}

		lpMessage->m_Size += lBytesRecv;

		if (m_NetConf.m_pFunc) {
			m_NetConf.m_pFunc(m_NetConf.m_pUserData, lpMessage);
		}
		else {
			ZNetMessage::Release(lpMessage);
		}
		lpMessage = nullptr;
	}
}



