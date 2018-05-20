#pragma once

#include <stdint.h>


#define ZUSER_ROLE_Visitor				0           // �ÿ�
#define ZUSER_ROLE_Normal				1           // ��ͨ�û�������������ݣ�
#define ZUSER_ROLE_Admin				2           // ��ͨ����Ա������ӡ�ɾ���ͻ���Ϣ��
#define ZUSER_ROLE_Root					4           // ��������Ա���ɹ����û��Ϳͻ���Ϣ��

#define ZUSER_CIPHER_None				0           // δ����
#define ZUSER_CIPHER_Simple				1           // �򵥼���
#define ZUSER_CIPHER_AES				2           // AES����

#define ZUSER_FLAG_Deleted				2

#define ZUSER_DEFAULT_RootName			"root"
#define ZUSER_DEFAULT_RootPasswd		"111111"

#define ZUSER_INFO_Version				10000

#define ZUSER_SimpleChange              true


typedef struct tagZUserInfo
{
	char        UserName[24];           // ����
	char        Password[24];           // ����
	char        Telephone[16];          // �绰
	char        QQ[16];                 // QQ
	char		IDNumber[20];			// ���֤��
	uint32_t    Number;                 // ���
	uint32_t    Role;                   // ��ɫ
	uint32_t	Department;             // ����
	int32_t     Cipher;                 // ���ܷ�ʽ
	int32_t		ExpireDate;				// ��������
	int64_t     InsertTime;             // ����ʱ��
	int64_t     UpdateTime;             // ����ʱ��
	uint32_t    Flags;                  // Ԥ���ֶ�
	char        Padding1[32];           //
	char        Padding2[128];          //
	char        Comment[256];           // ��ע
}ZUserInfo;


extern int ZUserInfoFixString(const ZUserInfo* apUserInfo, char* apBuffer, uint32_t aPrePaddingSize);

extern int ZFixString2UserInfo(char* apString, uint32_t aLength, uint32_t aPrePaddingSize, ZUserInfo* apUserInfo);

extern void ZUserInfoCopy(ZUserInfo* apDstInfo, const ZUserInfo* apSrcInfo);
