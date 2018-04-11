#pragma once

#include <stdint.h>


#define ZUSER_ROLE_Visitor          0           // �ÿ�
#define ZUSER_ROLE_Normal           1           // ��ͨ�û�������������ݣ�
#define ZUSER_ROLE_Admin            2           // ��ͨ����Ա������ӡ�ɾ���ͻ���Ϣ��
#define ZUSER_ROLE_Root             4           // ��������Ա���ɹ����û��Ϳͻ���Ϣ��

#define ZUSER_CIPHER_None           0           // δ����
#define ZUSER_CIPHER_Simple         1           // �򵥼���
#define ZUSER_CIPHER_AES            2           // AES����

typedef struct tagZUserInfo
{
	char        UserName[24];           // ����
	char        Password[24];           // ����
	char        Telephone[16];          // �绰
	char        QQ[16];                 // QQ
	uint32_t    Number;                 // ���
	uint32_t    Role;                   // ��ɫ
	int32_t     Cipher;                 // ���ܷ�ʽ
	int32_t     Deleted;                // Ԥ���ֶ�
	char        Padding1[32];           //
	char        Padding2[128];          //
	char        Comment[256];           // ��ע
}ZUserInfo;
