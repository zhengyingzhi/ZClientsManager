#pragma once

#include <stdint.h>

#include "StudentInfo.h"

#include "ZNetwork.h"

/* 设计：
 * 1. 有新增/修改数据时，则发送一条 Publish 的消息，接收端应当处理并显示该数据
 * 2. 程序刚启动时，向全局发送 Query 的消息，表示查询当前最新的消息（可带上序号），
 *    接收端收到消息后，则回应消息，回应可能会有多条数据，消息中携带Count + 起始序号，请求端判断数据是否丢失
 * 3. 收到消息 QueryRsp 消息的端，应当根据消息中的内容，在本地比较，并做过滤，或新增 （未做Query的端，则可直接忽略该类消息）
 */

#define ZNET_STARTTYPE_CM		0x4D43	// CM
#define ZNET_VERSION			0x0100	// 1.0

#define ZNET_T_Query			3		// query request
#define ZNET_T_QueryRsp			4		// query respone
#define ZNET_T_PublishAdd		5		// publish new added data
#define ZNET_T_PublishUpdate	6		// publish new updated data

#define ZNET_MSG_UserInfo		1		// user info message
#define ZNET_MSG_StuInfo		2		// student info message

typedef struct  
{
	uint16_t	m_StartType;			// start type
	uint16_t	m_Version;				// version
	uint32_t	m_Type;					// func type
	uint32_t	m_Length;				// body length
	uint32_t	m_Flag;					// header flag
}ZNetHead;

typedef struct 
{
	uint32_t	m_DataSize;			// the actual data size, like sizeof(ZStudentInfo)
	uint32_t	m_MsgType;			// UserInfo or StuInfo
	uint32_t	m_Count;			// message count for this publish
	uint32_t	m_BegSeq;			// begin sequence
}ZMsgHead;

class ZNetProtocol
{
public:
	static ZNetMessage* MakeNetMessage(uint32_t aProtoType, uint32_t aMsgType, const void* apDataInfo, uint32_t aDataSize);

	static void ExtractNetMessage(ZNetMessage* apMessage, ZNetHead** appProtoHead, ZMsgHead** appMsgHead, void** appDataInfo);

	static bool IsValidHead(void* apAddr);

};
