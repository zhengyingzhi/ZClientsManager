#pragma once

#include <stdint.h>

#include "StudentInfo.h"

#include "ZNetwork.h"

/* ��ƣ�
 * 1. ������/�޸�����ʱ������һ�� Publish ����Ϣ�����ն�Ӧ��������ʾ������
 * 2. ���������ʱ����ȫ�ַ��� Query ����Ϣ����ʾ��ѯ��ǰ���µ���Ϣ���ɴ�����ţ���
 *    ���ն��յ���Ϣ�����Ӧ��Ϣ����Ӧ���ܻ��ж������ݣ���Ϣ��Я��Count + ��ʼ��ţ�������ж������Ƿ�ʧ
 * 3. �յ���Ϣ QueryRsp ��Ϣ�Ķˣ�Ӧ��������Ϣ�е����ݣ��ڱ��رȽϣ��������ˣ������� ��δ��Query�Ķˣ����ֱ�Ӻ��Ը�����Ϣ��
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
