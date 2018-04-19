
#include "ZNetProtocol.h"


bool ZNetProtocol::IsValidHead(void* apAddr)
{
	ZNetHead* lpHead = (ZNetHead*)apAddr;
	if (lpHead->m_StartType != ZNET_STARTTYPE_CM) {
		return false;
	}

	if (lpHead->m_Version != ZNET_VERSION) {
		return false;
	}

	return true;
}

ZNetMessage* ZNetProtocol::MakeNetMessage(uint32_t aType, const void* apDataInfo, uint32_t aDataSize)
{
	uint32_t lSize = sizeof(ZNetHead) + sizeof(ZMsgHead) + sizeof(ZStudentInfo) + 8;
	ZNetMessage* lpMessage;
	lpMessage = ZNetMessage::Alloc(lSize);

	lpMessage->AddUsed(lSize);

	// fill header fields
	ZNetHead*	lpProtoHead;
	ZMsgHead*	lpMsgHed;
	void*		lpDataInfo;
	ExtractNetMessage(lpMessage, &lpProtoHead, &lpMsgHed, (void**)&lpDataInfo);

	lpProtoHead->m_StartType	= ZNET_STARTTYPE_CM;
	lpProtoHead->m_Version		= ZNET_VERSION;
	lpProtoHead->m_Type			= aType;
	lpProtoHead->m_Length		= sizeof(ZMsgHead) + aDataSize;
	lpProtoHead->m_Flag			= 0;

	lpMsgHed->m_DataSize		= aDataSize;
	lpMsgHed->m_Count			= 1;
	lpMsgHed->m_BegSeq			= 1;

	memcpy(lpDataInfo, apDataInfo, aDataSize);

	return lpMessage;
}

void ZNetProtocol::ExtractNetMessage(ZNetMessage* apMessage, ZNetHead** appProtoHead, ZMsgHead** appMsgHead, void** appDataInfo)
{
	char* lpRawBegin = apMessage->GetRawBegin();

	if (appProtoHead) {
		*appProtoHead = (ZNetHead*)lpRawBegin;
	}

	if (appMsgHead) {
		*appMsgHead = (ZMsgHead*)(lpRawBegin + sizeof(ZNetHead));
	}

	if (appDataInfo) {
		*appDataInfo = (void*)(lpRawBegin + sizeof(ZNetHead) + sizeof(ZMsgHead));
	}
}
