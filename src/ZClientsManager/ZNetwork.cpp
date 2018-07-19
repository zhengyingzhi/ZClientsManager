#include <stdint.h>
#include <string.h>

#include <new>

#include <ZToolLib/ztl_utils.h>

#include "ZNetwork.h"

#include "ZUtility.h"


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

bool ZNetMessage::AddUsed(uint32_t aUsedSize)
{
	if (m_Size + aUsedSize > m_Capacity) {
		return false;
	}

	m_Size += aUsedSize;
	return true;
}

ZNetMessage* ZNetMessage::Alloc(uint32_t aAllocSize)
{
	ZNetMessage* lpRet;
	uint32_t lCapacity = ztl_align(aAllocSize, 64);

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
	, m_LoopOnce()
	, m_pUserData()
    , m_Running(0)
{}

ZNetCommBase::~ZNetCommBase()
{
}

void ZNetCommBase::SetLoopOnceFunc(ZOnLoopOncePtr apFunc, void* apUserData)
{
	m_LoopOnce = apFunc;
	m_pUserData = apUserData;
}



ZUdpComm::ZUdpComm()
	: m_pIOThread()
	, m_Sender(INVALID_SOCKET)
	, m_Recver(INVALID_SOCKET)
{}

ZUdpComm::~ZUdpComm()
{
	if (m_Sender != INVALID_SOCKET)
	{
		close_socket(m_Sender);
	}

	if (m_Recver != INVALID_SOCKET)
	{
		close_socket(m_Recver);
	}
}

int ZUdpComm::Init(const ZNetConfig& aNetConf)
{
	m_NetConf = aNetConf;

	int rv;
	char lMultiIP[32] = "";
	inetaddr_to_string(lMultiIP, sizeof(lMultiIP), aNetConf.m_GroupAddr);

	// receiver
	if (aNetConf.m_BindAddr > 0 || aNetConf.m_BindPort > 0)
	{
		m_Recver = create_socket(SOCK_DGRAM);
		set_reuseaddr(m_Recver, true);
		set_rcv_buffsize(m_Recver, ZCOMM_DEFAULT_SO_BUFSZ);

		// bind
		struct sockaddr_in laddr;
		memset(&laddr, 0, sizeof laddr);
		make_sockaddr(&laddr, ZNET_DEFAULT_ANYIP, aNetConf.m_BindPort);
		if ((rv = ::bind(m_Recver, (struct sockaddr*)&laddr, sizeof(laddr))) < 0)
		{
			char lErrorMsg[512] = "";
			sprintf(lErrorMsg, "ZUdpComm init error %d", get_errno());
			ZDebug(lErrorMsg);

			close_socket(m_Recver);
			m_Recver = INVALID_SOCKET;
			return rv;
		}
		
		if (aNetConf.m_GroupAddr > 0 && !aNetConf.m_IsBroadcast)
		{
			//enable_multicast_loopback(m_Recver, false);
			//set_multicase_interface(m_Recver, ZNET_DEFAULT_ANYIP);

#if 1
			struct ip_mreq mreqInfo;
			memset(&mreqInfo, 0, sizeof(mreqInfo));
			mreqInfo.imr_interface.s_addr = aNetConf.m_BindAddr;
			mreqInfo.imr_multiaddr.s_addr = aNetConf.m_GroupAddr;
			rv = setsockopt(m_Recver, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreqInfo, sizeof(mreqInfo));
#else
			rv = join_multicast(m_Recver, lMultiIP, ZNET_DEFAULT_ANYIP);
#endif
			if (0 != rv)
			{
				char lErrorMsg[512] = "";
				sprintf(lErrorMsg, "receiver join_multicast failed:%d", get_errno());
				ZDebug(lErrorMsg);

				close_socket(m_Recver);
				m_Recver = INVALID_SOCKET;
				return -1;
			}
		}
	}
	
	// sender
	m_Sender = create_socket(SOCK_DGRAM);
	set_snd_buffsize(m_Sender, ZCOMM_DEFAULT_SO_BUFSZ);

	if (aNetConf.m_GroupAddr > 0)
	{
		if (aNetConf.m_IsBroadcast)
		{
			set_broadcast(m_Sender, true);
			enable_multicast_loopback(m_Sender, false);
			set_multicast_ttl(m_Sender, 4);
		}
		else
		{
			enable_multicast_loopback(m_Sender, false);
			set_multicast_ttl(m_Sender, 4);
			set_multicase_interface(m_Sender, ZNET_DEFAULT_ANYIP);

#if 1
			struct ip_mreq mreqInfo;
			memset(&mreqInfo, 0, sizeof(mreqInfo));
			mreqInfo.imr_interface.s_addr = aNetConf.m_BindAddr;
			mreqInfo.imr_multiaddr.s_addr = aNetConf.m_GroupAddr;
			rv = setsockopt(m_Sender, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreqInfo, sizeof(mreqInfo));
#else
			inetaddr_to_string(lMultiIP, sizeof(lMultiIP), aNetConf.m_GroupAddr);
			rv = join_multicast(m_Sender, lMultiIP, ZNET_DEFAULT_ANYIP);
#endif
			if (0 != rv)
			{
				char lErrorMsg[512] = "";
				sprintf(lErrorMsg, "sender join_multicast failed:%d", get_errno());
				OutputDebugString(lErrorMsg);

				close_socket(m_Sender);
				m_Sender = INVALID_SOCKET;
				return -2;
			}
		}
	}
	else if (aNetConf.m_IsBroadcast)
	{
		set_broadcast(m_Sender, true);
		enable_multicast_loopback(m_Sender, false);
		set_multicast_ttl(m_Sender, 4);
	}

	return 0;
}

int ZUdpComm::Start()
{
	if (m_Running) {
		return -1;
	}

	// not init receiver
	if (m_Recver == INVALID_SOCKET) {
		return -2;
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

	m_LoopOnce = NULL;

	if (m_Recver != INVALID_SOCKET)
	{
		close_socket(m_Recver);
		m_Recver = INVALID_SOCKET;
	}

	if (m_pIOThread)
	{
		m_pIOThread->join();
	}
	return 0;
}

int ZUdpComm::DirectRecv(char* apRawBuf, uint32_t* apBytesRecv)
{
	if (m_Recver == INVALID_SOCKET) {
		return -2;
	}

	struct sockaddr_in lPeerAddr = {};
	int rv = udp_recv(m_Recver, apRawBuf, *apBytesRecv, (sockaddr_in*)&lPeerAddr, 1000);
	if (rv > 0)
	{
		char lPeerIP[32] = "";
		uint16_t lPeerPort = ntohs(lPeerAddr.sin_port);
		inet_ntop(AF_INET, &lPeerAddr.sin_addr, lPeerIP, sizeof(lPeerIP));

		*apBytesRecv = rv;
		return rv;
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
	if (m_Sender == INVALID_SOCKET) {
		return -1;
	}

	struct sockaddr_in lToAddr = {};
	lToAddr.sin_family = AF_INET;
	lToAddr.sin_port = htons(m_NetConf.m_PeerPort);
	if (m_NetConf.m_GroupAddr > 0)
		lToAddr.sin_addr.s_addr = m_NetConf.m_GroupAddr;
	else
		lToAddr.sin_addr.s_addr = m_NetConf.m_PeerAddr;
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

		lBytesRecv = lpMessage->SizeAvail();
		rv = DirectRecv(lpMessage->GetRawBegin(), &lBytesRecv);
		if (rv <= 0)
		{
			// try auto reconnect if tcp
			ZOnLoopOncePtr LoopOnce = m_LoopOnce;
			if (LoopOnce)
				LoopOnce(m_pUserData);
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



