#pragma once

#include <stdint.h>

#define ZUSER_ROLE_Visitor      0       // �ÿ�
#define ZUSER_ROLE_Normal       1       // ��ͨ�û�������������ݣ�
#define ZUSER_ROLE_Admin        2       // ��ͨ����Ա������ӡ�ɾ���ͻ���Ϣ��
#define ZUSER_ROLE_Root         4       // ��������Ա���ɹ����û��Ϳͻ���Ϣ��

#define ZUSER_CIPHER_None       0       // δ����
#define ZUSER_CIPHER_Simple     1       // �򵥼���
#define ZUSER_CIPHER_AES        2       // AES����

typedef struct tagZUserInfo
{
	char        UserName[32];
	char        Password[24];
	uint32_t    Role;
	int32_t     Cipher;
}ZUserInfo;
