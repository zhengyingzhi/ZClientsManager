#pragma once

#include <stdint.h>

#define ZUSER_ROLE_Visitor      0       // 访客
#define ZUSER_ROLE_Normal       1       // 普通用户（仅可浏览数据）
#define ZUSER_ROLE_Admin        2       // 普通管理员（可添加、删除客户信息）
#define ZUSER_ROLE_Root         4       // 超级管理员（可管理用户和客户信息）

#define ZUSER_CIPHER_None       0       // 未加密
#define ZUSER_CIPHER_Simple     1       // 简单加密
#define ZUSER_CIPHER_AES        2       // AES加密

typedef struct tagZUserInfo
{
	char        UserName[32];           // 名字
	char        Password[24];           // 密码
	char        Telephone[16];          // 电话
	uint32_t    Number;                 // 编号
	uint32_t    Role;                   // 角色
	int32_t     Cipher;                 // 加密方式
	int32_t     Reserve1;               // 预留字段

	char        Comment[256];           // 备注
}ZUserInfo;
