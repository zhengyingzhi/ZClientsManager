
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

ZNetMessage* ZNetProtocol::MakeNetMessage(uint32_t aProtoType, uint32_t aMsgType, const void* apDataInfo, uint32_t aDataSize)
{
	uint32_t lSize = sizeof(ZNetHead) + sizeof(ZMsgDesc) + aDataSize + 8;
	ZNetMessage* lpMessage;
	lpMessage = ZNetMessage::Alloc(lSize);

	lpMessage->AddUsed(lSize);

	// fill header fields
	ZNetHead*	lpProtoHead;
	ZMsgDesc*	lpMsgDesc;
	void*		lpDataInfo;
	ExtractNetMessage(lpMessage, &lpProtoHead, &lpMsgDesc, (void**)&lpDataInfo);

	lpProtoHead->m_StartType	= ZNET_STARTTYPE_CM;
	lpProtoHead->m_Version		= ZNET_VERSION;
	lpProtoHead->m_Type			= aProtoType;
	lpProtoHead->m_Length		= sizeof(ZMsgDesc) + aDataSize;
	lpProtoHead->m_Flag			= 0;

	lpMsgDesc->m_DataSize		= aDataSize;
	lpMsgDesc->m_Count			= 1;
	lpMsgDesc->m_MsgType			= aMsgType;
	lpMsgDesc->m_BegSeq			= 1;

	if (apDataInfo && aDataSize > 0) {
		memcpy(lpDataInfo, apDataInfo, aDataSize);
	}

	return lpMessage;
}

ZNetHead* ZNetProtocol::GetMessageNetHead(ZNetMessage* apMessage)
{
	return (ZNetHead*)apMessage->GetRawBegin();
}

void ZNetProtocol::ExtractNetMessage(ZNetMessage* apMessage, ZNetHead** appProtoHead, ZMsgDesc** appMsgDesc, void** appDataInfo)
{
	char* lpRawBegin = apMessage->GetRawBegin();

	if (appProtoHead) {
		*appProtoHead = (ZNetHead*)lpRawBegin;
	}

	if (appMsgDesc) {
		*appMsgDesc = (ZMsgDesc*)(lpRawBegin + sizeof(ZNetHead));
	}

	if (appDataInfo) {
		*appDataInfo = (void*)(lpRawBegin + sizeof(ZNetHead) + sizeof(ZMsgDesc));
	}
}

uint32_t ZNetProtocol::NetMessagePreSize()
{
	return sizeof(ZNetHead) + sizeof(ZMsgDesc);
}
